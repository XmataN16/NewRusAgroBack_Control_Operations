#include <data_struct/InitialData.hpp>
#include <utils/utilsBoostDate.hpp>
#include <utils/utilsOptional.hpp>
#include <stdexcept>

int InitialData::Size()
{
    return data.size();
}

void InitialData::Print()
{
    for (const auto& row : data)
    {
        //if (row.culture_id == 12 && row.region_id == 7 && row.year == 2024)
        //{
            std::cout << "culture_id: " << row.culture_id << std::endl;
            std::cout << "t_material_id: " << row.t_material_id << std::endl;
            std::cout << "region_id: " << row.region_id << std::endl;
            std::cout << "season: " << row.season << std::endl;
            std::cout << "region_date: " << row.region_date << std::endl;
            std::cout << "input_operation_order: " << row.input_operation_order << std::endl;
            std::cout << "alternative_operation_order: " << row.alternative_operation_order << std::endl;
            std::cout << "input_deadline: " << row.input_deadline << std::endl;
            std::cout << "alternative_deadline: " << row.alternative_deadline << std::endl;
            std::cout << "noinput_deadline: " << row.noinput_deadline << std::endl;
            std::cout << "order: " << row.order << std::endl;
            std::cout << "year: " << row.year << std::endl;

            std::cout << "\n";

            std::cout << "planned_date: " << row.planned_dates.planned_date << std::endl;
            std::cout << "input_date: " << row.planned_dates.input_date << std::endl;
            std::cout << "alternative_date: " << row.planned_dates.alternative_date << std::endl;
            std::cout << "minimal_planned_date: " << row.planned_dates.minimal_planned_date << std::endl;

            std::cout << "\n\n";
        //}
    }
}

InitialData::InitialData(const pqxx::result& rows)
{
    auto optInt = [](const pqxx::row& r, const char* col) -> std::optional<int>
        {
            return r[col].is_null() ? std::nullopt : std::make_optional(r[col].as<int>());
        };

    // 1) —начала считываем все строки в vector<InitialDataFrame>
    for (const auto& row : rows)
    {
        InitialDataFrame frame;
        frame.culture_id = row["culture_id"].as<int>();
        frame.t_material_id = row["t_material_id"].as<int>();
        frame.region_id = row["region_id"].as<int>();
        frame.season = row["season"].as<std::string>();

        frame.region_date = row["region_date"].is_null()
            ? std::nullopt
            : std::make_optional(boost::gregorian::from_simple_string(row["region_date"].as<std::string>()));

        frame.input_operation_order = optInt(row, "input_operation_order");
        frame.alternative_operation_order = optInt(row, "alternative_operation_order");
        frame.input_deadline = optInt(row, "input_deadline");
        frame.alternative_deadline = optInt(row, "alternative_deadline");
        frame.noinput_deadline = optInt(row, "noinput_deadline");
        frame.order = row["order"].as<int>();
        frame.year = row["year"].as<int>();

        data.push_back(std::move(frame));
    }

    order_index_map.reserve(data.size());
    for (int i = 0; i < static_cast<int>(data.size()); ++i)
    {
        const InitialDataFrame& frame = data[i];

        KeyOrder4 key
        {
            frame.culture_id,
            frame.region_id,
            frame.order,
            frame.year
        };

        if (order_index_map.find(key) != order_index_map.end())
        {
            throw std::runtime_error(
                "Duplicate key detected: culture=" + std::to_string(frame.culture_id)
                + ", region=" + std::to_string(frame.region_id)
                + ", order=" + std::to_string(frame.order)
                + ", year=" + std::to_string(frame.year)
            );
        }

        order_index_map.emplace(std::move(key), i);

        KeyCRTYS5 fullKey
        {
           frame.culture_id,
           frame.region_id,
           frame.t_material_id,
           frame.year,
           frame.season
        };

        if (CRTYS_index_map.find(fullKey) != CRTYS_index_map.end())
        {
            throw std::runtime_error("Duplicate full key detected: culture=" + std::to_string(fullKey.culture_id) +
                ", region=" + std::to_string(fullKey.region_id) +
                ", t_material_id=" + std::to_string(fullKey.t_material_id) +
                ", year=" + std::to_string(fullKey.year))
;
        }

        CRTYS_index_map.emplace(std::move(fullKey), i);
    }
}