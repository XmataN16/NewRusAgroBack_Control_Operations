#include "CalcActualDate.hpp"
#include "utilsBoostDate.hpp"
#include "utilsOptional.hpp"

// Для удобства создаём индекс: t_material_id -> list of SapDataFrame (из полной выборки)
using TMaterialIndex = std::unordered_map<int, std::vector<const SapDataFrame*>>;

TMaterialIndex BuildTMaterialIndex(const YearSlices& fullSlices)
{
    TMaterialIndex index;
    for (const auto& [_, tmMap] : fullSlices)
    {
        for (const auto& [_, sliceList] : tmMap)
        {
            for (const auto& slice : sliceList)
            {
                for (const auto& frame : slice)
                {
                    index[frame.t_material_id].push_back(&frame);
                }
            }
        }
    }
    return index;
}

void calcActualDate(YearSlices& uniqueSlices, const YearSlices& fullSlices)
{
    int sumStartPlannedVolume, sumStartActualVolume, sumEndPlannedVolume, sumEndActualVolume, is_completed, is_started;

    for (auto& [year, tmMap] : uniqueSlices)
    {
        for (auto& [higher_tm, sliceList] : tmMap)
        {
            // Найдём соответствующий список срезов в fullSlices
            auto itYear = fullSlices.find(year);
            if (itYear == fullSlices.end()) continue;

            auto itTm = itYear->second.find(higher_tm);
            if (itTm == itYear->second.end()) continue;

            const auto& fullSliceList = itTm->second;

            for (size_t i = 0; i < sliceList.size(); ++i)
            {
                auto& slice = sliceList[i];
                if (i >= fullSliceList.size()) continue;

                const auto& fullSlice = fullSliceList[i]; // тот же индекс среза

                for (auto& uniqueFrame : slice)
                {
                    is_completed = false;
                    is_started = false;

                    sumStartPlannedVolume = 0;
                    sumStartActualVolume = 0;
                    sumEndPlannedVolume = 0;
                    sumEndActualVolume = 0;
                    boost::gregorian::date actualDate(1999, 1, 1);
                    boost::gregorian::date startDate(1999, 1, 1);

                    int t_material_id = uniqueFrame.t_material_id;

                    for (const auto& frame : fullSlice)
                    {
                        if (frame.t_material_id != t_material_id)
                            continue;

                        if (!is_started)
                        {
                            sumStartPlannedVolume += frame.planned_volume;
                            sumStartActualVolume += frame.actual_volume;

                            if (frame.calendar_day > startDate)
                                startDate = frame.calendar_day;

                            is_started = sumStartPlannedVolume * 0.1 <= sumStartActualVolume && sumStartPlannedVolume != 0 ? true : false;
                        }
                        
                        if (!is_completed)
                        {
                            sumEndPlannedVolume += frame.planned_volume;
                            sumEndActualVolume += frame.actual_volume;
                            if (frame.calendar_day > actualDate)
                                actualDate = frame.calendar_day;

                            is_completed = sumEndPlannedVolume * 0.8 <= sumEndActualVolume && sumEndPlannedVolume != 0 ? true : false;
                        }

                        if (is_completed) break;
                    }

                    uniqueFrame.is_completed = is_completed;
                    uniqueFrame.is_started = is_started;

                    if (!is_completed)
                        uniqueFrame.actual_date = std::nullopt;
                    else
                        uniqueFrame.actual_date = actualDate;

                    if (!is_started)
                        uniqueFrame.start_date = std::nullopt;
                    else
                        uniqueFrame.start_date = startDate;
                }
            }
        }
    }
}

