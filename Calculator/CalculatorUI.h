#pragma once
#ifndef CALCULATOR_UI_H
#define CALCULATOR_UI_H

#define ARR_HEAD 224
#define ENTER '\r'
#define SPACE ' '
#define ESC 27
#define BSP 8
#define FPS 60

#include <ctime>
#include <conio.h>
#include "ExpressionBuffer.h"
#include "Field.h"
#include "Coord.h"

class CalculatorUI {
private:
	ExpressionBuffer exprBuff;
	Coord cursor;

	Field bufferedField[2] = {};
	short fieldIdx;
	
	string result;
	unsigned char order;
	bool isRunning = false;

	void moveCursor(Coord coord) {
		/* calc : 6 * 5 */
		cursor += coord;
		if (cursor.x >= 5)
			cursor.x = 4;
		if (cursor.x < 0)
			cursor.x = 0;
		if (cursor.y >= 6)
			cursor.y = 5;
		if (cursor.y < 0)
			cursor.y = 0;
	}
	void GetUserInput() {
		if (!_kbhit())
			return;

		order = _getch();

		if (order != ARR_HEAD)
			result = "";

		if ('0' <= order && order <= '9') exprBuff.addNum(order);
		else if (order == '.') exprBuff.addDot();
		else if (order == '+') exprBuff.addInfixFunc("+");
		else if (order == '-') exprBuff.addInfixFunc("-");
		else if (order == '*') exprBuff.addInfixFunc("*");
		else if (order == '/') exprBuff.addInfixFunc("/");
		else if (order == '!') exprBuff.addPostfixFunc("!");
		else if ('A' <= order && order <= 'Z' || 'a' <= order && order <= 'z') {
			//calculator.addTBDChar(order) >> ?
		}
		//'('는 아무 현재로서는 아무 쓸모 없음
		//')'는 함수 인자 입력 시 마지막일 경우, spacebar와 동일한 역할을 하도록.
		//'}'역시 마지막이 후위 연산자 또는 숫자였을 경우, spacebar와 동일한 역할을 함
		//    다만 앞에 '{'가 하나는 있어야겠지
		//'{', '}'에 각각 전위, 후위 연산자의 속성을 부여하는 것은?
		//    어차피 연산을 할때도 스텍에 넣는 순서만 잘 챙기면 되는거니까!
		else if (order == '(') exprBuff.addLBracket();
		else if (order == ')') exprBuff.addRBracket();
		else if (order == ',') exprBuff.addComma();
		else if (order == ARR_HEAD) {
			static unsigned char order2;
			order2 = _getch();
			switch (order2) {
			case 72: moveCursor(Coord(0, -1)); break; //↑
			case 75: moveCursor(Coord(-1, 0)); break; //←
			case 80: moveCursor(Coord(0, +1)); break; //↓
			case 77: moveCursor(Coord(+1, 0)); break; //→

			// ↓↓ cursor move ↓↓
			case 141: break; //Ctrl+↑
			case 115: break; //Ctrl+←
			case 145: break; //Ctrl+↓
			case 116: break; //Ctrl+→

			case 83: break; //delete
			}
		}
		else if (order == BSP) {
			exprBuff.backspace();
		}
		else if (order == ENTER || order == '=') result = exprBuff.calculate();
		else if (order == SPACE) {
			switch (cursor.x * 0x10 +
					cursor.y * 0x01) {
			case 0x30: exprBuff.clear(); break; // cls
			case 0x40: exprBuff.backspace(); break; // bsp

			case 0x25: exprBuff.addNum('0'); break; // 0
			case 0x14: exprBuff.addNum('1'); break; // 1
			case 0x24: exprBuff.addNum('2'); break; // 2
			case 0x34: exprBuff.addNum('3'); break; // 3
			case 0x13: exprBuff.addNum('4'); break; // 4
			case 0x23: exprBuff.addNum('5'); break; // 5
			case 0x33: exprBuff.addNum('6'); break; // 6
			case 0x12: exprBuff.addNum('7'); break; // 7
			case 0x22: exprBuff.addNum('8'); break; // 8
			case 0x32: exprBuff.addNum('9'); break; // 9
			case 0x35: exprBuff.addDot(); break; // .

			case 0x10: exprBuff.addNum("3.141592653589"); break; // pi
			case 0x20: exprBuff.addNum("2.718281828459"); break; // e

			case 0x41: exprBuff.addInfixFunc("/"); break; // /
			case 0x42: exprBuff.addInfixFunc("*"); break; // *
			case 0x43: exprBuff.addInfixFunc("-"); break; // -
			case 0x44: exprBuff.addInfixFunc("+"); break; // +

			case 0x00: exprBuff.addInfixFunc("^"); exprBuff.addNum('2'); break; // x^2
			case 0x01: exprBuff.addInfixFunc("^"); break; // x^y

			case 0x02: break; // ?
			case 0x03: break; // ?

			case 0x04: exprBuff.addPrefixFunc("log"); break; // log
			case 0x05: exprBuff.addPrefixFunc("ln"); break; // ln

			case 0x15: break; // +/-    << 이거 어떻게 구현하냐
			case 0x31: exprBuff.addPostfixFunc("!"); break; // !

			case 0x11: exprBuff.addLBracket(); break; // (
			case 0x21: exprBuff.addRBracket(); break; // )

			case 0x45: result = exprBuff.calculate(); break; // =
			}
		}
		else if (' ' <= order && order <= '~') {
			//switch로 따지면 default인 경우.
			//어떠한 기능도 추가하지 않는다
		}
		else if (order == ESC) {
			isRunning = false;
			return;
		}
	}
	void Render() {
		short nextIdx = (fieldIdx + 1) % 2;
		bufferedField[fieldIdx].reset();
		bufferedField[fieldIdx].setExpression(exprBuff.getTotalExpression());
		bufferedField[fieldIdx].keyEntered(order);
		bufferedField[fieldIdx].setResult(result);
		bufferedField[fieldIdx].print(cursor, bufferedField[nextIdx]);
		fieldIdx = nextIdx;
	}
	void wait() {
		static clock_t oldClock = clock();
		while (oldClock + (1000.0f / FPS) > clock());
		oldClock = clock();
	}
public:
	CalculatorUI() {
		exprBuff = ExpressionBuffer();
		cursor = Coord(0, 0);
		bufferedField[0] = Field();
		bufferedField[1] = Field();
		fieldIdx = 0;
		result = "";
		order = ' ';
		isRunning = true;

		bufferedField[fieldIdx].print(cursor);
	}
	~CalculatorUI() {
		bufferedField[0].~Field();
		bufferedField[1].~Field();
		exprBuff.~ExpressionBuffer();
	}
	void run() {
		while (isRunning) {
			GetUserInput();
			Render();
			wait();
		}
		this->~CalculatorUI();
	}
};

#endif // !CALCULATOR_UI_H