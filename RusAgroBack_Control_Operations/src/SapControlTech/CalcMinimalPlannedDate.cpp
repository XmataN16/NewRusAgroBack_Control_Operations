#include "CalcMinimalPlannedDate.hpp"
#include "utilsBoostDate.hpp"

void CalcPlannedDate(InitialData& InitData, int row)
{
	if (InitData.data[row].noinput_deadline.has_value() && InitData.data[row].region_date.has_value())
	{
		InitData.data[row].planned_dates.planned_date = InitData.data[row].region_date.value() + boost::gregorian::days(InitData.data[row].noinput_deadline.value());
	}
	else
	{
		InitData.data[row].planned_dates.planned_date = std::nullopt;
	}

}

void CalcInputDate(InitialData& InitData, int row)
{
	if (InitData.data[row].input_operation_order.has_value() && InitData.data[row].input_deadline.has_value())
	{
		Key4 key
		{
	       InitData.data[row].culture_id,
	       InitData.data[row].region_id,
		   InitData.data[row].input_operation_order.value(),
	       InitData.data[row].year
		};

		auto it = InitData.index_map.find(key);
		if (it != InitData.index_map.end())
		{
			int parent_row = it->second;
			std::optional<boost::gregorian::date> InputDateTemp;
			if (InitData.data[parent_row - 1].planned_dates.minimal_planned_date.has_value())
			{
				InputDateTemp = InitData.data[parent_row - 1].planned_dates.minimal_planned_date.value();
			}
			else
			{
				InputDateTemp = std::nullopt;
			}
			std::optional<int> DeadlineDays = InitData.data[row].input_deadline.value();
			if (InputDateTemp.has_value() && DeadlineDays.has_value())
			{
				InitData.data[row].planned_dates.input_date = InputDateTemp.value() + boost::gregorian::days(DeadlineDays.value());
			}
			else
			{
				InitData.data[row].planned_dates.input_date = std::nullopt;
			}
		}
		else
		{
			InitData.data[row].planned_dates.input_date = std::nullopt;
		}
	}
}

void CalcAlternativeDate(InitialData& InitData, int row)
{
	if (InitData.data[row].alternative_operation_order.has_value() && InitData.data[row].alternative_deadline.has_value())
	{
		Key4 key
		{
		   InitData.data[row].culture_id,
		   InitData.data[row].region_id,
		   InitData.data[row].alternative_operation_order.value(),
		   InitData.data[row].year
		};

		auto it = InitData.index_map.find(key);
		if (it != InitData.index_map.end())
		{
			int parent_row = it->second;
			std::optional<boost::gregorian::date> AlternativeDateTemp;
			if (InitData.data[parent_row - 1].planned_dates.minimal_planned_date.has_value())
			{
				AlternativeDateTemp = InitData.data[parent_row - 1].planned_dates.minimal_planned_date.value();
			}
			else
			{
				AlternativeDateTemp = std::nullopt;
			}
			std::optional<int> DeadlineDays = InitData.data[row].alternative_deadline.value();
			if (AlternativeDateTemp.has_value() && DeadlineDays.has_value())
			{
				InitData.data[row].planned_dates.alternative_date = AlternativeDateTemp.value() + boost::gregorian::days(DeadlineDays.value());
			}
			else
			{
				InitData.data[row].planned_dates.alternative_date = std::nullopt;
			}
		}
		else
		{
			InitData.data[row].planned_dates.alternative_date = std::nullopt;
		}
	}
}

void CalcMinimalPlannedDate(InitialData& InitData)
{
	for (int row = 0; row < InitData.Size(); row++)
	{
		CalcPlannedDate(InitData, row);
		CalcInputDate(InitData, row);
		CalcAlternativeDate(InitData, row);

		auto PlannedDate = InitData.data[row].planned_dates.planned_date;
		auto InputDate = InitData.data[row].planned_dates.input_date;
		auto AlternativeDate = InitData.data[row].planned_dates.alternative_date;

		if (InitData.data[row].planned_dates.alternative_date.has_value())
		{

			InitData.data[row].planned_dates.minimal_planned_date = max_date(PlannedDate, InputDate, AlternativeDate);
		}
		else
		{
			InitData.data[row].planned_dates.minimal_planned_date = min_date(PlannedDate, InputDate, AlternativeDate);
		}
	}
}