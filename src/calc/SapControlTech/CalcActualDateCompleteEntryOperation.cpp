#include <calc/SapControlTech/CalcActualDateCompleteEntryOperation.hpp>
#include <utils/utilsBoostDate.hpp>
#include <utils/utilsOptional.hpp>

void calcInputDate(YearSlices& uniqueSlices, const InitialData& initData)
{
    for (auto& [year, higherTmMap] : uniqueSlices)
    {
        for (auto& [higher_tm, sliceList] : higherTmMap)
        {
            for (auto& slice : sliceList)
            {
                for (auto& frame : slice)
                {
                    KeyCRTYS5 key {
                        frame.culture_id,
                        frame.region_id,
                        frame.t_material_id,
                        frame.year,
                        frame.season
                    };

                    auto it = initData.CRTYS_index_map.find(key);
                    if (it == initData.CRTYS_index_map.end())
                    {
                        frame.actual_input_date = std::nullopt;
                        continue;
                    }

                    const InitialDataFrame& matchInitData = initData.data[it->second];

                    // Проверка наличия input_operation_order
                    if (!matchInitData.input_operation_order.has_value())
                    {
                        frame.actual_input_date = std::nullopt;
                        continue;
                    }

                    // Ищем родительскую запись по input_operation_order
                    KeyOrder4 parentKey 
                    {
                        matchInitData.culture_id,
                        matchInitData.region_id,
                        matchInitData.input_operation_order.value(),
                        matchInitData.year
                    };

                    auto parentIt = initData.order_index_map.find(parentKey);
                    if (parentIt == initData.order_index_map.end())
                    {
                        frame.actual_input_date = std::nullopt;
                        continue;
                    }

                    const InitialDataFrame& parentInitData = initData.data[parentIt->second];

                    // Теперь ищем actual_date по CRTYS из parentInitData
                    KeyCRTYS5 parentCRTYSKey 
                    {
                        parentInitData.culture_id,
                        parentInitData.region_id,
                        parentInitData.t_material_id,
                        parentInitData.year,
                        parentInitData.season
                    };

                    auto parentSliceIt = initData.CRTYS_index_map.find(parentCRTYSKey);
                    if (parentSliceIt == initData.CRTYS_index_map.end())
                    {
                        frame.actual_input_date = std::nullopt;
                        continue;
                    }

                    // actual_date мы берём из uniqueSlices
                    auto& parentFrame = uniqueSlices[year][higher_tm];

                    std::optional<boost::gregorian::date> actual_date;
                    for (const auto& parentSlice : parentFrame)
                    {
                        for (const auto& candidate : parentSlice)
                        {
                            if (candidate.culture_id == parentInitData.culture_id &&
                                candidate.region_id == parentInitData.region_id &&
                                candidate.t_material_id == parentInitData.t_material_id &&
                                candidate.year == parentInitData.year &&
                                candidate.season == parentInitData.season)
                            {
                                actual_date = candidate.actual_date;
                                break;
                            }
                        }
                        if (actual_date.has_value()) break;
                    }

                    // Вычисляем actual_input_date
                    if (actual_date.has_value() && matchInitData.input_deadline.has_value())
                    {
                        frame.actual_input_date = actual_date.value() + boost::gregorian::days(matchInitData.input_deadline.value());
                    }
                    else
                    {
                        frame.actual_input_date = std::nullopt;
                    }
                }
            }
        }
    }
}

void calcAlternativeDate(YearSlices& uniqueSlices, const InitialData& initData)
{
    for (auto& [year, higherTmMap] : uniqueSlices)
    {
        for (auto& [higher_tm, sliceList] : higherTmMap)
        {
            for (auto& slice : sliceList)
            {
                for (auto& frame : slice)
                {
                    KeyCRTYS5 key{
                        frame.culture_id,
                        frame.region_id,
                        frame.t_material_id,
                        frame.year,
                        frame.season
                    };

                    auto it = initData.CRTYS_index_map.find(key);
                    if (it == initData.CRTYS_index_map.end())
                    {
                        frame.actual_alternative_date = std::nullopt;
                        continue;
                    }

                    const InitialDataFrame& matchInitData = initData.data[it->second];

                    // Проверка наличия input_operation_order
                    if (!matchInitData.input_operation_order.has_value())
                    {
                        frame.actual_alternative_date = std::nullopt;
                        continue;
                    }

                    // Ищем родительскую запись по input_operation_order
                    KeyOrder4 parentKey
                    {
                        matchInitData.culture_id,
                        matchInitData.region_id,
                        matchInitData.input_operation_order.value(),
                        matchInitData.year
                    };

                    auto parentIt = initData.order_index_map.find(parentKey);
                    if (parentIt == initData.order_index_map.end())
                    {
                        frame.actual_alternative_date = std::nullopt;
                        continue;
                    }

                    const InitialDataFrame& parentInitData = initData.data[parentIt->second];

                    // Теперь ищем actual_date по CRTYS из parentInitData
                    KeyCRTYS5 parentCRTYSKey
                    {
                        parentInitData.culture_id,
                        parentInitData.region_id,
                        parentInitData.t_material_id,
                        parentInitData.year,
                        parentInitData.season
                    };

                    auto parentSliceIt = initData.CRTYS_index_map.find(parentCRTYSKey);
                    if (parentSliceIt == initData.CRTYS_index_map.end())
                    {
                        frame.actual_alternative_date = std::nullopt;
                        continue;
                    }

                    // actual_date мы берём из uniqueSlices
                    auto& parentFrame = uniqueSlices[year][higher_tm];

                    std::optional<boost::gregorian::date> actual_date;
                    for (const auto& parentSlice : parentFrame)
                    {
                        for (const auto& candidate : parentSlice)
                        {
                            if (candidate.culture_id == parentInitData.culture_id &&
                                candidate.region_id == parentInitData.region_id &&
                                candidate.t_material_id == parentInitData.t_material_id &&
                                candidate.year == parentInitData.year &&
                                candidate.season == parentInitData.season)
                            {
                                actual_date = candidate.actual_date;
                                break;
                            }
                        }
                        if (actual_date.has_value()) break;
                    }

                    // Вычисляем actual_alternative_date
                    if (actual_date.has_value() && matchInitData.alternative_deadline.has_value())
                    {
                        frame.actual_alternative_date = actual_date.value() + boost::gregorian::days(matchInitData.alternative_deadline.value());
                    }
                    else
                    {
                        frame.actual_alternative_date = std::nullopt;
                    }
                }
            }
        }
    }
}
