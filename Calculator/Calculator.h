#pragma once
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <vector>
#include <stack>
#include <cmath>
#include "ExpressionBuffer.h"

using std::vector;
using std::stack;

class Expression {
public:
	ElementType type;
	union _Value {
		long long iValue;
		double fValue;
		char funcID;
	} value;
	vector<Expression> lowerExpr = vector<Expression>();
public:
	Expression(Element element) {
		const string exprs[] = {
			"+", "-", "*", "/",
			"^", "!",
			"log", "ln",
		};
			
		type = element.type;
		value.iValue = 0;
		switch (type) {
		case ElementType::INT_NUM:		value.iValue = std::atoi(element.str.c_str()); break;
		case ElementType::FLOAT_NUM:	value.fValue = std::atof(element.str.c_str()); break;
		default:
			for (int i = 0; i < sizeof(exprs) / sizeof(string); ++i)
				if (exprs[i] == element.str)
					value.funcID = i;
			value.funcID = EOF;
		} 
	}
	Expression() {
		type = ElementType::NON;
		value.iValue = NULL;
	}

	void addExpression(Expression expr) {
		lowerExpr.push_back(expr);
	}
	static Expression elementaryArithmetic(char aritChar, Expression expr1, Expression expr2) {
		Expression temp = Expression();
		temp.type = ElementType::FLOAT_NUM;

		if (expr1.type == ElementType::INT_NUM && expr2.type == ElementType::INT_NUM) {
			temp.type = ElementType::INT_NUM;
			switch (aritChar) {
			case '+': temp.value.iValue = expr1.value.iValue + expr2.value.iValue; break;
			case '-': temp.value.iValue = expr1.value.iValue - expr2.value.iValue; break;
			case '*': temp.value.iValue = expr1.value.iValue * expr2.value.iValue; break;
			case '/': temp.value.iValue = expr1.value.iValue / expr2.value.iValue; break;
			}
		}
		else if (expr1.type == ElementType::FLOAT_NUM && expr2.type == ElementType::INT_NUM)
			switch (aritChar) {
			case '+': temp.value.fValue = expr1.value.fValue + expr2.value.iValue; break;
			case '-': temp.value.fValue = expr1.value.fValue - expr2.value.iValue; break;
			case '*': temp.value.fValue = expr1.value.fValue * expr2.value.iValue; break;
			case '/': temp.value.fValue = expr1.value.fValue / expr2.value.iValue; break;
			}
		else if (expr1.type == ElementType::INT_NUM && expr2.type == ElementType::FLOAT_NUM)
			switch (aritChar) {
			case '+': temp.value.fValue = expr1.value.iValue + expr2.value.fValue; break;
			case '-': temp.value.fValue = expr1.value.iValue - expr2.value.fValue; break;
			case '*': temp.value.fValue = expr1.value.iValue * expr2.value.fValue; break;
			case '/': temp.value.fValue = expr1.value.iValue / expr2.value.fValue; break;
			}
		else if (expr1.type == ElementType::FLOAT_NUM && expr2.type == ElementType::FLOAT_NUM)
			switch (aritChar) {
			case '+': temp.value.fValue = expr1.value.fValue + expr2.value.fValue; break;
			case '-': temp.value.fValue = expr1.value.fValue - expr2.value.fValue; break;
			case '*': temp.value.fValue = expr1.value.fValue * expr2.value.fValue; break;
			case '/': temp.value.fValue = expr1.value.fValue / expr2.value.fValue; break;
			}
		return temp;
	}
	static Expression exp(Expression base, Expression exponent) {
		Expression temp = Expression();
		temp.type = ElementType::FLOAT_NUM;

		if (base.type == ElementType::INT_NUM && exponent.type == ElementType::INT_NUM)
			temp.value.fValue = pow(base.value.iValue, exponent.value.iValue);
		else if (base.type == ElementType::INT_NUM && exponent.type == ElementType::FLOAT_NUM)
			temp.value.fValue = pow(base.value.iValue, exponent.value.fValue);
		else if (base.type == ElementType::FLOAT_NUM && exponent.type == ElementType::INT_NUM)
			temp.value.fValue = pow(base.value.fValue, exponent.value.iValue);
		else if (base.type == ElementType::FLOAT_NUM && exponent.type == ElementType::FLOAT_NUM)
			temp.value.fValue = pow(base.value.fValue, exponent.value.fValue);
		return temp;
	}
	static Expression exprFactorial(Expression n) {
		Expression temp = Expression();
		temp.type = ElementType::FLOAT_NUM;

		if (n.type == ElementType::INT_NUM)
			temp.value.fValue = tgamma(n.value.iValue + 1);
		else if (n.type == ElementType::FLOAT_NUM)
			temp.value.fValue = tgamma(n.value.fValue + 1);
		return temp;
	}
	static Expression exprLog(Expression base, Expression value) {
		Expression temp = Expression();
		temp.type = ElementType::FLOAT_NUM;

		if (base.type == ElementType::INT_NUM && value.type == ElementType::INT_NUM)
			temp.value.fValue = log(value.value.iValue) / log(base.value.iValue);
		else if (base.type == ElementType::INT_NUM && value.type == ElementType::FLOAT_NUM)
			temp.value.fValue = log(value.value.iValue) / log(base.value.fValue);
		else if (base.type == ElementType::FLOAT_NUM && value.type == ElementType::INT_NUM)
			temp.value.fValue = log(value.value.fValue) / log(base.value.iValue);
		else if (base.type == ElementType::FLOAT_NUM && value.type == ElementType::FLOAT_NUM)
			temp.value.fValue = log(value.value.fValue) / log(base.value.fValue);
		return temp;
	}
	static Expression exprLogE(Expression value) {
		Expression temp = Expression();
		temp.type = ElementType::FLOAT_NUM;

		if (value.type == ElementType::INT_NUM)
			temp.value.fValue = log(value.value.iValue) / log(2.7182818284590452);
		else if (value.type == ElementType::FLOAT_NUM)
			temp.value.fValue = log(value.value.fValue) / log(2.7182818284590452);
		return temp;
	}
	Expression /*!*/calculate() {
		/*
		* 계산방식 1:
		*		인자들을 모두 계산하고, switch문 내부에서 따로 또 계산
		* 계산방식 2:
		*		calculate가 값을 반환하게 만들어 수식 내부에서 실행 및 계산
		*/
		for (int i = 0; i < lowerExpr.size(); ++i) {
			if (lowerExpr[i].type == ElementType::TBD_FUNC &&
				lowerExpr[i].type == ElementType::PREFIX_FUNC &&
				lowerExpr[i].type == ElementType::INFIX_FUNC &&
				lowerExpr[i].type == ElementType::POSTFIX_FUNC)
				lowerExpr[i].calculate();
		}
		
		//값 계산
		Expression temp;
		switch (value.funcID) {
		case 0: temp = elementaryArithmetic('+', lowerExpr[0], lowerExpr[1]); break; // +
		case 1: temp = elementaryArithmetic('-', lowerExpr[0], lowerExpr[1]); break; // -
		case 2: temp = elementaryArithmetic('*', lowerExpr[0], lowerExpr[1]); break; // *
		case 3: temp = elementaryArithmetic('/', lowerExpr[0], lowerExpr[1]); break; // /
		
		case 4: exp(lowerExpr[0], lowerExpr[1]); break; // ^
		case 5: exprFactorial(lowerExpr[0]); break; // !
		
		case 6: exprLog(lowerExpr[0], lowerExpr[1]); break; // log
		case 7: exprLogE(lowerExpr[0]); break; // ln
		}
		//하위 식들 소멸
		lowerExpr.~vector();

		return *this;
	}
};

