#include "SapDataAggregated.hpp"
#include "utilsOptional.hpp"

void SapDataAggregated::AddFrame(const SapDataAggregatedFrame& frame)
{
    frames.push_back(frame);
}

void SapDataAggregated::Print() const
{
    for (const auto& frame : this->frames)
    {
        std::cout << "culture: " << frame.culture << std::endl;
        std::cout << "nzp_zp: " << frame.nzp_zp << std::endl;
        std::cout << "business_dir: " << frame.business_dir << std::endl;
        std::cout << "higher_tm: " << frame.higher_tm << std::endl;
        std::cout << "material_order: " << frame.material_order << std::endl;
        std::cout << "pu: " << frame.pu << std::endl;
        std::cout << "t_material: " << frame.t_material << std::endl;
        std::cout << "year: " << frame.year << "\n\n";

        std::cout << "order: " << frame.order << std::endl;
        std::cout << "is_completed: " << frame.is_completed << std::endl;
        std::cout << "minimal_planned_date: " << frame.minimal_planned_date << std::endl;
        std::cout << "actual_data: " << frame.actual_data << std::endl;
        std::cout << "actual_input_data: " << frame.actual_input_data << std::endl;
        std::cout << "actual_alternative_data: " << frame.actual_alternative_data << std::endl;
        std::cout << "ten_percent: " << frame.ten_percent << std::endl;
        std::cout << "minimal_date: " << frame.minimal_date << std::endl;
        std::cout << "sawing_date: " << frame.sawing_date << std::endl;
        std::cout << "status: " << frame.status << std::endl;
        std::cout << "is_actual: " << frame.is_actual << std::endl;

        std::cout << "\n\n";
    }
}

int SapDataAggregated::Size()
{
    return frames.size();
}