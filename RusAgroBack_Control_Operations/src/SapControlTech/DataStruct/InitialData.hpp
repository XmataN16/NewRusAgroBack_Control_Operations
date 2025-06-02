#pragma once
#include <string>
#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <optional>

struct PlannedDates
{
    std::optional<boost::gregorian::date> planned_date;
    std::optional<boost::gregorian::date> input_date;
    std::optional<boost::gregorian::date> alternative_date;
    std::optional<boost::gregorian::date> minimal_planned_date;
};

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

    PlannedDates planned_dates;
};

struct InitialData 
{
    std::vector<InitialDataFrame> frames;

    // Метод для добавления нового элемента
    void AddFrame(const InitialDataFrame& frame);

    // Метод вывода данных
    void Print() const;

    int Size();
};