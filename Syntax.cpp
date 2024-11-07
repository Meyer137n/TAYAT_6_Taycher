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
	return scaner->UseScaner(lex);
}

// Основной метод для анализа структуры программы
void Syntax::program() {
	//std::cout << "\nОбнаружено: программа";
	Type_lex lex;
	int type;
	type = look_forward(1);  // Заглядываем вперед на 1 лексему
	while (type == TInt || type == TShort || type == TLong || type == TConst)
	{
		type = look_forward(1);
		int type3 = look_forward(3);
		int type2 = look_forward(2);
		int type4 = look_forward(4);
		if (type == TConst) {
			named_const();
		}

		if ((type == TInt || type == TShort || type == TLong ) && type3 != TLBracket) {
			data();
		}

		if ((type == TLong && type2 == TLong) && type4 != TLBracket) {
			data();
		}

		if ((type == TInt || type == TShort || type == TLong) && type3 == TLBracket) {
			function();
		}

		if((type == TLong && type2 == TLong) && type4 == TLBracket) {
			function();
		}
	}
	if (type != TEnd) {  // Ожидаем конец программы
		type = scan(lex);
		scaner->PrintError("\nExpected end got", lex);
	}
}

// Метод для обработки именованной константы
void Syntax::named_const() {
	//std::cout << "\nОбнаружено: именованная константа\n";
	Type_lex lex;
	int type;

	type = scan(lex); // Ожидаем ключевое слово const
	if (type != TConst)
	{
		scaner->PrintError("\nExpected const got", lex);
		return;
	}

	this->type();	

	// Обработка присваивания значения константе
	do {
		assignment();
		// Проверяем на наличие запятой или конца строки
		type = scan(lex);
		if (type == TSemi) 
		{
			break;
		}
		if (type != TComma && type != TSemi)
		{
			scaner->PrintError("\nExpected ',' or ';' got", lex);
			return;
		}
	} while (true);
}

// Метод для обработки присваивания
void Syntax::assignment() {
	//std::cout << "Обнаружено: присваивание\n";
	Type_lex lex;
	int type;
	
	// Ожидаем имя константы
	type = scan(lex);
	if (type != TId) {
		scaner->PrintError("\nExpected identifier got", lex);
	}
	//std::cout << lex << " ";

	// Ожидаем символ '='
	type = scan(lex);
	if (type != TAssign)
		scaner->PrintError("\nExpected = got", lex);
	//std::cout << lex << " ";

	expression();
}

// Метод для обработки выражений
void Syntax::expression() {
	//std::cout << "Обнаружено: выражение\n";
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
	//std::cout << "Обнаружено: сравнение\n";
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
	//std::cout << "Обнаружено: сложение или вычитание\n";
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
	//std::cout << "Обнаружено: умножение или деление\n";
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
	//std::cout << "Обнаружено: унарная операция\n";
	Type_lex lex;
	int type = look_forward(1);

	if (type == TPlus || type == TMinus) {
		type = scan(lex);
		idary_expression();
	}
	else {
		idary_expression();
	}
}

