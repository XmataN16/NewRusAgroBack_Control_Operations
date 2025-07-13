// Новый calcMinimalDate, адаптированный под структуры SapData и InitialData
#include "CalcMinimalDate.hpp"
#include "utilsBoostDate.hpp"

using boost::gregorian::date;

#include <string>
#include <optional>
#include <boost/date_time/gregorian/gregorian.hpp>

std::optional<std::string> setStatus(const std::optional<date>& actual_date, const std::optional<date>& minimal_date, const date& today) 
{
    if (!minimal_date) return u8"Операция не отслеживается";

    if (!actual_date) 
    {
        if (today < minimal_date.value()) return u8"Не завершено";
        else return u8"Просрочено";
    }
    else 
    {
        if (*actual_date <= minimal_date.value()) return u8"Выполнено в срок";
        else return u8"Выполнено не в срок";
    }
}

std::optional<std::string> setIsActual(const std::optional<std::string>& status, const std::optional<date>& actual_data, const std::optional<date>& actual_input_data, const std::optional<date>& actual_alternative_data) 
{
    if (!status || *status == u8"Операция не отслеживается")
        return u8"Статус отсутствует";

    if (actual_data || actual_input_data || actual_alternative_data)
        return u8"Актуально";
    else
        return u8"Ориентировочно";
}

void calcMinimalDate(YearSlices& uniqueSlices, const InitialData& initData) 
{
    using namespace boost::gregorian;

    // Получаем текущую дату для статуса
    date today = day_clock::local_day();

    for (auto& [year, higherTmMap] : uniqueSlices) 
    {
        for (auto& [higher_tm, sliceList] : higherTmMap) 
        {
            for (auto& slice : sliceList) 
            {
                for (auto& frame : slice) 
                {
                    // Поиск соответствующей записи в InitialData
                    KeyCRTYS5 key
                    {
                        frame.culture_id,
                        frame.region_id,
                        frame.t_material_id,
                        frame.year,
                        frame.season
                    };
                    auto it = initData.CRTYS_index_map.find(key);
                    if (it == initData.CRTYS_index_map.end()) 
                    {
                        frame.minimal_date = std::nullopt;
                        frame.status = u8"Операция не отслеживается";
                        frame.is_actual = u8"Статус отсутствует";
                        continue;
                    }

                    const InitialDataFrame& initFrame = initData.data[it->second];

                    frame.order = initFrame.order;

                    // Вычисление min_plan_date
                    std::optional<date> min_plan_date;
                    if (initFrame.input_operation_order.has_value()) 
                    {
                        min_plan_date = initFrame.planned_dates.minimal_planned_date;
                    }
                    else if (frame.start_date.has_value()) 
                    {
                        min_plan_date = frame.start_date.value() + days(initFrame.noinput_deadline.value_or(0));
                    }
                    else 
                    {
                        min_plan_date = std::nullopt;
                    }

                    // Выбор минимальной даты
                    std::vector<std::optional<date>> dates;
                    if (min_plan_date) dates.push_back(min_plan_date.value());
                    if (frame.actual_input_date) dates.push_back(frame.actual_input_date.value());
                    if (frame.actual_alternative_date) dates.push_back(frame.actual_alternative_date.value());

                    if (dates.empty()) 
                    {
                        frame.minimal_date = std::nullopt;
                    }
                    else 
                    {
                        frame.minimal_date = *std::min_element(dates.begin(), dates.end(),
                            [](const auto& a, const auto& b) { return a < b; });
                    }

                    // Вычисление status
                    frame.status = setStatus(frame.actual_date, frame.minimal_date, today);
                    frame.is_actual = setIsActual(frame.status, frame.actual_date,
                        frame.actual_input_date, frame.actual_alternative_date);
                }
            }
        }
    }
}
