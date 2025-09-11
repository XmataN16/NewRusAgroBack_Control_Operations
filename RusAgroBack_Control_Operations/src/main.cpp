#include <iostream>
#include <Windows.h>
#include <SapControlTech.hpp>
#include <SapSugarBeetSawingDates.hpp>

int main(int argc, char* argv[])
{
    SetConsoleOutputCP(65001); // ¬ключить UTF-8 в консоли Windows

    std::string mode = "-full"; // –ежим работы по умолчанию (вычисление данных по контролю тех. операций)

    mode = argv[1];
    
    if (mode == "-full")
    {
        calcSapControlAggregated();
    }
    else if (mode == "-sugar_beet_sawing")
    {
        calcSapSugarBeetSawingDates();
    }

    return 0;
}
