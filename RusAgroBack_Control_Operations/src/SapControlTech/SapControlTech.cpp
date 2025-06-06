#include "SapControlTech.hpp"
#include "db.hpp"
#include "InitialData.hpp"
#include "SapData.hpp"
#include "SapDataDistinct.hpp"
#include "CalcMinimalPlannedDate.hpp"

int CalcSapControlAggregated()
{
    try
    {
        Database db("config/db_config.yml");
        db.connect();

        // �������� ������
        pqxx::result rawInitData = db.FetchInitialDataRaw();
        InitialData InitData(rawInitData);

        pqxx::result rawSapData = db.FetchSapControlOperationsRaw();
        SapData SapDataAll(rawSapData);

        pqxx::result rawSapDataAggregated = db.FetchSapControlOperationsDistinctRaw();
        SapDataDistinct SapDataAggregated(rawSapDataAggregated);

        std::cout << "��������� �������: " << InitData.Size() << std::endl;

        CalcMinimalPlannedDate(InitData);




        InitData.Print();

    }
    catch (const std::exception& e)
    {
        std::cerr << "������: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
