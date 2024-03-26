#include "Calculator.h"
#include "ExpressionBuffer.h"
#include <cassert>

ElementType ExpressionBuffer::getLastElementType() {
	return elements[elementCount - 1].type;
}

void ExpressionBuffer::addChar(char c) {
	//tokens[tokenCursor].str.insert(stringCursor, str);
	elements[elementCount - 1].str += c;
}
// 커서 위치의 char를 삭제한다
//  만약 삭제한 char가 해당 element의 마지막이었다면(첫 번째 문자였다면) 해당 element까지 삭제한다
/* 지울 때 빈 공간에 '_'를 남길 것인가? */
void ExpressionBuffer::deleteChar() {
	if (elementCount) {
		if (elements[elementCount - 1].str.length() > 0)
			elements[elementCount - 1].str.pop_back();
		if (elements[elementCount - 1].str.length() == 0)
			deleteElement();
	}
	// version 1 
	/*if (elementsLength) {
		if (elements[elementsLength - 1].str.length() >= 1)
			elements[elementsLength - 1].str.pop_back();
		if (elements[elementsLength - 1].str.length() == 0)
			deleteElement();
	}*/
	// version 2 
	/*char* text = (char*)elements[elementCursor].str.c_str();
	for (int i = stringCursor; i < strlen(text); ++i)
		text[i] = text[i + 1];
	elements[elementCursor].str = text;*/
}
void ExpressionBuffer::addElement(ElementType type) {
	// version 1
	/*elementsLength++;
	elements[elementsLength - 1].type = type;
	elements[elementsLength - 1].str = "";*/
	if (elementCount != 0 && elements[elementCount - 1].str.back() == '.') {
		deleteChar();
		elements[elementCount - 1].type = ElementType::INT_NUM;
	}
	elementCount++;
	elements[elementCount - 1].type = type;
	elements[elementCount - 1].str = "";
}
// 해당 element 자체를 완전히 밀어버리는 개념
void ExpressionBuffer::deleteElement() {
	// version 1
	/*if (elementsLength >= 1) {
		elements[elementsLength - 1].type = TokenType::NON;
		elements[elementsLength - 1].str = "";
		elementsLength--;
	}*/
	if (elementCount >= 1) {
		elements[elementCount - 1].type = ElementType::NON;
		elements[elementCount - 1].str = "";
		elementCount--;
	}
}
bool ExpressionBuffer::isNumberHasDot() {
	assert(isElementNum());
	if (elements[elementCount - 1].str.find('.') != string::npos)
		return true;
	else
		return false;
}

bool ExpressionBuffer::isElementNum() {
	switch (elements[elementCount-1].type) {
	case ElementType::INT_NUM:
	case ElementType::FLOAT_NUM:
		return true;
	default:
		return false;
	}
}
bool ExpressionBuffer::isElementPrefix() {
	switch (getLastElementType()) {
	case ElementType::L_BRACKET:
	case ElementType::PREFIX_FUNC:
		return true;
	default:
		return false;
	}
}
bool ExpressionBuffer::isElementInfix() {
	switch (getLastElementType()) {
	case ElementType::COMMA:
	case ElementType::TBD_FUNC:
	case ElementType::INFIX_FUNC:
		return true;
	default:
		return false;
	}
}
bool ExpressionBuffer::isElementPostfix() {
	switch (getLastElementType()) {
	case ElementType::R_BRACKET:
	case ElementType::POSTFIX_FUNC:
		return true;
	default:
		return false;
	}
}

ExpressionBuffer::ExpressionBuffer() {
	// version 1
	/*elements = new Token[maxToken];
	for (int i = 0; i < maxToken; ++i)
		elements[i] = Token();
	elementsLength = 0;

	elementCursor = 0;
	stringCursor = 0;*/
	elements = new Element[maxToken];
	for (int i = 0; i < maxToken; ++i)
		elements[i] = Element();
	elementCount = 0;
}
string ExpressionBuffer::calculate() {
	Calculator calc = Calculator(this);
	return calc.calculate().str;
}
string ExpressionBuffer::getTotalExpression() {
	string result = "";
	for (int i = 0; i < elementCount; ++i) {
		result += elements[i].str;
	}
	return result;
}

