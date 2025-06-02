#pragma once
#include <string>
#include <optional>
#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>

struct SapDataAggregatedFrame
{
	std::string culture;
	std::string nzp_zp;
	std::string business_dir;
	std::string higher_tm;
	std::string material_order;
	std::string pu;
	std::string t_material;
	int year;

	// calc fields
	std::optional<int> order;
	std::optional<bool> is_completed;
	std::optional<boost::gregorian::date> minimal_planned_date;
	std::optional<boost::gregorian::date> actual_data;
	std::optional<boost::gregorian::date> actual_input_data;
	std::optional<boost::gregorian::date> actual_alternative_data;
	std::optional<boost::gregorian::date> ten_percent;
	std::optional<boost::gregorian::date> minimal_date;
	std::optional<boost::gregorian::date> sawing_date;
	std::optional<std::string> status;
	std::optional<std::string> is_actual;
};

struct SapDataAggregated
{
	std::vector<SapDataAggregatedFrame> frames;

	// Метод для добавления нового элемента
	void AddFrame(const SapDataAggregatedFrame& frame);

	// Метод вывода данных
	void Print() const;

	int Size();
};