#include "SapControlTech.hpp"
#include "db.hpp"
#include "InitialData.hpp"
#include "SapData.hpp"
#include "CalcMinimalPlannedDate.hpp"
#include "CalcActualDate.hpp"
#include "CalcActualDateCompleteEntryOperation.hpp"

int calcSapControlAggregated()
{
    try
    {
        Database db("config/db_config.yml");
        db.connect();

        // �������� ������
        pqxx::result rawInitData = db.fetchInitialDataRaw();
        InitialData initialData(rawInitData);

        pqxx::result rawSapDataAll = db.fetchSapControlOperationsRaw();
        SapData sapDataAll(rawSapDataAll);

        pqxx::result rawIDSReseeding = db.fetchSapIDSReseedingRaw();
        IDSReseeding idsReseeding(rawIDSReseeding);

        calcMinimalPlannedDate(initialData);

        // ���������� ������
        YearSlices sapDataSlices = sliceSapData(sapDataAll.data, idsReseeding);

        // ������� ��������� � ������� ������ ���� �������� ������ ������ � ����������� t_material �� ������ ������� ��������� (��� sort �� calendar_day)
        YearSlices sapDataUniqueTMaterialSlices = makeUniqueTMaterialSlices(sapDataSlices);

        CalcActualDate(sapDataUniqueTMaterialSlices, sapDataSlices);

        calcInputDate(sapDataUniqueTMaterialSlices, initialData);

        calcAlternativeDate(sapDataUniqueTMaterialSlices, initialData);

        printSlicesForYearAndTm(sapDataUniqueTMaterialSlices, 2024, "PR-01-02-05-0057");

        //slices.at(2024).at("BL-04-03-16-0007").at()

        //InitData.Print();

    }
    catch (const std::exception& e)
    {
        std::cerr << u8"������: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
