#include "CalcMinimalPlannedDate.hpp"
#include "utilsBoostDate.hpp"

//Вычисление плановой даты завершения
void CalcPlannedDate(InitialData& InitData, int frame)
{
	if (InitData.frames[frame].noinput_deadline.has_value() && InitData.frames[frame].region_date.has_value())
	{
		InitData.frames[frame].planned_dates.planned_date = InitData.frames[frame].region_date.value() + boost::gregorian::days(InitData.frames[frame].noinput_deadline.value());
	}
	else
	{
		InitData.frames[frame].planned_dates.planned_date = std::nullopt;
	}
}

//Вычисление вводной даты завершения
void CalcInputDate(InitialData& InitData, int frame)
{
	if (InitData.frames[frame].input_operation.has_value() && InitData.frames[frame].deadline_input.has_value())
	{
		int input_operation_id = InitData.frames[frame].input_operation.value();
		std::optional<boost::gregorian::date> InputDate;
		if (InitData.frames[input_operation_id - 1].planned_dates.minimal_planned_date.has_value())
		{
			InputDate = InitData.frames[input_operation_id - 1].planned_dates.minimal_planned_date.value();
		}
		else
		{
			InputDate = std::nullopt;
		}

		std::optional<int> DeadlineInputDays = InitData.frames[frame].deadline_input.value();
		if (DeadlineInputDays.has_value() && InputDate.has_value())
		{
			InitData.frames[frame].planned_dates.input_date = InputDate.value() + boost::gregorian::days(DeadlineInputDays.value());
		}
		else
		{
			InitData.frames[frame].planned_dates.input_date = std::nullopt;
		}
	}
	else
	{
		InitData.frames[frame].planned_dates.input_date = std::nullopt;
	}
}

//Вычисление альтеранативной даты завершения
void CalcAlternativeDate(InitialData& InitData, int frame)
{
	if (InitData.frames[frame].alternative_input.has_value() && InitData.frames[frame].alternative_complete.has_value())
	{
		int AlternativeOperationId = InitData.frames[frame].alternative_input.value();
		std::optional<boost::gregorian::date> AlternativeDate;
		if (InitData.frames[AlternativeOperationId - 1].planned_dates.minimal_planned_date.has_value())
		{
			AlternativeDate = InitData.frames[AlternativeOperationId - 1].planned_dates.minimal_planned_date.value();
		}
		else
		{
			AlternativeDate = std::nullopt;
		}

		std::optional<int> AlternativeCompleteDays = InitData.frames[frame].alternative_complete.value();
		if (AlternativeCompleteDays.has_value() && AlternativeDate.has_value())
		{
			InitData.frames[frame].planned_dates.alternative_date = AlternativeDate.value() + boost::gregorian::days(AlternativeCompleteDays.value());
		}
		else
		{
			InitData.frames[frame].planned_dates.alternative_date = std::nullopt;
		}
	}
	else
	{
		InitData.frames[frame].planned_dates.alternative_date = std::nullopt;
	}
}

//Вычисление кратчайших плановых дат
void CalcMinimalPlannedDate(InitialData& InitData)
{
    #pragma omp parallel for
	for (int frame = 0; frame < InitData.Size(); frame++)
	{
		CalcPlannedDate(InitData, frame);
		CalcInputDate(InitData, frame);
		CalcAlternativeDate(InitData, frame);

		auto PlannedDate = InitData.frames[frame].planned_dates.planned_date;
		auto InputDate = InitData.frames[frame].planned_dates.planned_date;
		auto AlternativeDate = InitData.frames[frame].planned_dates.planned_date;

		if (InitData.frames[frame].planned_dates.alternative_date.has_value())
		{
			
			InitData.frames[frame].planned_dates.minimal_planned_date = max_date(PlannedDate, InputDate, AlternativeDate);
		}
		else
		{
			InitData.frames[frame].planned_dates.minimal_planned_date = min_date(PlannedDate, InputDate, AlternativeDate);
		}
	}
}