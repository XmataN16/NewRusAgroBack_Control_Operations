#include <iostream>
#include <Windows.h>
#include <SapControlTech.hpp>

int main()
{
    SetConsoleOutputCP(65001); // ¬ключить UTF-8 в консоли Windows

    calcSapControlAggregated();
}
