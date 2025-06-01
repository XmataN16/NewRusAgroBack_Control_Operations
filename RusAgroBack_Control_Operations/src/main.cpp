#include <iostream>
#include <Windows.h>
#include "db.hpp"  // Убедитесь, что путь к заголовкам указан верно
#include "utilsBoostDate.hpp"

int main() 
{
    SetConsoleOutputCP(65001);  // Поддержка UTF-8 в консоли Windows

    try 
    {
        Database db;
        std::future<void> connect_future = db.connect();  // Асинхронное подключение

        std::cout << "Connecting to the database asynchronously...\n";
        connect_future.wait();  // Ждем завершения подключения

        if (db.is_connected()) 
        {
            std::cout << "Successfully connected to the database!\n";

            // Вызов асинхронного метода загрузки данных
            std::future<std::vector<InitialDataFrame>> data_future = db.LoadInitialData();

            std::cout << "Loading data from the database...\n";
            // Ждем завершения загрузки данных
            std::vector<InitialDataFrame> data = data_future.get();

            std::cout << "Loaded " << data.size() << " records:\n";
            for (const auto& item : data) 
            {
                std::cout << "ID: " << item.id
                    << ", Culture: " << item.culture
                    << ", Region: " << item.region
                    << ", Date: " << (item.region_date ? boost::gregorian::to_simple_string(*item.region_date) : "NULL")
                    << ", Year: " << item.year << "\n";
            }
        }
        else 
        {
            std::cerr << "Failed to connect to the database.\n";
        }
    }
    catch (const std::exception& ex) 
    {
        std::cerr << "Error: " << ex.what() << "\n";
    }

    return 0;
}