#pragma once
#ifndef EXPRESSION_BUFFER_H
#define EXPRESSION_BUFFER_H

#include <string>
using std::string;

constexpr int maxToken = 100;

enum class ElementType {
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

class Element {
public:
	ElementType type;
	string str;

	Element(ElementType type, string str) : type(type), str(str) { }
	Element() { 
		type = ElementType::NON;
		str = "";
	}
};

class ExpressionBuffer {
public:
	Element* elements;
	int elementCount;
private:
	//int elementCursor; /* element index */
	//int stringCursor;  /* string index */

	ElementType getLastElementType();

	void addChar(char c);
	void deleteChar();
	
	void addElement(ElementType type);
	void deleteElement();
	
	bool isNumberHasDot();
	
	bool isElementNum();
	bool isElementPrefix();
	bool isElementInfix();
	bool isElementPostfix();
public:
	ExpressionBuffer();

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
};

#endif // !EXPRESSION_BUFFER_H