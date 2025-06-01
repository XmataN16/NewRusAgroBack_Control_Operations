#include <iostream>
#include "db.hpp"

int main() 
{
    Database db;
    std::future<void> connect_future = db.connect(); // Используются значения по умолчанию

    std::cout << "Connecting to the database asynchronously...\n";

    try 
    {
        connect_future.wait(); // Ждём завершения асинхронного подключения

        if (db.is_connected()) 
        {
            std::cout << "Successfully connected to the database!\n";
        }
        else 
        {
            std::cerr << "Failed to connect to the database.\n";
        }
    }
    catch (const std::exception& ex) 
    {
        std::cerr << "Exception during connection: " << ex.what() << "\n";
    }

    return 0;
}