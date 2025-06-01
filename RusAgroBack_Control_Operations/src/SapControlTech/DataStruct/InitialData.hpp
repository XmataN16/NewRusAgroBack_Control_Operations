#pragma once
#include <string>
#include <vector>
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

struct InitialData 
{
    std::vector<InitialDataFrame> frames;

    // Метод для добавления нового элемента
    void AddFrame(const InitialDataFrame& frame);

    // Метод вывода данных
    void Print() const;
};