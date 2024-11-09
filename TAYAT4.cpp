#include "Scaner.h"
#include "Syntax.h"
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <fstream>

int main()
{
    setlocale(LC_ALL, "rus");
    Scaner* scaner;
    scaner = new Scaner("input.txt");
    Syntax* syntax = new Syntax(scaner);

    syntax->program();

    int type; 
    Type_lex lex;
    type = scaner->UseScaner(lex);

    if (type == TEnd)
        std::cout << "\n\nСинтаксических ошибок не обнаружено\n\n";
    else
        std::cout << "\n\nERROR\n\n";

    return 0;
}

