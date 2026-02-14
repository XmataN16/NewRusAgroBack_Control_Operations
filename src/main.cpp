#include <iostream>
#include <calc/SapControlTech/SapControlTech.hpp>
#include <calc/SapSugarBeetSawingDates/SapSugarBeetSawingDates.hpp>

#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

int main(int argc, char* argv[])
{
    #if defined(_WIN32)
    // Переключаем кодовую страницу консоли на UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif

    std::string mode = "-full"; // Режим работы по умолчанию (вычисление данных по контролю тех. операций)

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
