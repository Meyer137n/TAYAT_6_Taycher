#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxText 100000			// Максимальная длина текста
#define maxLex 100				// Максимальная длина лексемы
#define maxKeyword 20			// Максимальная длина ключевого слова
#define maxDecLex 10			// Максимальная длина int 10сс числа
#define maxHexLex 8			// Максимальная длина 16cc числа
typedef char Type_mod[maxText]; // Хранение текста
typedef char Type_lex[maxLex];  // Хранение лексем

/**
* Ключевые слова
*/
#define TInt 1
#define TShort 2
#define TLong 3
#define TFor 4
#define TReturn 5
#define TMain 6
#define TConst 7

/**
* Тип лексем
*/
#define TId 20

/**
* Константы
*/
#define TConstInt 30
#define TConstHex 31
#define TCLongInt 32
#define TCLongHex 33

/**
* Знаки операций
*/
#define TPlus 40			// +
#define TMinus 41			// -
#define TMul 42				// *
#define TDiv 43				// /
#define TMod 44				// %
#define TEq 45				// ==
#define TNotEq 46			// !=
#define TLess 47			// <
#define TLessEq 48			// <=
#define TGreater 49			// >
#define TGreaterEq 50		// >=
#define TAssign 51			// =

/**
* Специальные знаки
*/
#define TSemi 60		// ;
#define TComma 61		// ,
#define TLBracket 62	// (
#define TRBracket 63	// )
#define TLBrace 64		// {
#define TRBrace 65		// }

/**
* Дополнительно
*/
#define TEnd 100
#define TError 200