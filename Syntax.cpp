#include "Syntax.h"
#include <iostream>

// Конструктор класса Syntax, инициализирующий указатель на объект Scaner
Syntax::Syntax(Scaner* scaner)
{
	this->scaner = scaner;
}

// Метод для заглядывания вперед на несколько лексем, не изменяя текущей позиции
int Syntax::look_forward(int pos) {
	Type_lex lex;
	int saved_pointer = scaner->GetUK();  // Сохраняем текущую позицию указателя
	int next_type;
	for (int i = 0; i < pos; i++)
		next_type = scaner->UseScaner(lex);  // Сканируем следующую лексему
	scaner->PutUK(saved_pointer);  // Восстанавливаем исходную позицию указателя
	return next_type;  // Возвращаем тип лексемы на позиции pos
}

// Метод для получения следующей лексемы и продвижения указателя
int Syntax::scan(Type_lex lex) {
	int type = scaner->UseScaner(lex);
	return type;
}


void Syntax::program() {
	Type_lex lex;
	int type;

	type = look_forward(1); // определение первого символа

	if (type != TConst && type != TInt && type != TShort && type != TLong)
	{
		type = scan(lex);
		scaner->PrintError("Expected type got", lex);
		return;
	}

	while (type == TInt || type == TShort || type == TLong || type == TConst) 
	{
		{
			type = look_forward(1);
			int simbol = look_forward(3); // проверка левой скобки для int,short,long
			int simbol2 = look_forward(4); // для long long
			if (type == TConst)
			{
				named_const();
				type = look_forward(1);
			}
			if ((type == TInt || type == TShort || type == TLong) && (simbol == TLBracket || simbol2 == TLBracket)) {
				function();
				type = look_forward(1);
			}
			if ((type == TInt || type == TShort || type == TLong) && (simbol != TLBracket || simbol2 != TLBracket)) {
				data_description();
				type = look_forward(1);
			}
		}
	}

	if (type != TEnd) {
		type = scan(lex);
		scaner->PrintError("Expected end got", lex);
	}
}

// Метод для обработки именованной константы
void Syntax::named_const() {
	Type_lex lex;
	int type;

	type = scan(lex); // Ожидаем ключевое слово const
	if (type != TConst)
	{
		scaner->PrintError("\nExpected const got", lex);
		return;
	}

	this->type();	

	do {
		assignment();
		type = scan(lex);
		if (type != TComma && type != TSemi)
		{
			scaner->PrintError("\nExpected ',' or ';' got", lex);
			return;
		}
	} while (type != TSemi);
}

// Метод для обработки объявления функции
void Syntax::function() {
	Type_lex lex;

	this->type();

	int type = scan(lex);
	if (type != TId && type != TMain)
		scaner->PrintError("\nExpected identifier got", lex);

	type = scan(lex);
	if (type != TLBracket)
		scaner->PrintError("\nExpected ( got", lex);

	type = scan(lex);
	if (type != TRBracket)
		scaner->PrintError("\nExpected ) got", lex);

	type = scan(lex);
	if (type != TLBrace)
		scaner->PrintError("\nExpected { got", lex);

	while (type != TRBrace)
	{
		operators_descriptions();
		type = look_forward(1);
	}

	type = scan(lex);
	if (type != TRBrace)
		scaner->PrintError("\nExpected } got", lex);
}

// Описание данных
void Syntax::data_description()
{
	Type_lex lex;
	int type;

	type = look_forward(1); // Ожидаем ключевое слово const
	if (type != TInt && type != TShort && type != TLong)
	{
		scaner->PrintError("\nExpected const got", lex);
		return;
	}
	this->type();

	// Обработка присваивания значения константе
	do {
		type = look_forward(1);
		if (type != TId) {
			scaner->PrintError("\nExpected identifier got", lex);
			return;
		}

		type = look_forward(2);
		if (type == TAssign)
		{
			assignment();
			type = look_forward(1);
		}
		else 
		{
			scan(lex);
		}

		if (type != TComma && type != TSemi)
		{
			scaner->PrintError("\nExpected ',' or ';' got", lex);
			return;
		}
		type = scan(lex);
	} while (type != TSemi);
}

// Метод для обработки присваивания
void Syntax::assignment() {
	Type_lex lex;
	int type;

	// Ожидаем имя константы
	type = scan(lex);
	if (type != TId) {
		scaner->PrintError("\nExpected identifier got", lex);
		return;
	}

	// Ожидаем символ '='
	type = scan(lex);
	if (type != TAssign)
	{
		scaner->PrintError("\nExpected = got", lex);
		return;
	}

	expression();
}

// Метод для проверки типа (int, short, long, long long)
void Syntax::type() {
	Type_lex lex;
	int type;

	// int, short, long
	type = scan(lex);
	if (type != TInt && type != TShort && (type != TLong || (type != TLong && look_forward(1) != TLong)))
	{
		scaner->PrintError("\nExpected type got", lex);
		return;
	}

	// long long
	if (type == TLong && look_forward(1) == TLong)
	{
		scan(lex);
	}
}