void ExpressionBuffer::clear() {
	while (elementCount > 0)
		deleteElement();
}
void ExpressionBuffer::backspace() {
	// version 1
	/*if (getLastType() == ElementType::TBD_FUNC ||
		getLastType() == ElementType::PREFIX_FUNC ||
		getLastType() == ElementType::INFIX_FUNC ||
		getLastType() == ElementType::POSTFIX_FUNC)
		deleteElement();
	else if (getLastType() == ElementType::INT_NUM ||
		getLastType() == ElementType::FLOAT_NUM)
		deleteChar();
	
	if (!getLastHasDot())
		elements[elementsLength - 1].type = ElementType::INT_NUM;*/

	/* 타입에 따라 지우는 것이 문자/요소로 나뉜다
	* 문자 : 숫자, 콤마, 괄호, 종류 미정 함수
	* 요소 : 정의된 함수
	*/
	/*if (getCursorElementType() == TokenType::TBD_FUNC)
		deleteChar();
	else if (isElementFunc())
		deleteElement();
	else
		deleteChar();*/
	if (elementCount >= 1) // 전체 수식이 하나라도 요소가 있을 경우
		switch (getLastElementType()) {
		case ElementType::INT_NUM: // 숫자 및 선언되지 않은 함수. 한 글자만을 지운다.
		case ElementType::FLOAT_NUM:
		case ElementType::TBD_FUNC:
			deleteChar();
			break;
		default: // 함수 및 괄호 등. 전체를 지운다.
			deleteElement();
			break;
		}
	if (isElementNum() && !isNumberHasDot())
		elements[elementCount - 1].type = ElementType::INT_NUM;
}

//void Calculator::moveCursorL() {}
//void Calculator::moveCursorR() {}

void ExpressionBuffer::addNum(char num) {
	// version 1
	/*if (getLastType() == TokenType::INT_NUM ||
		getLastType() == TokenType::FLOAT_NUM);
	else if (elementsLength == 0 ||
		getLastType() == TokenType::NON ||
		getLastType() == TokenType::TBD_FUNC ||
		getLastType() == TokenType::PREFIX_FUNC ||
		getLastType() == TokenType::INFIX_FUNC)
		addElement(TokenType::INT_NUM);
	else
		return;

	if (elements[elementsLength - 1].str.length() == 1 &&
		elements[elementsLength - 1].str[0] == '0') {
		elements[elementsLength - 1].str.pop_back();
	}
	addChar(num);*/
	if (isElementNum());
	else if (isElementPrefix() || isElementInfix() || elementCount == 0)
		addElement(ElementType::INT_NUM);
	else
		return;

	if (elements[elementCount - 1].str == "0")
		elements[elementCount - 1].str.pop_back();
	
	addChar(num);
}
void ExpressionBuffer::addNum(string num) {
	// version 1
	/*if (getLastType() == TokenType::INT_NUM ||
		getLastType() == TokenType::FLOAT_NUM);
	else if (elementsLength == 0 ||
		getLastType() == TokenType::NON ||
		getLastType() == TokenType::TBD_FUNC ||
		getLastType() == TokenType::PREFIX_FUNC ||
		getLastType() == TokenType::INFIX_FUNC)
		addElement(TokenType::INT_NUM);
	else
		return;

	elements[elementsLength - 1].str = num;
	if (getLastHasDot())
		elements[elementsLength - 1].type = TokenType::FLOAT_NUM;*/
	if (isElementNum())
		return;
	else if (isElementPrefix() || isElementInfix() || elementCount == 0)
		addElement(ElementType::INT_NUM);
	else
		return;

	elements[elementCount - 1].str = num;
	if (isNumberHasDot())
		elements[elementCount - 1].type = ElementType::FLOAT_NUM;
}
void ExpressionBuffer::addDot() {
	// version 1
	/*if (getLastType() == TokenType::INT_NUM ||
		getLastType() == TokenType::FLOAT_NUM);
	else if (elementsLength == 0 ||
		getLastType() == TokenType::NON ||
		getLastType() == TokenType::TBD_FUNC ||
		getLastType() == TokenType::PREFIX_FUNC ||
		getLastType() == TokenType::INFIX_FUNC)
		addElement(TokenType::FLOAT_NUM);
	else
		return;

	if (elements[elementsLength - 1].str.length() == 0)
		addChar('0');
	if (!getLastHasDot())
		addChar('.');*/
	if (isElementNum())
		elements[elementCount - 1].type = ElementType::FLOAT_NUM;
	else if (isElementPrefix() || isElementInfix() || elementCount == 0)
		addElement(ElementType::FLOAT_NUM);
	else
		return;

	if (elements[elementCount - 1].str.length() == 0)
		addChar('0');
	if (!isNumberHasDot())
		addChar('.');
}

