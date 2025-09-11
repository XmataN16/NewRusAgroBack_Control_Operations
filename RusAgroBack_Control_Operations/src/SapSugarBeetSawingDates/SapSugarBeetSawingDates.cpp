#include "SapControlTech.hpp"
#include "db.hpp"
#include "InitialData.hpp"
#include "SapData.hpp"
#include "CalcActualDate.hpp"
#include "CalcSawingDate.hpp"
#include "CalcResawingDate.hpp"
#include <chrono>

int calcSapSugarBeetSawingDates()
{
    try
    {
        Database db("config/db_config.yml");
        db.connect();

        // Выгрузка данных из таблицы sap_sugar_beet_sawing_dates_ops_source
        pqxx::result rawSapDataAll = db.fetchSapSugarBeetSawingDatesOpsRaw();
        SapData sapDataAll(rawSapDataAll);

        // Получаем id операций с пересевами
        pqxx::result rawIDSReseeding = db.fetchSapIDSReseedingRaw();
        IDSReseeding idsReseeding(rawIDSReseeding);

        // Получаем id операций с посевами
        pqxx::result rawIDSSeeding = db.fetchSapIDSSeedingRaw();
        IDSSeeding idsSeeding(rawIDSSeeding);

        clock_t calc_start = clock();

        // Разделение данных
        YearSlices sapDataSlices = sliceSapData(sapDataAll.data, idsReseeding);

        // Создаем структуру в которой каждый срез содержит только записи с уникальными t_material на основе первого вхождения (уже sort по calendar_day)
        YearSlices sapDataUniqueTMaterialSlices = makeUniqueTMaterialSlices(sapDataSlices);

        calcActualDate(sapDataUniqueTMaterialSlices, sapDataSlices);

        calcSawingDate(sapDataUniqueTMaterialSlices, idsSeeding);

        clock_t calc_end = clock();

        double calc_seconds = (double)(calc_end - calc_start) / CLOCKS_PER_SEC;

        std::cout << "Calc time: " << calc_seconds << " seconds" << std::endl;

        clock_t uploading_start = clock();

        db.truncateAndRestartIdentity("sap_sugar_beet_sawing_dates");

        db.insertIntoSapSugarBeetSawingDates(sapDataUniqueTMaterialSlices);

        clock_t uploading_end = clock();

        double uploading_seconds = (double)(uploading_end - uploading_start) / CLOCKS_PER_SEC;

        std::cout << "Unloading data into PostgreSQL: " << uploading_seconds << " seconds" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << u8"Ошибка: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
