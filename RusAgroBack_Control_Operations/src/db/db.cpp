#include "db.hpp"

// Теперь подключаем полный заголовок libpqxx, так как он нужен только в реализации
#include <pqxx/pqxx>
#include <iostream>

Database::Database() = default;

Database::~Database() 
{
    disconnect();
}

std::future<void> Database::connect(const std::string& host, int port, const std::string& dbname, const std::string& user, const std::string& password) 
{
    return std::async(std::launch::async, [this, host, port, dbname, user, password]() 
        {
        try 
        {
            // Формируем строку подключения
            std::string conn_str = "host=" + host + " port=" + std::to_string(port) + " dbname=" + dbname + " user=" + user + " password=" + password;

            // Создаем новое соединение
            conn_ = std::make_unique<pqxx::connection>(conn_str);

            if (conn_->is_open()) 
            {
                std::cout << "Connection to database established successfully.\n";
            }
            else 
            {
                throw std::runtime_error("Failed to connect to the database.");
            }
        }
        catch (const std::exception& ex) 
        {
            std::cerr << "Exception during connection: " << ex.what() << "\n";
            throw; // Перебрасываем исключение в future
        }
        });
}

bool Database::is_connected() const 
{
    return conn_ && conn_->is_open();
}

void Database::disconnect() 
{
    if (conn_ && conn_->is_open()) 
    {
        conn_->close();
    }
}

std::future<std::vector<InitialDataFrame>> Database::LoadInitialData() 
{
    return std::async(std::launch::async, [this]() 
        {
        std::vector<InitialDataFrame> result;

        if (!conn_ || !conn_->is_open()) 
        {
            throw std::runtime_error("Database connection is not open.");
        }

        try 
        {
            pqxx::work txn(*conn_);
            pqxx::result res = txn.exec(
                "SELECT id, culture, t_material, season, operation, "
                "input_operation, deadline_input, region, "
                "TO_CHAR(region_date, 'YYYY-MM-DD') AS region_date, "
                "noinput_deadline, alternative_input, alternative_complete, ""\"order\", year "
                "FROM static_initial_data"
            );

            for (const auto& row : res) 
            {
                InitialDataFrame data;

                row["id"].to(data.id);
                row["culture"].to(data.culture);
                row["t_material"].to(data.t_material);
                row["season"].to(data.season);
                row["operation"].to(data.operation);

                // Обработка optional<int>
                if (row["input_operation"].is_null()) 
                {
                    data.input_operation = std::nullopt;
                }
                else 
                {
                    int value;
                    row["input_operation"].to(value);
                    data.input_operation = value;
                }

                if (row["deadline_input"].is_null()) 
                {
                    data.deadline_input = std::nullopt;
                }
                else 
                {
                    int value;
                    row["deadline_input"].to(value);
                    data.deadline_input = value;
                }

                row["region"].to(data.region);

                // Обработка optional<boost::gregorian::date>
                if (row["region_date"].is_null()) 
                {
                    data.region_date = std::nullopt;
                }
                else 
                {
                    std::string date_str;
                    row["region_date"].to(date_str);
                    data.region_date = parse_date(date_str);
                }

                if (row["noinput_deadline"].is_null()) 
                {
                    data.noinput_deadline = std::nullopt;
                }
                else 
                {
                    int value;
                    row["noinput_deadline"].to(value);
                    data.noinput_deadline = value;
                }

                if (row["alternative_input"].is_null()) 
                {
                    data.alternative_input = std::nullopt;
                }
                else 
                {
                    int value;
                    row["alternative_input"].to(value);
                    data.alternative_input = value;
                }

                if (row["alternative_complete"].is_null()) 
                {
                    data.alternative_complete = std::nullopt;
                }
                else 
                {
                    int value;
                    row["alternative_complete"].to(value);
                    data.alternative_complete = value;
                }

                row["order"].to(data.order);
                row["year"].to(data.year);

                result.push_back(data);
            }
        }
        catch (const std::exception& ex) 
        {
            std::cerr << "Error fetching data: " << ex.what() << "\n";
            throw;
        }

        return result;
        });
}