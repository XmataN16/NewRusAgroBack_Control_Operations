#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <optional>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <pqxx/pqxx>

struct Key4
{
    int culture_id;
    int region_id;
    int order;
    int year;

    bool operator==(Key4 const& o) const
    {
        return culture_id == o.culture_id
            && region_id == o.region_id
            && order == o.order
            && year == o.year;
    }
};

struct Key4Hash
{
    std::size_t operator()(Key4 const& key) const noexcept
    {
        std::size_t h1 = static_cast<std::size_t>(key.culture_id) * 73856093;
        std::size_t h2 = static_cast<std::size_t>(key.region_id) * 83492791;
        std::size_t h3 = static_cast<std::size_t>(key.order) * 19349663;
        std::size_t h4 = static_cast<std::size_t>(key.year) * 49979693;

        return (h1 ^ h2 ^ h3 ^ h4);
    }
};

struct PlannedDates
{
	std::optional<boost::gregorian::date> planned_date;
	std::optional<boost::gregorian::date> input_date;
	std::optional<boost::gregorian::date> alternative_date;
	std::optional<boost::gregorian::date> minimal_planned_date;
};

struct InitialDataFrame
{
	int culture_id;
	int t_material_id;
	int region_id;
	std::string season;
	std::string tech_operation;
	std::optional<boost::gregorian::date> region_date;
	std::optional<int> input_operation_order;
    std::optional<int> alternative_operation_order;
	std::optional<int> input_deadline;
	std::optional<int> alternative_deadline;
	std::optional<int> noinput_deadline;
	int order;
	int year;

    // calc struct
	PlannedDates planned_dates;
};

struct InitialData
{
	std::vector<InitialDataFrame> data;

	std::unordered_map<Key4, int, Key4Hash> index_map;

	explicit InitialData(const pqxx::result& rows);

	int Size();

	void Print();
};