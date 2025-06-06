#include "SapDataDistinct.hpp"
#include <iostream>

int SapDataDistinct::Size()
{
	return data.size();
}

void SapDataDistinct::Print()
{
    for (const auto& row : data)
    {
        std::cout << "culture_id: " << row.culture_id << std::endl;
        std::cout << "region_id: " << row.region_id << std::endl;
        std::cout << "t_material_id: " << row.t_material_id << std::endl;
        std::cout << "pu_id: " << row.pu_id << std::endl;
        std::cout << "higher_tm: " << row.higher_tm << std::endl;
        std::cout << "season: " << row.season << std::endl;
        std::cout << "year: " << row.year << std::endl;

        std::cout << "\n\n";
    }
}

SapDataDistinct::SapDataDistinct(const pqxx::result& rows)
{
    auto optInt = [](const pqxx::row& r, const char* col) -> std::optional<int>
        {
            return r[col].is_null() ? std::nullopt : std::make_optional(r[col].as<int>());
        };

    for (const auto& row : rows)
    {
        SapDataDistinctFrame frame;
        frame.culture_id = row["culture_id"].as<int>();
        frame.region_id = row["region_id"].as<int>();
        frame.t_material_id = row["t_material_id"].as<int>();
        frame.pu_id = row["pu_id"].as<int>();
        frame.higher_tm = row["higher_tm"].as<std::string>();
        frame.season = row["season"].as<std::string>();
        frame.year = row["year"].as<int>();

        data.push_back(std::move(frame));
    }
}