// Метод для обработки простых выражений
void Syntax::idary_expression() {
	//std::cout << "Обнаружено: простое выражение\n";
	Type_lex lex;
	int type = look_forward(1);
	if (type == TConstInt || type == TCLongInt || type == TConstHex || type == TCLongHex) {
		type = scan(lex);
		std::cout << "\n"<<lex<<"\n";
		//int type = look_forward(1);
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

// Метод для проверки типа (int, short, long, long long)
void Syntax::type() {
	Type_lex lex;
	int type;
	// int, short, long
	type = scan(lex);
	if (type != TInt && type != TShort && type != TLong && look_forward(1) != TLong)
	{
		scaner->PrintError("\nExpected type got", lex);
		return;
	}
	std::cout << lex << " ";

	//long long int
	if (type == TLong && look_forward(1) == TLong && look_forward(2) == TInt)
	{
		scan(lex);
		std::cout << lex << " ";
		//scan(lex);
		//std::cout << lex << " ";
	}

	// long long, long short
		if (type == TLong && (look_forward(1) == TLong || look_forward(1) == TInt) || type == TShort && look_forward(1) == TInt)
	{
		//scan(lex);
		std::cout << lex << " ";
	}
}

// Метод для обработки объявления функции
void Syntax::function() {
	Type_lex lex;

	//int type = scan(lex);
	//std::cout << "Обнаружено: функция => ";
	this->type();
	int type = scan(lex);
	std::cout << lex << " ";

	if (type != TId && type != TMain)
		scaner->PrintError("\nExpected identifier got", lex);

	type = scan(lex);
	std::cout << lex << " ";
	if (type != TLBracket)
		scaner->PrintError("\nExpected ( got", lex);


	type = scan(lex);
	std::cout << lex << " ";
	if (type != TRBracket)
		scaner->PrintError("\nExpected ) got", lex);

	type = scan(lex);
	std::cout << lex << " ";
	if (type != TLBrace)
		scaner->PrintError("\nExpected { got", lex);

	operators_and_descriptions();

	type = scan(lex);
	std::cout << lex << " ";
	if (type != TRBrace)
		scaner->PrintError("\nExpected } got", lex);
}



// Метод для описания переменной или функции
void Syntax::description() {
	//std::cout << "Обнаружено: описание\n";
	Type_lex lex;
	int type = look_forward(1);
	int simbol = look_forward(3);
	if ((type == TInt || type == TShort || type == TLong) && simbol != TLBracket) {
		data();  // Обработка переменной
		return;
	}
	if ((type == TInt || type == TShort || type == TLong) && simbol == TLBracket) {
		function();  // Обработка функции
		return;
	}
	type = scan(lex);
	scaner->PrintError("\nExpected type got", lex);
}

// Обработка списка элементов, разделенных запятыми
void Syntax::list() {
	//std::cout << "Обнаружено: список\n";
	Type_lex lex;
	int type;

	id();

	type = look_forward(1);

	while (type == TComma) {
		type = scan(lex);
		id();
		type = look_forward(1);
	}
}

// Обработка одного элемента (например, переменной)
void Syntax::id() {
	//std::cout << "Обнаружено: элемент\n";
	Type_lex lex;
	int type;
	type = look_forward(1);
	if (type == TId) {
		variable();
		return;
	}
	else {
		type = scaner->UseScaner(lex);
		scaner->PrintError("\nExpected id got", lex);
	}
}

// Метод для обработки объявления переменной
void Syntax::data() {
	//std::cout << "Обнаружено: данные\n";
	Type_lex lex;
	int Type_;

	type();

	list();

	Type_ = scan(lex);
	if (Type_ != TSemi)
		scaner->PrintError("\nExpected ; got", lex);
}

// Метод для обработки переменной
void Syntax::variable() {
	//std::cout << "Обнаружено: переменная\n";
	Type_lex lex;
	int type;

	type = look_forward(1);

	if (type != TId) {
		type = scan(lex);
		scaner->PrintError("\nExpected identifier got", lex);
	}

	int pointer = scaner->GetUK();
	type = scan(lex);
	scaner->PutUK(pointer);

	type = look_forward(2);
	if (type == TAssign) {
		assignment();
		return;
	}
	type = scan(lex);
}

// Метод для составного оператора
void Syntax::composite_operator() {
	//std::cout << "Обнаружено: составной оператор\n";
	Type_lex lex;
	int type;

	type = scan(lex);
	if (type != TLBrace)
		scaner->PrintError("\nExpected { got", lex);

	operators_and_descriptions();

	type = scan(lex);
	if (type != TRBrace)
		scaner->PrintError("\nExpected } got", lex);
}

// Метод для обработки операторов и описаний
void Syntax::operators_and_descriptions() {
	//std::cout << "Обнаружено: операторы и описания\n";
	Type_lex lex;
	int type;

	type = look_forward(1);
	while (type != TRBrace) {
		if (type == TInt || type == TShort || type == TLong) {
			data();
		}
		else {
			operator_();
		}
		type = look_forward(1);
	}
}

// Метод для обработки различных операторов
void Syntax::operator_() {
	//std::cout << "Обнаружено: оператор\n";
	Type_lex lex;
	int type;

	type = look_forward(1);

	if (type == TReturn) {
		return_statement();
		return;
	}

	if (type == TSemi) {
		type = scan(lex);
		return;
	}

	if (type == TLBrace) {
		composite_operator();
		return;
	}

	if (type == TFor) {
		cycle();
		return;
	}

	int T2 = look_forward(2);
	if (type == TId && T2 == TLBracket) {
		function_call();
		return;
	}

	if (type == TId && T2 == TAssign) {
		assignment();
		type = scan(lex);
		if (type != TSemi)
			scaner->PrintError("\nExpected ; got", lex);
		return;
	}

	type = scan(lex);
	scaner->PrintError("\nExpected operator got", lex);
}

// Метод для обработки оператора return
void Syntax::return_statement() {
	//std::cout << "Обнаружено: return\n";
	Type_lex lex;
	int type;

	type = scan(lex);
	if (type != TReturn) {
		scaner->PrintError("\nExpected return got", lex);
	}

	expression();

	type = scan(lex);
	if (type != TSemi) {
		scaner->PrintError("\nExpected ; after return statement got", lex);
	}
}

// Метод для обработки цикла for
void Syntax::cycle() {
	//std::cout << "Обнаружено: цикл for\n";
	Type_lex lex;
	int type;

	type = scan(lex);
	if (type != TFor) {
		scaner->PrintError("\nExpected for got", lex);
	}

	type = scan(lex);
	if (type != TLBracket) {
		scaner->PrintError("\nExpected ( got", lex);
	}

	expression();

	type = scan(lex);
	if (type != TRBracket) {
		scaner->PrintError("\nExpected ) got", lex);
	}

	operator_();
}

// Метод для обработки вызова функции
void Syntax::function_call() {
	//std::cout << "Обнаружено: вызов функции\n";
	Type_lex lex;
	int type;

	type = scan(lex);
	if (type != TId)
		scaner->PrintError("\nExpected identifier got", lex);

	type = scan(lex);
	if (type != TLBracket)
		scaner->PrintError("\nExpected ( got", lex);

	type = scan(lex);
	if (type != TRBracket)
		scaner->PrintError("\nExpected ) got", lex);

	type = scan(lex);
	if (type != TSemi)
		scaner->PrintError("\nExpected ; got", lex);
}


