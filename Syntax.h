#pragma once

#include "Scaner.h"

class Syntax
{
private:
	//type_data last_type_data;
	//type_object last_type_object;
	Scaner* scaner;
	int scan(Type_lex lex);
	int look_forward(int pointer);

	void description();                  // Описание
	void id();
	void data();                         // Данные
	//void function();					 // Функция
	//void type();                         // Тип
	void list();                         // Список
	void variable();                     // Переменная
	//void assignment();                   // Присваивание
	void composite_operator();           // Составной оператор
	void operators_and_descriptions();   // Операторы и описания
	void operator_();                    // Оператор
	void function_call();                // Вызов функции
	//void expression();                   // Выражение
	//void comparison();                   // Сравнение
	//void addendum();                     // Слагаемое
	//void multiplier();                   // Умножитель
	//void unary_operation();              // Унарная операция
	//void idary_expression();        // Элементарное выражение
	void return_statement();
	void cycle();


	void function();					// Функция
	void type();                        // Тип
	//void operators_description();     // Операторы и описания
	//void operators();					// Операторы
	//void composite_operator();        // Составной оператор
	void assignment();					// сравнение
	//void data_description();			// Описание данных
	void named_const();					// именованная константа
	//void operator_for();				// for
	//void func_call();					// Вызов функции
	void expression();                  // Выражение
	void comparison();                  // Сравнение
	void addendum();                    // Слагаемое
	void multiplier();                  // Умножитель
	void unary_operation();             // Унарная операция
	void idary_expression();            // Элементарное выражение

public:
	Syntax(Scaner* scaner);
	void program();						// Программа
};