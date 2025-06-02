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
            std::future<InitialData> data_future = db.LoadInitialData();

            std::cout << "Loading data from the database...\n";
            InitialData InitData = data_future.get(); // �������� ������

            CalcMinimalPlannedDate(InitData);

            std::cout << "Loaded " << InitData.Size() << " records.\n";
            InitData.Print(); // ����� ������ ������
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