class Calculator {
private:
	vector<Element> elements;

	void setElementIVal(int idx, long long int value) {
		elements[idx].type = ElementType::INT_NUM;
		char* buff = new char[32];
		sprintf_s(buff, 32, "%lld", value);
		elements[idx].str = buff;
		delete[] buff;
	}
	void setElementFVal(int idx, double value) {
		elements[idx].type = ElementType::FLOAT_NUM;
		char* buff = new char[32];
		sprintf_s(buff, 32, "%lf", value);
		elements[idx].str = buff;
		delete[] buff;
	}
	void eraseBrackets(int lBracketIdx) {
		for (int i = lBracketIdx; i < elements.size(); i++) {
			switch (elements[i].type) {
			case ElementType::L_BRACKET:
			case ElementType::COMMA:
				elements.erase(elements.begin() + i);
				break;
			case ElementType::R_BRACKET:
				elements.erase(elements.begin() + i);
				return;
			}
		}
	}
	void calculate(int lBracketIdx, int rBracketIdx) {
		for (int i = lBracketIdx; i <= rBracketIdx; ++i) {
			if (elements[i].str == "log") {
				if (elements[i + 1].type == ElementType::INT_NUM &&
					elements[i + 2].type == ElementType::INT_NUM)
					setElementFVal(i,
						log(std::atoi(elements[i + 2].str.c_str())) /
						log(std::atoi(elements[i + 1].str.c_str())));
				else if (elements[i + 1].type == ElementType::INT_NUM &&
					elements[i + 2].type == ElementType::FLOAT_NUM)
					setElementFVal(i,
						log(std::atof(elements[i + 2].str.c_str())) / 
						log(std::atoi(elements[i + 1].str.c_str())));
				else if (elements[i + 1].type == ElementType::FLOAT_NUM &&
					elements[i + 2].type == ElementType::INT_NUM)
					setElementFVal(i, 
						log(std::atoi(elements[i + 2].str.c_str())) / 
						log(std::atof(elements[i + 1].str.c_str())));
				else if (elements[i + 1].type == ElementType::FLOAT_NUM &&
					elements[i + 2].type == ElementType::FLOAT_NUM)
					setElementFVal(i, 
						log(std::atof(elements[i + 2].str.c_str())) / 
						log(std::atof(elements[i + 1].str.c_str())));
				elements.erase(elements.begin() + i + 1);
				elements.erase(elements.begin() + i + 1); rBracketIdx -= 2;
			}
			else if (elements[i].str == "ln") {
				if (elements[i + 1].type == ElementType::INT_NUM)
					setElementFVal(i, 
						log(std::atoi(elements[i + 1].str.c_str())) / 
						log(2.7182818284590452));
				else if (elements[i + 1].type == ElementType::FLOAT_NUM)
					setElementFVal(i, 
						log(std::atof(elements[i + 1].str.c_str())) / 
						log(2.7182818284590452));
				elements.erase(elements.begin() + i + 1); rBracketIdx--;
			}
		}
		for (int i = lBracketIdx; i <= rBracketIdx; ++i) {
			if (elements[i].str == "^") {
				if (elements[i - 1].type == ElementType::INT_NUM && 
					elements[i + 1].type == ElementType::INT_NUM)
					setElementFVal(i, 
						pow(std::atoi(elements[i - 1].str.c_str()), 
							std::atoi(elements[i + 1].str.c_str())));
				else if (elements[i - 1].type == ElementType::FLOAT_NUM && 
					elements[i + 1].type == ElementType::INT_NUM)
					setElementFVal(i,
						pow(std::atof(elements[i - 1].str.c_str()),
							std::atoi(elements[i + 1].str.c_str())));
				else if (elements[i - 1].type == ElementType::INT_NUM &&
					elements[i + 1].type == ElementType::FLOAT_NUM)
					setElementFVal(i,
						pow(std::atoi(elements[i - 1].str.c_str()), 
							std::atof(elements[i + 1].str.c_str())));
				else if (elements[i - 1].type == ElementType::FLOAT_NUM &&
					elements[i + 1].type == ElementType::FLOAT_NUM)
					setElementFVal(i,
						pow(std::atof(elements[i - 1].str.c_str()),
							std::atof(elements[i + 1].str.c_str())));
				elements.erase(elements.begin() + i + 1);
				elements.erase(elements.begin() + i - 1); rBracketIdx -= 2;
			}
			else if (elements[i].str == "!") {
				if (elements[i - 1].type == ElementType::INT_NUM)
					setElementFVal(i, tgamma(std::atoi(elements[i - 1].str.c_str()) + 1));
				else if (elements[i].type == ElementType::FLOAT_NUM)
					setElementFVal(i, tgamma(std::atof(elements[i - 1].str.c_str()) + 1));
				elements.erase(elements.begin() + i - 1); rBracketIdx--;
			}
		}
		for (int i = lBracketIdx; i <= rBracketIdx; ++i) {
			if (elements[i].str == "+") {
				if (elements[i - 1].type == ElementType::INT_NUM &&
					elements[i + 1].type == ElementType::INT_NUM)
					setElementIVal(i,
						std::atoi(elements[i - 1].str.c_str()) +
						std::atoi(elements[i + 1].str.c_str()));
				else if (elements[i - 1].type == ElementType::FLOAT_NUM &&
					elements[i + 1].type == ElementType::INT_NUM)
					setElementFVal(i,
						std::atof(elements[i - 1].str.c_str()) +
						std::atoi(elements[i + 1].str.c_str()));
				else if (elements[i - 1].type == ElementType::INT_NUM &&
					elements[i + 1].type == ElementType::FLOAT_NUM)
					setElementFVal(i,
						std::atoi(elements[i - 1].str.c_str()) +
						std::atof(elements[i + 1].str.c_str()));
				else if (elements[i - 1].type == ElementType::FLOAT_NUM &&
					elements[i + 1].type == ElementType::FLOAT_NUM)
					setElementFVal(i,
						std::atof(elements[i - 1].str.c_str()) + 
						std::atof(elements[i + 1].str.c_str()));
				elements.erase(elements.begin() + i + 1);
				elements.erase(elements.begin() + i - 1); rBracketIdx -= 2;
			}
			else if (elements[i].str == "-") {
				if (elements[i - 1].type == ElementType::INT_NUM &&
					elements[i + 1].type == ElementType::INT_NUM)
					setElementIVal(i,
						std::atoi(elements[i - 1].str.c_str()) -
						std::atoi(elements[i + 1].str.c_str()));
				else if (elements[i - 1].type == ElementType::FLOAT_NUM &&
					elements[i + 1].type == ElementType::INT_NUM)
					setElementFVal(i,
						std::atof(elements[i - 1].str.c_str()) -
						std::atoi(elements[i + 1].str.c_str()));
				else if (elements[i - 1].type == ElementType::INT_NUM &&
					elements[i + 1].type == ElementType::FLOAT_NUM)
					setElementFVal(i,
						std::atoi(elements[i - 1].str.c_str()) -
						std::atof(elements[i + 1].str.c_str()));
				else if (elements[i - 1].type == ElementType::FLOAT_NUM &&
					elements[i + 1].type == ElementType::FLOAT_NUM)
					setElementFVal(i,
						std::atof(elements[i - 1].str.c_str()) -
						std::atof(elements[i + 1].str.c_str()));
				elements.erase(elements.begin() + i + 1);
				elements.erase(elements.begin() + i - 1); rBracketIdx -= 2;
			}
			else if (elements[i].str == "*") {
				if (elements[i - 1].type == ElementType::INT_NUM &&
					elements[i + 1].type == ElementType::INT_NUM)
					setElementIVal(i,
						std::atoi(elements[i - 1].str.c_str()) *
						std::atoi(elements[i + 1].str.c_str()));
				else if (elements[i - 1].type == ElementType::FLOAT_NUM &&
					elements[i + 1].type == ElementType::INT_NUM)
					setElementFVal(i,
						std::atof(elements[i - 1].str.c_str()) *
						std::atoi(elements[i + 1].str.c_str()));
				else if (elements[i - 1].type == ElementType::INT_NUM &&
					elements[i + 1].type == ElementType::FLOAT_NUM)
					setElementFVal(i,
						std::atoi(elements[i - 1].str.c_str()) *
						std::atof(elements[i + 1].str.c_str()));
				else if (elements[i - 1].type == ElementType::FLOAT_NUM &&
					elements[i + 1].type == ElementType::FLOAT_NUM)
					setElementFVal(i,
						std::atof(elements[i - 1].str.c_str()) *
						std::atof(elements[i + 1].str.c_str()));
				elements.erase(elements.begin() + i + 1);
				elements.erase(elements.begin() + i - 1); rBracketIdx -= 2;
			}
			else if (elements[i].str == "/") {
				if (elements[i - 1].type == ElementType::INT_NUM &&
					elements[i + 1].type == ElementType::INT_NUM)
					setElementIVal(i,
						std::atoi(elements[i - 1].str.c_str()) /
						std::atoi(elements[i + 1].str.c_str()));
				else if (elements[i - 1].type == ElementType::FLOAT_NUM &&
					elements[i + 1].type == ElementType::INT_NUM)
					setElementFVal(i,
						std::atof(elements[i - 1].str.c_str()) /
						std::atoi(elements[i + 1].str.c_str()));
				else if (elements[i - 1].type == ElementType::INT_NUM &&
					elements[i + 1].type == ElementType::FLOAT_NUM)
					setElementFVal(i,
						std::atoi(elements[i - 1].str.c_str()) /
						std::atof(elements[i + 1].str.c_str()));
				else if (elements[i - 1].type == ElementType::FLOAT_NUM &&
					elements[i + 1].type == ElementType::FLOAT_NUM)
					setElementFVal(i,
						std::atof(elements[i - 1].str.c_str()) /
						std::atof(elements[i + 1].str.c_str()));
				elements.erase(elements.begin() + i + 1);
				elements.erase(elements.begin() + i - 1); rBracketIdx -= 2;
			}
		}
	}
public:
	Calculator(ExpressionBuffer* exprBuff) {
		elements = vector<Element>();
		for (int i = 0; i < exprBuff->elementCount; ++i) {
			elements.push_back(exprBuff->elements[i]);
		}
	}
	Calculator() {
		elements = vector<Element>();
	}

