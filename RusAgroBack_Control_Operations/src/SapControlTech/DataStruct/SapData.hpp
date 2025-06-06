#pragma once
#include <vector>
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <pqxx/pqxx>

struct SapDataFrame
{
	int culture_id;
	int region_id;
	int t_material_id;
	int pu_id;
	std::string higher_tm;
	std::string season;
	boost::gregorian::date calendar_day;
	int planned_volume;
	int actual_volume;
	int year;
};

struct SapData
{
	std::vector<SapDataFrame> data;

	explicit SapData(const pqxx::result& rows);

	int Size();

	void Print();
};