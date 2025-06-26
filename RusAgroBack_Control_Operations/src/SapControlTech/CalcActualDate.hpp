#pragma once
#include "SapData.hpp"
#include <unordered_map>
#include <optional>
#include <boost/date_time/gregorian/gregorian.hpp>

// Для удобства создаём индекс: t_material_id -> list of SapDataFrame (из полной выборки)
using TMaterialIndex = std::unordered_map<int, std::vector<const SapDataFrame*>>;

TMaterialIndex BuildTMaterialIndex(const YearSlices& fullSlices);

void calcActualDate(YearSlices& uniqueSlices, const YearSlices& fullSlices);