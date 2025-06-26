#include "CalcResawingDate.hpp"
#include <unordered_set>

void calcResawingDate(YearSlices& uniqueSlices, IDSReseeding idsReseeding)
{
    std::unordered_set<int> reseeding_ids(idsReseeding.IDs.begin(), idsReseeding.IDs.end());

    for (auto& [year, tmMap] : uniqueSlices)
    {
        for (auto& [higher_tm, sliceList] : tmMap)
        {
            std::optional<boost::gregorian::date> lastResawingDate;

            // �������� �� ���� ������ � ���������� ��������� ���� ��������
            for (const auto& slice : sliceList)
            {
                for (auto it = slice.rbegin(); it != slice.rend(); ++it)
                {
                    if (reseeding_ids.count(it->t_material_id))
                    {
                        if (!lastResawingDate || it->actual_date > lastResawingDate)
                        {
                            lastResawingDate = it->actual_date;
                        }
                        break; // ������ ���� ��������� �� ����� �����
                    }
                }
            }

            // ����������� ��������� ���� �� ��� ������ ���� ������
            for (auto& slice : sliceList)
            {
                for (auto& frame : slice)
                {
                    frame.resawing_date = lastResawingDate;
                }
            }
        }
    }
}