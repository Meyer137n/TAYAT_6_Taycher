﻿#include "Scaner.h"
#include "Diagram.h"
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <fstream>

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Scaner* scaner;
    scaner = new Scaner("input.txt");
    Diagram* diagram = new Diagram(scaner);

    diagram->Program();

    int type; type_lex lex;
    type = scaner->UseScaner(lex);

    if (type == typeEnd)
        std::cout << "\n\nОшибок не обнаружено\n\n";
    else
        std::cout << "\n\nError\n\n";

    return 0;
}
