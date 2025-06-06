#pragma once
#include <vector>
#include <string>
#include <pqxx/pqxx>

struct SapDataDistinctFrame
{
	int culture_id;
	int region_id;
	int t_material_id;
	int pu_id;
	std::string higher_tm;
	std::string season;
	int year;
};

struct SapDataDistinct
{
	std::vector<SapDataDistinctFrame> data;

	explicit SapDataDistinct(const pqxx::result& rows);

	int Size();

	void Print();
};