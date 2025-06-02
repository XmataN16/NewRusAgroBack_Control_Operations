#include <iostream>
#include <Windows.h>
#include "SapControlTech.hpp"

int main() 
{
    SetConsoleOutputCP(65001); // UTF-8 поддержка

    CalcControlTechOperAggregated();

    return 0;
}