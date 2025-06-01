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
        std::cout << "year: " << frame.year << std::endl;
        std::cout << "\n\n";
    }
}