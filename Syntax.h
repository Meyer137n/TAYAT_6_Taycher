#pragma once

#include "Scaner.h"

class Syntax
{
private:
	//type_data last_type_data;
	//type_object last_type_object;
	Scaner* scaner;
	int look_forward(int pointer);
	int scan(Type_lex lex);

	void function();					// Функция
	void type();                        // Тип
	void id();					// Идентификаторы
	void operators_descriptions();       // Операторы и описания
	void operators();					// Операторы
	void composite_operator();          // Составной оператор
	void assignment();					// сравнение
	void data_description();			// Описание данных
	void named_const();					// именованная константа
	void operator_for();				// for
	void func_call();					// Вызов функции
	void expression();                  // Выражение
	void comparison();                  // Сравнение
	void addendum();                    // Слагаемое
	void multiplier();                  // Умножитель
	void unary_operation();             // Унарная операция
	void elementary_expression();       // Элементарное выражение

	void f_for();

public:
	Syntax(Scaner* scaner);
	void program();						// Программа
};