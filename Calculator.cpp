#include "Calculator.h"
#include <cassert>

//ExpressionType Calculator::getCursorElementType() {
//	return tokens[tokenCursor].type;
//}
ExpressionType Calculator::getLastElementType() {
	return tokens[tokenCount - 1].type;
}

void Calculator::addChar(char c) {
	//tokens[tokenCursor].str.insert(stringCursor, str);
	tokens[tokenCount - 1].str += c;
}
// 커서 위치의 char를 삭제한다
//  만약 삭제한 char가 해당 element의 마지막이었다면(첫 번째 문자였다면) 해당 element까지 삭제한다
/* 지울 때 빈 공간에 '_'를 남길 것인가? */
void Calculator::deleteChar() {
	if (tokenCount) {
		if (tokens[tokenCount - 1].str.length() > 0)
			tokens[tokenCount - 1].str.pop_back();
		if (tokens[tokenCount - 1].str.length() == 0)
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
void Calculator::addElement(ExpressionType type) {
	// version 1
	/*elementsLength++;
	elements[elementsLength - 1].type = type;
	elements[elementsLength - 1].str = "";*/
	if (tokenCount != 0 && tokens[tokenCount - 1].str.back() == '.') {
		deleteChar();
		tokens[tokenCount - 1].type = ExpressionType::INT_NUM;
	}
	tokenCount++;
	tokens[tokenCount - 1].type = type;
	tokens[tokenCount - 1].str = "";
}
// 해당 element 자체를 완전히 밀어버리는 개념
void Calculator::deleteElement() {
	// version 1
	/*if (elementsLength >= 1) {
		elements[elementsLength - 1].type = TokenType::NON;
		elements[elementsLength - 1].str = "";
		elementsLength--;
	}*/
	if (tokenCount >= 1) {
		tokens[tokenCount - 1].type = ExpressionType::NON;
		tokens[tokenCount - 1].str = "";
		tokenCount--;
	}
}
bool Calculator::doesNumberHasDot() {
	assert(isElementNum());
	if (tokens[tokenCount - 1].str.find('.') != string::npos)
		return true;
	else
		return false;
}

bool Calculator::isElementNum() {
	switch (tokens[tokenCount-1].type) {
	case ExpressionType::INT_NUM:
	case ExpressionType::FLOAT_NUM:
		return true;
	default:
		return false;
	}
}
bool Calculator::isElementPrefix() {
	switch (getLastElementType()) {
	case ExpressionType::L_BRACKET:
	case ExpressionType::PREFIX_FUNC:
		return true;
	default:
		return false;
	}
}
bool Calculator::isElementInfix() {
	switch (getLastElementType()) {
	case ExpressionType::COMMA:
	case ExpressionType::TBD_FUNC:
	case ExpressionType::INFIX_FUNC:
		return true;
	default:
		return false;
	}
}
bool Calculator::isElementPostfix() {
	switch (getLastElementType()) {
	case ExpressionType::R_BRACKET:
	case ExpressionType::POSTFIX_FUNC:
		return true;
	default:
		return false;
	}
}
bool Calculator::isExpressionEmpty() { // 이거 특별히 쓸데가 있는 함수인가?
	if (tokens[tokenCount - 1].str.length() <= 0)
		return true;
	else
		return false;
}

Calculator::Calculator() {
	// version 1
	/*elements = new Token[maxToken];
	for (int i = 0; i < maxToken; ++i)
		elements[i] = Token();
	elementsLength = 0;

	elementCursor = 0;
	stringCursor = 0;*/
	tokens = new Expression[maxToken];
	for (int i = 0; i < maxToken; ++i)
		tokens[i] = Expression();
	tokenCount = 0;
}
string Calculator::calculate() {
	string result = "not yet";
	return result;
}
string Calculator::getTotalExpression() {
	string result = "";
	for (int i = 0; i < tokenCount; ++i) {
		result += tokens[i].str;
	}
	return result;
}

void Calculator::clear() {
	while (tokenCount > 0)
		deleteElement();
}
void Calculator::backspace() {
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
	if (tokenCount >= 1) // 전체 수식이 하나라도 요소가 있을 경우
		switch (getLastElementType()) {
		case ExpressionType::INT_NUM: // 숫자 및 선언되지 않은 함수. 한 글자만을 지운다.
		case ExpressionType::FLOAT_NUM:
		case ExpressionType::TBD_FUNC:
			deleteChar();
			break;
		default: // 함수 및 괄호 등. 전체를 지운다.
			deleteElement();
			break;
		}
	if (isElementNum() && !doesNumberHasDot())
		tokens[tokenCount - 1].type = ExpressionType::INT_NUM;
}

//void Calculator::moveCursorL() {}
//void Calculator::moveCursorR() {}

void Calculator::addNum(char num) {
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
	else if (isElementPrefix() || isElementInfix() || tokenCount == 0)
		addElement(ExpressionType::INT_NUM);
	else
		return;

	if (tokens[tokenCount - 1].str == "0")
		tokens[tokenCount - 1].str.pop_back();
	
	addChar(num);
}
void Calculator::addNum(string num) {
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
	else if (isElementPrefix() || isElementInfix() || tokenCount == 0)
		addElement(ExpressionType::INT_NUM);
	else
		return;

	tokens[tokenCount - 1].str = num;
	if (doesNumberHasDot())
		tokens[tokenCount - 1].type = ExpressionType::FLOAT_NUM;
}
void Calculator::addDot() {
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
	if (isElementNum());
	else if (isElementPrefix() || isElementInfix() || tokenCount == 0)
		addElement(ExpressionType::FLOAT_NUM);
	else
		return;

	if (tokens[tokenCount - 1].str.length() == 0)
		addChar('0');
	if (!doesNumberHasDot())
		addChar('.');
}

void Calculator::addPrefixFunc(string func) {
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
	if (isElementPrefix() || isElementInfix() || tokenCount == 0) {
		addElement(ExpressionType::PREFIX_FUNC);
		tokens[tokenCount - 1].str = func;
	}
}
void Calculator::addInfixFunc(string func) {
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
		addElement(ExpressionType::INFIX_FUNC);
		tokens[tokenCount - 1].str = func;
	}
	else if (getLastElementType() == ExpressionType::INFIX_FUNC)
		tokens[tokenCount - 1].str = func;
}
void Calculator::addPostfixFunc(string func) {
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
		addElement(ExpressionType::POSTFIX_FUNC);
		tokens[tokenCount - 1].str = func;
	}
}

void Calculator::addLBracket() {
	if (isElementPrefix() || isElementInfix() || tokenCount == 0) {
		addElement(ExpressionType::L_BRACKET);
		addChar('(');
	}
}
void Calculator::addRBracket() {
	if (isElementNum() || isElementPostfix()) {
		addElement(ExpressionType::R_BRACKET);
		addChar(')');
	}
}
void Calculator::addComma() {
	if (isElementNum() || isElementPostfix()) {
		addElement(ExpressionType::COMMA);
		addChar(',');
	}
}