	Element calculate() {
		stack<int> lBracketIdx = stack<int>();

		int commaIdx = -1;
		for (int i = 0; i < elements.size(); ++i) {
			switch (elements[i].type) {
			case ElementType::L_BRACKET:
				lBracketIdx.push(i);
				commaIdx = -1;
				break;
			case ElementType::COMMA:
				commaIdx = i;
				break;
			case ElementType::R_BRACKET:
				if (commaIdx == EOF) { //괄호 안의 수식을 하나로 합칠 수 있는 경우
					calculate(lBracketIdx.top() + 1, i - 1);
				}
				else { //괄호 안의 수식을 두 개로 나눠야 하는 경우
					calculate(lBracketIdx.top() + 1, commaIdx);
					calculate(lBracketIdx.top() + 3, (lBracketIdx.top() + 3) + i - commaIdx - 1);
				}
				eraseBrackets(lBracketIdx.top());
				i = lBracketIdx.top(); //이런 거추장스러운 방식 대신, '('을 찾았을 때 ')'를 찾는,
									   //이중 반복문으로 계량하기 >> 생각해보면 이쪽이 다중인자에 대처하기 쉬울 것 같다
				lBracketIdx.pop();
				break;
			}
		}

		calculate(0, (int)elements.size() - 1);
		return elements[0];
	}
};

//class Calculator {
//private:
//	vector<Element> elements;
//public:
//	Calculator(ExpressionBuffer* exprBuff) {
//		elements = vector<Element>();
//		for (int i = 0; i < exprBuff->elementCount; ++i) {
//			elements.push_back(exprBuff->elements[i]);
//		}
//	}
//	Calculator() {
//		elements = vector<Element>();
//	}
//
//	vector<Expression> /*!*/getPackableArea() {
//		return vector<Expression>();
//	}
//	Expression /*!*/pack(vector<Expression> exprs) {
//		return Expression();
//	}
//	string calculate() {
//		//괄호 부분을 찾는다
//
//		//괄호 내부의 식들을 하나로 합친다.(pack() 사용)
//
//		//모두 합치고 괄호가 더 이상 남지 않았을 때,
//		//    그 Expression에 calculate()를 사용한다
//		return "";
//	}
//};

#endif // !CALCULATOR_H