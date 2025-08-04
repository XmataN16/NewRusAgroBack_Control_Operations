#include "SapControlTech.hpp"
#include "db.hpp"
#include "InitialData.hpp"
#include "SapData.hpp"
#include "CalcMinimalPlannedDate.hpp"
#include "CalcActualDate.hpp"
#include "CalcActualDateCompleteEntryOperation.hpp"
#include "CalcSawingDate.hpp"
#include "CalcResawingDate.hpp"
#include "CalcMinimalDate.hpp"
#include <chrono>

int calcSapControlAggregated()
{
    try
    {
        Database db("config/db_config.yml");
        db.connect();

        // �������� ������ �� ������� static_initial_data
        pqxx::result rawInitData = db.fetchInitialDataRaw();
        InitialData initialData(rawInitData);

        // �������� ������ �� ������� sap_control_operations
        pqxx::result rawSapDataAll = db.fetchSapControlOperationsRaw();
        SapData sapDataAll(rawSapDataAll);

        // �������� id �������� � ����������
        pqxx::result rawIDSReseeding = db.fetchSapIDSReseedingRaw();
        IDSReseeding idsReseeding(rawIDSReseeding);

        // �������� id �������� � ��������
        pqxx::result rawIDSSeeding = db.fetchSapIDSSeedingRaw();
        IDSSeeding idsSeeding(rawIDSSeeding);

        clock_t calc_start = clock();

        calcMinimalPlannedDate(initialData);

        // ���������� ������
        YearSlices sapDataSlices = sliceSapData(sapDataAll.data, idsReseeding);

        // ������� ��������� � ������� ������ ���� �������� ������ ������ � ����������� t_material �� ������ ������� ��������� (��� sort �� calendar_day)
        YearSlices sapDataUniqueTMaterialSlices = makeUniqueTMaterialSlices(sapDataSlices);

        calcActualDate(sapDataUniqueTMaterialSlices, sapDataSlices);

        calcInputDate(sapDataUniqueTMaterialSlices, initialData);

        calcAlternativeDate(sapDataUniqueTMaterialSlices, initialData);

        calcSawingDate(sapDataUniqueTMaterialSlices, idsSeeding);

        calcResawingDate(sapDataUniqueTMaterialSlices, idsReseeding);

        calcMinimalDate(sapDataUniqueTMaterialSlices, initialData);

        clock_t calc_end = clock();

        double calc_seconds = (double)(calc_end - calc_start) / CLOCKS_PER_SEC;

        std::cout << "Calc time: " << calc_seconds << " seconds" << std::endl;

        clock_t uploading_start = clock();

        db.truncateAndRestartIdentity("control_operations_aggregated_new");

        db.insertIntoControlOperationsAggregated(sapDataUniqueTMaterialSlices);

        clock_t uploading_end = clock();

        double uploading_seconds = (double)(uploading_end - uploading_start) / CLOCKS_PER_SEC;

        std::cout << "Unloading data into PostgreSQL: " << uploading_seconds << " seconds" << std::endl;

        //printSlicesForYearAndTm(sapDataUniqueTMaterialSlices, 2024, "BL-04-01-01-0004");
    }
    catch (const std::exception& e)
    {
        std::cerr << u8"������: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
