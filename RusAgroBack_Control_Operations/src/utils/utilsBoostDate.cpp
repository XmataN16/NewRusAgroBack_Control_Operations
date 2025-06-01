#include "utilsBoostDate.hpp"
#include <boost/date_time/gregorian/gregorian.hpp>

boost::gregorian::date parse_date(const std::string& date_str) 
{
    if (date_str.empty()) return boost::gregorian::date(); // Возвращаем "not a date"

    try 
    {
        return boost::gregorian::from_simple_string(date_str);
    }
    catch (...) 
    {
        int year, day, month;
        char sep1, sep2;
        std::istringstream ss(date_str);
        if (ss >> year >> sep1 >> day >> sep2 >> month && sep1 == '-' && sep2 == '-') 
        {
            return boost::gregorian::date(year, month, day);
        }
        throw std::runtime_error("Invalid date format: " + date_str);
    }
}

std::ostream& operator<<(std::ostream& os, const boost::gregorian::date& d) 
{
    if (d.is_not_a_date()) 
    {
        os << "NULL";
    }
    else 
    {
        os << boost::gregorian::to_simple_string(d);
    }
    return os;
}