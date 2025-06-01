#pragma once
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>

boost::gregorian::date parse_date(const std::string& date_str);

std::ostream& operator<<(std::ostream& os, const boost::gregorian::date& d);