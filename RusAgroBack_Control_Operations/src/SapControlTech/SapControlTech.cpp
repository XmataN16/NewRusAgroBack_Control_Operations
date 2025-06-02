#include "SapControlTech.hpp"
#include <omp.h>

void CalcControlTechOperAggregated()
{
    int num_threads = omp_get_max_threads(); // ������ ������������ ����� �������
    omp_set_num_threads(num_threads); // ������������� ����� �������

    try
    {
        Database db;
        std::future<void> connect_future = db.connect();

        std::cout << "Connecting to the database asynchronously...\n";
        connect_future.wait();

        if (db.is_connected())
        {
            std::cout << "Successfully connected to the database!\n";

            // �������� ������ � InitialData
            std::future<InitialData> InitialData_future = db.LoadInitialData();

            std::cout << "Loading InitialData from the database...\n";
            InitialData InitData = InitialData_future.get(); // �������� ������

            // �������� ������ � SapData
            std::future<SapData> SapData_future = db.LoadSapData();

            std::cout << "Loading SapData from the database...\n";
            SapData SapDataAll = SapData_future.get(); // �������� ������

            // �������� ������ � SapDataAggregated
            std::future<SapDataAggregated> SapDataAggregated_future = db.LoadSapDataAggregated();

            std::cout << "Loading SapDataAggregated from the database...\n";
            SapDataAggregated SapDataAggregatedAll = SapDataAggregated_future.get(); // �������� ������

            CalcMinimalPlannedDate(InitData);

            std::cout << "Loaded " << SapDataAggregatedAll.Size() << " records.\n";
            SapDataAggregatedAll.Print(); // ����� ������ ������
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