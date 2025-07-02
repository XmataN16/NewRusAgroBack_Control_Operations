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

        clock_t start = clock();

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

        clock_t end = clock();

        double seconds = (double)(end - start) / CLOCKS_PER_SEC;

        std::cout << "Calc time: " << seconds << " seconds" << std::endl;

        db.truncateAndRestartIdentity("control_operations_aggregated");

        db.insertIntoControlOperationsAggregated(sapDataUniqueTMaterialSlices);

        //printSlicesForYearAndTm(sapDataUniqueTMaterialSlices, 2024, "BL-04-01-01-0004");
    }
    catch (const std::exception& e)
    {
        std::cerr << u8"������: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
