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