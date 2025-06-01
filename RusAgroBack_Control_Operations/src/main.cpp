#include <iostream>
#include <Windows.h>
#include "db.hpp"
#include "InitialData.hpp"

int main() 
{
    SetConsoleOutputCP(65001); // UTF-8 поддержка

    try 
    {
        Database db;
        std::future<void> connect_future = db.connect();

        std::cout << "Connecting to the database asynchronously...\n";
        connect_future.wait();

        if (db.is_connected()) 
        {
            std::cout << "Successfully connected to the database!\n";

            // Загрузка данных в InitialData
            std::future<InitialData> data_future = db.LoadInitialData();

            std::cout << "Loading data from the database...\n";
            InitialData initial_data = data_future.get(); // Получаем объект

            std::cout << "Loaded " << initial_data.frames.size() << " records.\n";
            initial_data.Print(); // Вызов метода вывода
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