#include "SapData.hpp"

void SapData::AddFrame(const SapDataFrame& frame)
{
    frames.push_back(frame);
}

void SapData::Print() const
{
    for (const auto& frame : this->frames)
    {
        std::cout << "ID: " << frame.id << std::endl;
        std::cout << "culture: " << frame.culture << std::endl;
        std::cout << "nzp_zp: " << frame.nzp_zp << std::endl;
        std::cout << "business_dir: " << frame.business_dir << std::endl;
        std::cout << "calendar_day: " << frame.calendar_day << std::endl;
        std::cout << "higher_tm: " << frame.higher_tm << std::endl;
        std::cout << "material_order: " << frame.material_order << std::endl;
        std::cout << "planned_volume: " << frame.planned_volume << std::endl;
        std::cout << "actual_volume: " << frame.actual_volume << std::endl;
        std::cout << "pu: " << frame.pu << std::endl;
        std::cout << "t_material: " << frame.t_material << std::endl;
        std::cout << "year: " << frame.year << std::endl;

        std::cout << "\n\n";
    }
}

int SapData::Size()
{
    return frames.size();
}