#pragma once
#include <optional>
#include <string>

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::optional<T>& value)
{
    if (value.has_value())
    {
        os << value.value();
    }
    else
    {
        os << "NULL";
    }
    return os;
}