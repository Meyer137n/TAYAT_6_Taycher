#include "Diagram.h"

Diagram::Diagram(Scaner* scaner)
{
	this->scaner = scaner;
}

int Diagram::LookForward(int pos) 
{
	type_lex lex;
	int saved_pointer = scaner->GetUK();
	int next_type;
	for (int i = 0; i < pos; i++)
		next_type = scaner->UseScaner(lex);
	scaner->PutUK(saved_pointer);
	return next_type;
}

int Diagram::Scan(type_lex lex) 
{
	return scaner->UseScaner(lex);
}

void Diagram::Program()
{
	type_lex lex;
	int type;
	int simbol;
	int simbol2;

	type = LookForward(1);

	while (type == typeInt || type == typeShort || type == typeLong || type == typeConst)
	{
		type = LookForward(1);
		simbol = LookForward(3);
		simbol2 = LookForward(4);
		if (type == typeConst)
		{
			Named_const();
		}
		if ((type == typeInt || type == typeShort || type == typeLong) && simbol != typeLeftBracket && simbol2 != typeLeftBracket)
		{
			Data();
		}
		if ((type == typeInt || type == typeShort || type == typeLong) && (simbol == typeLeftBracket || simbol2 == typeLeftBracket))
		{
			Function();
		}
		type = LookForward(1);
	}

	if (type != typeEnd)
	{
		type = Scan(lex);
		scaner->PrintError("ожидался конец, ", lex);
	}
}

void Diagram::Data()
{
	type_lex lex;
	int type;
	int type2;
	Type();

	do {
		type = LookForward(1);

		if (type != typeId)
		{
			type = Scan(lex);
			scaner->PrintError("ожидался идентификатор, ", lex);
		}

		type2 = LookForward(2);
		if (type2 == typeEval)
		{
			Assignment();
			type = Scan(lex);
		}
		else
		{
			type = Scan(lex);
			type = Scan(lex);
		}
	} while (type == typeComma);

	if (type != typeSemicolon)
	{
		scaner->PrintError("ожидалась ;, ", lex);
	}
}

void Diagram::Named_const()
{
	type_lex lex;
	int type;

	type = LookForward(1);
	if (type != typeConst)
	{
		type = Scan(lex);
		scaner->PrintError("ожидалось const, ", lex);
		return;
	}
	type = Scan(lex);
	Type();

	do {

		Assignment();
		type = Scan(lex);
	}
	while (type == typeComma);

	if (type != typeSemicolon)
	{
		scaner->PrintError("ожидалась ;, ", lex);
	}
}



void Diagram::Function() 
{
	type_lex lex;
	int type;
	
	Type();
	
	type = Scan(lex);
	if (type != typeId && type != typeMain)
	{
		scaner->PrintError("ожидался идентификатор, ", lex);
	}

	type = Scan(lex);
	if (type != typeLeftBracket)
	{
		scaner->PrintError("ожидалась (, ", lex);
	}

	type = Scan(lex);
	if (type != typeRightBracket)
	{
		scaner->PrintError("ожидалась ), ", lex);
	}

	type = Scan(lex);
	if (type != typeLeftBrace)
	{
		scaner->PrintError("ожидалась {, ", lex);
	}

	type = LookForward(1);

	while (type != typeRightBrace)
	{
		OperatorsAndDescriptions();
		type = LookForward(1);
	}

	type = Scan(lex);
	if (type != typeRightBrace)
	{
		scaner->PrintError("ожилалась }, ", lex);
	}
}



void Diagram::OperatorsAndDescriptions()
{
	int type = LookForward(1);

	if (type == typeInt || type == typeShort || type == typeLong)
	{
		Data();
	}
	else Operator();
}

void Diagram::Type() 
{
	type_lex lex;
	int type;
	int type2;
	int type3;
	type = LookForward(1);
	if (type != typeInt && type != typeShort && type != typeLong)
	{
		type = Scan(lex);
		scaner->PrintError("ожидался тип (int, short, long), ", lex);
		return;
	}
	type = Scan(lex);

	if (type == typeShort)
	{
		type2 = LookForward(1);
		if (type2 == typeInt)
		{
			type = Scan(lex);
			return;
		}
		return;
	}

	if (type == typeLong)
	{
		type2 = LookForward(1);
		if (type2 == typeLong)
		{
			type3 = LookForward(2);
			if (type3 == typeInt)
			{
				type = Scan(lex);
				return;
			}
			type = Scan(lex);
			return;
		}
		if (type2 == typeInt)
		{
			type = Scan(lex);
			return;
		}
	}
}

void Diagram::Assignment() 
{
	type_lex lex;
	int type;

	type = Scan(lex);
	if (type != typeId) 
	{
		scaner->PrintError("ожидался идентификатор, ", lex);
	}

	type = Scan(lex);
	if (type != typeEval)
	{
		scaner->PrintError("ожидалось =, ", lex);
	}

	Expression();
}

void Diagram::Expression() 
{
	type_lex lex;
	int type;

	Comparison();
	type = LookForward(1);
	while (type == typeEq || type == typeUnEq) 
	{
		type = Scan(lex);
		Comparison();
		type = LookForward(1);
	}
}

