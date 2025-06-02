#include "InitialData.hpp"
#include "utilsOptional.hpp"
#include <iostream>
#include <iomanip>
#include <boost/date_time/gregorian/gregorian.hpp>

void InitialData::AddFrame(const InitialDataFrame& frame) 
{
    frames.push_back(frame);
}

void InitialData::Print() const 
{
    for (const auto& frame : this->frames)
    {
        if (frame.culture == u8"ίχμενό" and frame.year == 2024)
        {
            std::cout << "ID: " << frame.id << std::endl;
            std::cout << "culture: " << frame.culture << std::endl;
            std::cout << "t_material: " << frame.t_material << std::endl;
            std::cout << "season: " << frame.season << std::endl;
            std::cout << "operation: " << frame.operation << std::endl;
            std::cout << "input_operation: " << frame.input_operation << std::endl;
            std::cout << "deadline_input: " << frame.deadline_input << std::endl;
            std::cout << "region: " << frame.region << std::endl;
            std::cout << "region_date: " << frame.region_date << std::endl;
            std::cout << "noinput_deadline: " << frame.noinput_deadline << std::endl;
            std::cout << "alternative_input: " << frame.alternative_input << std::endl;
            std::cout << "alternative_complete: " << frame.alternative_complete << std::endl;
            std::cout << "order: " << frame.order << std::endl;
            std::cout << "year: " << frame.year << "\n\n";

            std::cout << "planned_date: " << frame.planned_dates.planned_date << std::endl;
            std::cout << "input_date: " << frame.planned_dates.input_date << std::endl;
            std::cout << "alternative_date: " << frame.planned_dates.alternative_date << std::endl;
            std::cout << "minimal_planned_date: " << frame.planned_dates.minimal_planned_date << std::endl;

            std::cout << "\n\n";
        }
    }
}

int InitialData::Size()
{
    return frames.size();
}
