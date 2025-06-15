#include "SapControlTech.hpp"
#include "db.hpp"
#include "InitialData.hpp"
#include "SapData.hpp"
#include "CalcMinimalPlannedDate.hpp"

int CalcSapControlAggregated()
{
    try
    {
        Database db("config/db_config.yml");
        db.connect();

        // Выгрузка данных
        pqxx::result rawInitData = db.FetchInitialDataRaw();
        InitialData initialData(rawInitData);

        pqxx::result rawSapDataAll = db.FetchSapControlOperationsRaw();
        SapData sapDataAll(rawSapDataAll);

        pqxx::result rawIDSReseeding = db.FetchSapIDSReseedingRaw();
        IDSReseeding idsReseeding(rawIDSReseeding);

        CalcMinimalPlannedDate(initialData);

        // Разделение данных
        YearSlices sapDataSlices = SliceSapData(sapDataAll.data, idsReseeding);

        PrintSlicesForYearAndTm(sapDataSlices, 2024, "BL-04-03-16-0007");

        //slices.at(2024).at("BL-04-03-16-0007").at()

        //InitData.Print();

    }
    catch (const std::exception& e)
    {
        std::cerr << u8"Ошибка: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
