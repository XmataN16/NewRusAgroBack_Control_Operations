#include "CalcResawingDate.hpp"
#include <unordered_set>

void calcResawingDate(YearSlices& uniqueSlices, IDSReseeding idsReseeding)
{
    std::unordered_set<int> reseeding_ids(idsReseeding.IDs.begin(), idsReseeding.IDs.end());

    for (auto& [year, tmMap] : uniqueSlices)
    {
        for (auto& [higher_tm, sliceList] : tmMap)
        {
            std::optional<boost::gregorian::date> lastZPDate;
            std::optional<boost::gregorian::date> lastNZPDate;

            // Ищем последние пересевы по всем срезам отдельно для ZP и NZP
            for (const auto& slice : sliceList)
            {
                for (auto it = slice.rbegin(); it != slice.rend(); ++it)
                {
                    if (reseeding_ids.count(it->t_material_id))
                    {
                        if (it->season == "ZP")
                        {
                            if (!lastZPDate || it->actual_date > lastZPDate)
                                lastZPDate = it->actual_date;
                        }
                        else if (it->season == "NZP")
                        {
                            if (!lastNZPDate || it->actual_date > lastNZPDate)
                                lastNZPDate = it->actual_date;
                        }
                    }
                }
            }

            // Проставляем даты во все записи
            for (auto& slice : sliceList)
            {
                for (auto& frame : slice)
                {
                    if (frame.season == "ZP")
                        frame.resawing_date = lastZPDate;
                    else if (frame.season == "NZP")
                        frame.resawing_date = lastNZPDate;
                }
            }
        }
    }
}