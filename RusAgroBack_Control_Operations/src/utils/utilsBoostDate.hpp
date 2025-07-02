#pragma once
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>

boost::gregorian::date parse_date(const std::string& date_str);

std::ostream& operator<<(std::ostream& os, const boost::gregorian::date& d);

template<typename... Dates>
std::optional<boost::gregorian::date> max_date(const std::optional<boost::gregorian::date>& first, const Dates&... rest)
{
    std::vector<boost::gregorian::date> valid_dates;

    // Лямбда для добавления валидных дат в вектор
    auto add_date = [&](const std::optional<boost::gregorian::date>& d)
        {
            if (d) valid_dates.push_back(*d);
        };

    // Обрабатываем первый аргумент
    add_date(first);

    // Распаковываем остальные аргументы
    (add_date(rest), ...);

    // Если все даты были std::nullopt, возвращаем std::nullopt
    if (valid_dates.empty())
    {
        return std::nullopt;
    }

    return *std::max_element(valid_dates.begin(), valid_dates.end());
}

template<typename... Dates>
std::optional<boost::gregorian::date> min_date(const std::optional<boost::gregorian::date>& first, const Dates&... rest)
{
    std::vector<boost::gregorian::date> valid_dates;

    // Лямбда для добавления валидных дат в вектор
    auto add_date = [&](const std::optional<boost::gregorian::date>& d)
        {
            if (d) valid_dates.push_back(*d);
        };

    // Обрабатываем первый аргумент
    add_date(first);

    // Распаковываем остальные аргументы
    (add_date(rest), ...);

    // Если все даты были std::nullopt, возвращаем std::nullopt
    if (valid_dates.empty())
    {
        return std::nullopt;
    }

    return *std::min_element(valid_dates.begin(), valid_dates.end());
}

