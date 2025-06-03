#include <iostream>
#include <Windows.h>
#include "db.hpp"
#include "InitialData.hpp"
#include "CalcMinimalPlannedDate.hpp"

int main()
{
    SetConsoleOutputCP(65001); // Включить UTF-8 в консоли Windows

    try
    {
        Database db("config/db_config.yml");
        db.connect();

        // Выгрузка данных
        pqxx::result raw_data = db.FetchInitialDataRaw();
        InitialData InitData(raw_data);

        std::cout << "Загружено записей: " << InitData.Size() << std::endl;

        CalcMinimalPlannedDate(InitData);

        InitData.Print();
        
    }
    catch (const std::exception& e)
    {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