void ExpressionBuffer::addPrefixFunc(string func) {
	// version 1
	/*if (elementsLength == 0 ||
		getLastType() == TokenType::NON ||
		getLastType() == TokenType::TBD_FUNC ||
		getLastType() == TokenType::PREFIX_FUNC ||
		getLastType() == TokenType::INFIX_FUNC)
		addElement(TokenType::PREFIX_FUNC);
	else
		return;

	elements[elementsLength - 1].str = func;*/
	if (isElementPrefix() || isElementInfix() || elementCount == 0) {
		addElement(ElementType::PREFIX_FUNC);
		elements[elementCount - 1].str = func;
	}
}
void ExpressionBuffer::addInfixFunc(string func) {
	// version 1
	/*if (getLastType() == TokenType::INFIX_FUNC);
	else if (getLastType() == TokenType::TBD_FUNC ||
		getLastType() == TokenType::INT_NUM ||
		getLastType() == TokenType::FLOAT_NUM ||
		getLastType() == TokenType::POSTFIX_FUNC)
		addElement(TokenType::INFIX_FUNC);
	else
		return;

	elements[elementsLength - 1].str = func;*/
	if (isElementNum() || isElementPostfix()) {
		addElement(ElementType::INFIX_FUNC);
		elements[elementCount - 1].str = func;
	}
	else if (getLastElementType() == ElementType::INFIX_FUNC)
		elements[elementCount - 1].str = func;
}
void ExpressionBuffer::addPostfixFunc(string func) {
	// version 1
	/*if (getLastType() == TokenType::TBD_FUNC ||
		getLastType() == TokenType::INT_NUM ||
		getLastType() == TokenType::FLOAT_NUM ||
		getLastType() == TokenType::POSTFIX_FUNC)
		addElement(TokenType::POSTFIX_FUNC);
	else
		return;

	elements[elementsLength - 1].str = func;*/
	if (isElementNum() || isElementPostfix()) {
		addElement(ElementType::POSTFIX_FUNC);
		elements[elementCount - 1].str = func;
	}
}

void ExpressionBuffer::addLBracket() {
	if (isElementNum()) addInfixFunc("*");
	if (isElementPrefix() || isElementInfix() || elementCount == 0) {
		addElement(ElementType::L_BRACKET);
		addChar('(');
	}
}
void ExpressionBuffer::addRBracket() {
	if (isElementNum() || isElementPostfix()) {
		addElement(ElementType::R_BRACKET);
		addChar(')');
	}
}
void ExpressionBuffer::addComma() {
	if (isElementNum() || isElementPostfix()) {
		addElement(ElementType::COMMA);
		addChar(',');
	}
}
