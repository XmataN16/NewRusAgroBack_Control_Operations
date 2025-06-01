#pragma once

#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <optional>

struct InitialDataFrame 
{
    int id;
    std::string culture;
    std::string t_material;
    std::string season;
    std::string operation;
    std::optional<int> input_operation;
    std::optional<int> deadline_input;
    std::string region;
    std::optional<boost::gregorian::date> region_date;
    std::optional<int> noinput_deadline;
    std::optional<int> alternative_input;
    std::optional<int> alternative_complete;
    int order;
    int year;
};