// Метод для обработки операторов и описаний
void Syntax::operators_descriptions() {
	Type_lex lex;
	int type;

	type = look_forward(1);

	if (type == TInt || type == TShort || type == TLong) {
		data_description();
	}
	else {
		operators();
	}
}

// Оператор For
void Syntax::operator_for()
{
	Type_lex lex;
	int type;

	type = scan(lex);
 	if (type != TFor) {
		scaner->PrintError("Expected while got", lex);
	}

	type = scan(lex);
	if (type != TLBracket) {
		scaner->PrintError("Expected ( got", lex);
	}
	assignment();

	type = scan(lex);
	if (type != TSemi)
		scaner->PrintError("Expected ; got", lex);
	expression();

	type = scan(lex);
	if (type != TSemi)
		scaner->PrintError("Expected ; got", lex);
	assignment();

	type = scan(lex);
	if (type != TRBracket)
		scaner->PrintError("Expected ) got", lex);

	operators();
}

// Операторы
void Syntax::operators()
{
	Type_lex lex;
	int type;

	type = look_forward(1);

	if (type == TRBrace) //без операторов
	{
		return;
	}

	if (type == TReturn) // Оператор for
	{
		type = scan(lex);
		expression();

		type = scan(lex);
		if (type != TSemi) {
			scaner->PrintError("\nExpected ; got", lex);
		}
		return;
	}

	if (type == TSemi) // пустой оператор
	{
		type = scan(lex);
		return;
	}

	if (type == TLBrace) // составной оператор
	{
		composite_operator();
		return;
	}

	if (type == TFor) // оператор for
	{
		operator_for();
		return;
	}

	int type2 = look_forward(2);
	if (type == TId && type2 == TLBracket) // вызов функции
	{
		func_call();
		return;
	}

	if (type == TId && type2 == TAssign) // присваивание
	{
		assignment();
		type = scan(lex);
		if (type != TSemi)
			scaner->PrintError("\nExpected ; got", lex);
		return;
	}

	scaner->PrintError("\nExpected operator got", lex);
}

// Составной оператор
void Syntax::composite_operator()
{
	Type_lex lex;
	int type;

	type = scan(lex);
	if (type != TLBrace)
		scaner->PrintError("Expected { got", lex);

	while (type != TRBrace)
	{
		operators_descriptions();
		type = scan(lex);
	}

	if (type != TRBrace)
		scaner->PrintError("Expected } got", lex);

}

// Вызов функции
void Syntax::func_call()
{
	Type_lex lex;
	int type;

	type = scan(lex);
	if (type != TId)
		scaner->PrintError("Expected identificator got", lex);

	type = scan(lex);
	if (type != TLBracket)
		scaner->PrintError("Expected ( got", lex);

	type = scan(lex);
	if (type != TRBracket)
		scaner->PrintError("Expected ) got", lex);

	type = scan(lex);
	if (type != TSemi)
		scaner->PrintError("Expected ; got", lex);
}

// Метод для обработки выражений
void Syntax::expression() {
	Type_lex lex;
	int type;

	comparison();
	type = look_forward(1);
	while (type == TEq || type == TNotEq) {
		type = scan(lex);
		comparison();
		type = look_forward(1);
	}
}

// Метод для обработки сравнений
void Syntax::comparison() {
	Type_lex lex;
	int type;
	addendum();
	type = look_forward(1);
	while (type == TLess || type == TLessEq || type == TGreater || type == TGreaterEq) {
		type = scan(lex);
		addendum();
		type = look_forward(1);
	}
}

// Метод для обработки сложения и вычитания
void Syntax::addendum() {
	Type_lex lex;
	int type;

	multiplier();

	type = look_forward(1);
	while (type == TPlus || type == TMinus) {
		type = scan(lex);
		multiplier();
		type = look_forward(1);
	}
}

// Метод для обработки умножения и деления
void Syntax::multiplier() {
	Type_lex lex;
	int type;

	unary_operation();

	type = look_forward(1);
	while (type == TMul || type == TDiv || type == TMod) {
		type = scan(lex);
		unary_operation();
		type = look_forward(1);
	}
}

// Метод для обработки унарных операций
void Syntax::unary_operation() {
	Type_lex lex;
	int type = look_forward(1);

	if (type == TPlus || type == TMinus) {
		type = scan(lex);
		elementary_expression();
	}
	else {
		elementary_expression();
	}
}

// Метод для обработки простых выражений
void Syntax::elementary_expression() {
	Type_lex lex;
	int type = look_forward(1);
	if (type == TConstInt || type == TCLongInt || type == TConstHex || type == TCLongHex) {
		type = scan(lex);
		return;
	}
	if (type == TId) {
		type = scan(lex);
		int type = look_forward(1);
		return;
	}
	else if (type == TInt || type == TShort || type == TLong) {
		type = scan(lex);
		return;
	}
	else if (type == TLBracket) {
		type = scan(lex);
		expression();
		type = scan(lex);
		if (type != TRBracket)
			scaner->PrintError("\nExpected ) got ", lex);
		return;
	}
	else {
		type = scan(lex);
		scaner->PrintError("\nExpected expression got", lex);
	}
}