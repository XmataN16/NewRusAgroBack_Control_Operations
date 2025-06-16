#include "SapControlTech.hpp"
#include "db.hpp"
#include "InitialData.hpp"
#include "SapData.hpp"
#include "CalcMinimalPlannedDate.hpp"

int calcSapControlAggregated()
{
    try
    {
        Database db("config/db_config.yml");
        db.connect();

        // Выгрузка данных
        pqxx::result rawInitData = db.fetchInitialDataRaw();
        InitialData initialData(rawInitData);

        pqxx::result rawSapDataAll = db.fetchSapControlOperationsRaw();
        SapData sapDataAll(rawSapDataAll);

        pqxx::result rawIDSReseeding = db.fetchSapIDSReseedingRaw();
        IDSReseeding idsReseeding(rawIDSReseeding);

        calcMinimalPlannedDate(initialData);

        // Разделение данных
        YearSlices sapDataSlices = sliceSapData(sapDataAll.data, idsReseeding);

        // Создаем структуру в которой каждый срез содержит только записи с уникальными t_material на основе первого вхождения (уже sort по calendar_day)
        YearSlices sapDataUniqueTMaterialSlices = makeUniqueTMaterialSlices(sapDataSlices);

        printSlicesForYearAndTm(sapDataUniqueTMaterialSlices, 2024, "BL-04-03-16-0007");

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
