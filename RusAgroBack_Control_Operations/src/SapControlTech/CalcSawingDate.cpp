#include "CalcSawingDate.hpp"
#include <unordered_set>

void calcSawingDate(YearSlices& uniqueSlices, IDSSeeding idsSeeding)
{
    std::unordered_set<int> seeding_ids(idsSeeding.IDs.begin(), idsSeeding.IDs.end());

    for (auto& [year, tmMap] : uniqueSlices)
    {
        for (auto& [higher_tm, sliceList] : tmMap)
        {
            // Ищем первую подходящую запись для ZP и NZP во всех срезах
            std::optional<boost::gregorian::date> zp_date;
            std::optional<boost::gregorian::date> nzp_date;

            for (const auto& slice : sliceList)
            {
                for (const auto& frame : slice)
                {
                    if (!zp_date && frame.season == "ZP" && seeding_ids.count(frame.t_material_id))
                    {
                        zp_date = frame.actual_date;
                    }
                    if (!nzp_date && frame.season == "NZP" && seeding_ids.count(frame.t_material_id))
                    {
                        nzp_date = frame.actual_date;
                    }
                    if (zp_date && nzp_date)
                        break;
                }
                if (zp_date && nzp_date)
                    break;
            }

            // Проставляем найденные даты по всем срезам
            for (auto& slice : sliceList)
            {
                for (auto& frame : slice)
                {
                    if (frame.season == "ZP")
                    {
                        frame.sawing_date = zp_date;
                    }
                    else if (frame.season == "NZP")
                    {
                        frame.sawing_date = nzp_date;
                    }
                }
            }
        }
    }
}