void Diagram::CompositeOperator() 
{
	type_lex lex;
	int type;
	
	type = Scan(lex);
	if (type != typeLeftBrace)
	{
		scaner->PrintError("ожидалась {, ", lex);
	}

	type = LookForward(1);

	while (type != typeRightBrace)
	{
		OperatorsAndDescriptions();
		type = LookForward(1);
	}

	type = Scan(lex);
	if (type != typeRightBrace)
	{
		scaner->PrintError("ожидалась }, ", lex);
	}
}

void Diagram::Operator() 
{
	type_lex lex;
	int type = LookForward(1);

	if (type == typeReturn) 
	{
		type = Scan(lex);
		if (type != typeReturn)
		{
			scaner->PrintError("ожидалось return, ", lex);
		}

		Expression();

		type = Scan(lex);
		if (type != typeSemicolon)
		{
			scaner->PrintError("ожидалась ; после return <выражение>, ", lex);
		}

		return;
	}

	if (type == typeSemicolon) // пустой оператор
	{
		type = Scan(lex);
		return;
	}

	if (type == typeLeftBrace) 
	{
		CompositeOperator();
		return;
	}

	if (type == typeFor)
	{
		For_operator();
		return;
	}

	int type2 = LookForward(2);
	if (type == typeId && type2 == typeLeftBracket) 
	{
		FunctionCall();
		return;
	}

	if (type == typeId && type2 == typeEval) 
	{
		Assignment();
		type = Scan(lex);
		if (type != typeSemicolon)
			scaner->PrintError("ожидалась ;, ", lex);
		return;
	}

	if (type == typeId) 
	{
		Expression();
		return;
	}

	type = Scan(lex);
	scaner->PrintError("ожидался оператор, ", lex);
}

void Diagram::For_operator()
{
	type_lex lex;
	int type;

	type = Scan(lex);
	if (type != typeFor) 
	{
		scaner->PrintError("ожидалось while, ", lex);
	}

	type = Scan(lex);
	if (type != typeLeftBracket) 
	{
		scaner->PrintError("ожидалась (, ", lex);
	}

	type = LookForward(1);

	if (type == typeInt || type == typeShort || type == typeLong)
	{
		Data();
	}
	else
	{
		Assignment();
		type = Scan(lex);
		if (type != typeSemicolon)
		{
			scaner->PrintError("ожидалась ), ", lex);
		}
	}

	Expression();
	type = Scan(lex);
	if (type != typeSemicolon)
	{
		scaner->PrintError("ожидалась ), ", lex);
	}

	Assignment();

	type = Scan(lex);
	if (type != typeRightBracket) 
	{
		scaner->PrintError("ожидалась ), ", lex);
	}

	Operator();
}

void Diagram::FunctionCall() 
{
	type_lex lex;
	int type;

	type = Scan(lex);
	if (type != typeId)
	{
		scaner->PrintError("ожидался идентификатор, ", lex);
	}

	type = Scan(lex);
	if (type != typeLeftBracket)
	{
		scaner->PrintError("ожидалась (, ", lex);
	}

	type = Scan(lex);
	if (type != typeRightBracket)
	{
		scaner->PrintError("ожидалась ), ", lex);
	}

	type = Scan(lex);
	if (type != typeSemicolon)
	{
		scaner->PrintError("ожидалась ;, ", lex);
	}
}

void Diagram::Comparison() 
{
	type_lex lex;
	Summand();
	int type = LookForward(1);
	while (type == typeLess || type == typeLessOrEq || type == typeMore || type == typeMoreOrEq) 
	{
		type = Scan(lex);
		Summand();
		type = LookForward(1);
	}
}

void Diagram::Summand() 
{
	type_lex lex;
	int type;
	Multiplier();
	type = LookForward(1);
	while (type == typePlus || type == typeMinus) 
	{
		type = Scan(lex);
		Multiplier();
		type = LookForward(1);
	}
}

void Diagram::Multiplier() 
{
	type_lex lex;
	int type;
	UnaryOperation();
	type = LookForward(1);
	while (type == typeMul || type == typeDiv || type == typeMod) 
	{
		type = Scan(lex);
		UnaryOperation();
		type = LookForward(1);
	}
}

void Diagram::UnaryOperation()
{
	type_lex lex;
	int type = LookForward(1);

	if (type == typePlus || type == typeMinus)
	{
		type = Scan(lex);
	}
	ElementaryExpression();
}


void Diagram::ElementaryExpression() 
{
	type_lex lex;
	int type = LookForward(1);
	if (type == typeId) 
	{
		if (LookForward(2) == typeLeftBracket)
		{
			type = Scan(lex);
			type = Scan(lex);
		}
		type = Scan(lex);
		return;
	}
	if (type == typeConstInt || type == typeConstLongInt || type == typeConstHex || type == typeConstLongHex)
	{
		type = Scan(lex);
		return;
	}
	if (type == typeLeftBracket) 
	{
		type = Scan(lex);
		Expression();
		type = Scan(lex);
		if (type != typeRightBracket)
		{
			scaner->PrintError("ожидалась ), ", lex);
		}
		return;
	}
	type = Scan(lex);
	scaner->PrintError("ожидалось выражение, ", lex);
}