#include "CalcActualDate.hpp"
#include "utilsBoostDate.hpp"
#include "utilsOptional.hpp"

// Для удобства создаём индекс: t_material_id -> list of SapDataFrame (из полной выборки)
using TMaterialIndex = std::unordered_map<int, std::vector<const SapDataFrame*>>;

TMaterialIndex BuildTMaterialIndex(const YearSlices& fullSlices)
{
    TMaterialIndex index;
    for (const auto& [_, tmMap] : fullSlices)
    {
        for (const auto& [_, sliceList] : tmMap)
        {
            for (const auto& slice : sliceList)
            {
                for (const auto& frame : slice)
                {
                    index[frame.t_material_id].push_back(&frame);
                }
            }
        }
    }
    return index;
}

void CalcActualDate(YearSlices& uniqueSlices, const YearSlices& fullSlices)
{
    int sum_planned_volume, sum_actual_volume;

    for (auto& [year, tmMap] : uniqueSlices)
    {
        for (auto& [higher_tm, sliceList] : tmMap)
        {
            // Найдём соответствующий список срезов в fullSlices
            auto itYear = fullSlices.find(year);
            if (itYear == fullSlices.end()) continue;

            auto itTm = itYear->second.find(higher_tm);
            if (itTm == itYear->second.end()) continue;

            const auto& fullSliceList = itTm->second;

            for (size_t i = 0; i < sliceList.size(); ++i)
            {
                auto& slice = sliceList[i];
                if (i >= fullSliceList.size()) continue;

                const auto& fullSlice = fullSliceList[i]; // тот же индекс среза

                for (auto& uniqueFrame : slice)
                {
                    sum_planned_volume = 0;
                    sum_actual_volume = 0;
                    boost::gregorian::date max_date(1999, 1, 1);

                    int t_material_id = uniqueFrame.t_material_id;

                    for (const auto& frame : fullSlice)
                    {
                        if (frame.t_material_id != t_material_id)
                            continue;

                        sum_planned_volume += frame.planned_volume;
                        sum_actual_volume += frame.actual_volume;

                        if (frame.calendar_day > max_date)
                            max_date = frame.calendar_day;

                        uniqueFrame.is_completed = ((sum_planned_volume * 0.8f) <= sum_actual_volume && sum_planned_volume != 0);

                        if (uniqueFrame.is_completed) break;
                    }

                    if (!uniqueFrame.is_completed)
                        uniqueFrame.actual_date = std::nullopt;
                    else
                        uniqueFrame.actual_date = max_date;
                }
            }
        }
    }
}

