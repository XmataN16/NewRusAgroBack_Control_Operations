#pragma once
#include <string>
#include <optional>
#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>

struct SapDataFrame
{
	int id;
	std::string culture;
	std::string nzp_zp;
	std::string business_dir;
	boost::gregorian::date calendar_day;
	std::string higher_tm;
	std::string material_order;
	int planned_volume;
	int actual_volume;
	std::string pu;
	std::string t_material;
	int year;
};

struct SapData
{
	std::vector<SapDataFrame> frames;

	// Метод для добавления нового элемента
	void AddFrame(const SapDataFrame& frame);

	// Метод вывода данных
	void Print() const;

	int Size();
};