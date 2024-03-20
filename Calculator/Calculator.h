#pragma once
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <vector>
#include "ExpressionBuffer.h"

using std::vector;

class Expression {
private:
	ElementType type;
	union _Value {
		long long iValue;
		double fValue;
		char funcID;
	} value;
	vector<Expression> lowerExpr;
public:
	Expression(Element element) {
		const string exprs[] = {
			"+", "-", "*", "/",
			"^", "!",
			"log", "ln",
		};
			
		type = element.type;
		value.iValue = 0;
		switch (element.type) {
		case ElementType::INT_NUM:		value.iValue = std::atoi(element.str.c_str()); break;
		case ElementType::FLOAT_NUM:	value.fValue = std::atof(element.str.c_str()); break;
		default:
			for (int i = 0; i < sizeof(exprs) / sizeof(string); ++i)
				if (exprs[i] == element.str)
					value.funcID = i;
			value.funcID = EOF;
			break;
		} 
	}
	Expression() {
		type = ElementType::NON;
		value.iValue = NULL;
	}

	void addExpression(Expression expr) {
		lowerExpr.push_back(expr);
	}
	void calculate() {
		/*
		* ����� 1:
		*		���ڵ��� ��� ����ϰ�, switch�� ���ο��� ���� �� ���
		* ����� 2:
		*		calculate�� ���� ��ȯ�ϰ� ����� ���� ���ο��� ���� �� ���
		*/
		for (int i = 0; i < lowerExpr.size(); ++i) {
			if (lowerExpr[i].type == ElementType::TBD_FUNC &&
				lowerExpr[i].type == ElementType::PREFIX_FUNC &&
				lowerExpr[i].type == ElementType::INFIX_FUNC &&
				lowerExpr[i].type == ElementType::POSTFIX_FUNC)
				lowerExpr[i].calculate();
		}
		
		//�� ���
		switch (value.funcID) {
		case 0: break; // +
		case 1: break; // -
		case 2: break; // *
		case 3: break; // /
		
		case 4: break; // ^
		case 5: break; // !
		
		case 6: break; // log
		case 7: break; // ln
		}
		//���� �ĵ� �Ҹ�
	}
};

class Calculator {
private:
public:
	//vector<Expression> expressions = vector<Expression>(100);
	//for (int i = 0; i < tokenCount; ++i) {
	//	expressions[i] = Expression(tokens[i]);
	//}
	//
	//bool whileFlag = true;
	//while (whileFlag) {
	//	//
	//}
};

#endif // !CALCULATOR_H