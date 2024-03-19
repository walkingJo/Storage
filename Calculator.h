#pragma once
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
using std::string;

constexpr int maxToken = 100;

enum class ExpressionType {
			 NON,

		 INT_NUM,
	   FLOAT_NUM,

	   L_BRACKET,
	   R_BRACKET,
		   COMMA,

		TBD_FUNC,
	 PREFIX_FUNC,
	  INFIX_FUNC,
	POSTFIX_FUNC,
};

class Expression {
public:
	ExpressionType type;
	string str;

	Expression(ExpressionType type, string str) : type(type), str(str) { }
	Expression() { 
		string temp = string();
		Expression(ExpressionType::NON, temp);
	}
};

class Calculator {
private:
	Expression* tokens;
	int tokenCount;
	//int tokenCursor; /* token index */
	//int stringCursor;/* string index */

	ExpressionType getLastElementType();

	void addChar(char c);
	void deleteChar();
	
	void addElement(ExpressionType type);
	void deleteElement();
	
	bool doesNumberHasDot();
	
	bool isElementNum();
	bool isElementPrefix();
	bool isElementInfix();
	bool isElementPostfix();
	bool isExpressionEmpty();
public:
	Calculator();

	void clear();
	void backspace();

	//void moveCursorL();
	//void moveCursorR();

	void addNum(char number);
	void addNum(string number);
	void addDot();

	void addPrefixFunc(string function);
	void addInfixFunc(string function);
	void addPostfixFunc(string function);

	void addLBracket();
	void addRBracket();
	void addComma();

	string getTotalExpression();
	string calculate();

	static const string buttons[6][5];
};

#endif // !CALCULATOR_H