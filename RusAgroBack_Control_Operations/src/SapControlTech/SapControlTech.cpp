#include "SapControlTech.hpp"
#include <omp.h>

void CalcControlTechOperAggregated()
{
    int num_threads = omp_get_max_threads(); // Узнаем максимальное число потоков
    omp_set_num_threads(num_threads); // Устанавливаем число потоков

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
            std::future<InitialData> InitialData_future = db.LoadInitialData();

            std::cout << "Loading InitialData from the database...\n";
            InitialData InitData = InitialData_future.get(); // Получаем объект

            // Загрузка данных в SapData
            std::future<SapData> SapData_future = db.LoadSapData();

            std::cout << "Loading SapData from the database...\n";
            SapData SapDataAll = SapData_future.get(); // Получаем объект

            // Загрузка данных в SapDataAggregated
            std::future<SapDataAggregated> SapDataAggregated_future = db.LoadSapDataAggregated();

            std::cout << "Loading SapDataAggregated from the database...\n";
            SapDataAggregated SapDataAggregatedAll = SapDataAggregated_future.get(); // Получаем объект

            CalcMinimalPlannedDate(InitData);

            std::cout << "Loaded " << SapDataAggregatedAll.Size() << " records.\n";
            SapDataAggregatedAll.Print(); // Вызов метода вывода
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
}