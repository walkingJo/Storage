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
#include "Calculator.h"
#include "Field.h"
#include "Coord.h"

class CalculatorUI {
private:
	Calculator calculator;
	Coord cursor;
	bool isRunning = false;
	Field bufferedField[2] = {};
	short fieldIdx;
	string result;

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

		static unsigned char order;
		order = _getch();

		//아래의 규칙은 함수와 숫자가 완전히 별개인 경우에 성립한다
		//    1. 함수 이름에 숫자를 사용하지 않는다
		if ('0' <= order && order <= '9') calculator.addNum(order);
		else if (order == '.') calculator.addDot();
		else if (order == '+') calculator.addInfixFunc("+");
		else if (order == '-') calculator.addInfixFunc("-");
		else if (order == '*') calculator.addInfixFunc("*");
		else if (order == '/') calculator.addInfixFunc("/");
		else if ('A' <= order && order <= 'Z' || 'a' <= order && order <= 'z') {
			//calculator.addTBDChar(order) >> ?
		}
		//'('는 아무 현재로서는 아무 쓸모 없음
		//')'는 함수 인자 입력 시 마지막일 경우, spacebar와 동일한 역할을 하도록.
		//'}'역시 마지막이 후위 연산자 또는 숫자였을 경우, spacebar와 동일한 역할을 함
		//    다만 앞에 '{'가 하나는 있어야겠지
		//'{', '}'에 각각 전위, 후위 연산자의 속성을 부여하는 것은?
		//    어차피 연산을 할때도 스텍에 넣는 순서만 잘 챙기면 되는거니까!
		else if (order == '(') calculator.addLBracket();
		else if (order == ')') calculator.addRBracket();
		else if (order == ',') calculator.addComma();
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
			calculator.backspace();
		}
		else if (order == ENTER) result = calculator.calculate();
		else if (order == SPACE) {
			switch (cursor.x * 0x10 +
					cursor.y * 0x01) {
			case 0x30: calculator.clear(); break; // cls
			case 0x40: calculator.backspace(); break; // bsp

			case 0x25: calculator.addNum('0'); break; // 0
			case 0x14: calculator.addNum('1'); break; // 1
			case 0x24: calculator.addNum('2'); break; // 2
			case 0x34: calculator.addNum('3'); break; // 3
			case 0x13: calculator.addNum('4'); break; // 4
			case 0x23: calculator.addNum('5'); break; // 5
			case 0x33: calculator.addNum('6'); break; // 6
			case 0x12: calculator.addNum('7'); break; // 7
			case 0x22: calculator.addNum('8'); break; // 8
			case 0x32: calculator.addNum('9'); break; // 9
			case 0x35: calculator.addDot(); break; // .

			case 0x10: calculator.addNum("3.141592653589"); break; // pi
			case 0x20: calculator.addNum("2.718281828459"); break; // e

			case 0x41: calculator.addInfixFunc("/"); break; // /
			case 0x42: calculator.addInfixFunc("*"); break; // *
			case 0x43: calculator.addInfixFunc("-"); break; // -
			case 0x44: calculator.addInfixFunc("+"); break; // +

			case 0x00: calculator.addInfixFunc("^"); calculator.addNum('2'); break; // x^2
			case 0x01: calculator.addInfixFunc("^"); break; // x^y

			case 0x02: break; // ?
			case 0x03: break; // ?

			case 0x04: calculator.addPrefixFunc("log"); break; // log
			case 0x05: calculator.addPrefixFunc("ln"); break; // ln

			case 0x15: break; // +/-    << 이거 어떻게 구현하냐
			case 0x31: calculator.addPostfixFunc("!"); break; // !

			case 0x11: calculator.addLBracket(); break; // (
			case 0x21: calculator.addRBracket(); break; // )

			case 0x45: result = calculator.calculate(); break; // =
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
		bufferedField[fieldIdx].setExpression(calculator.getTotalExpression());
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
		calculator = Calculator();
		cursor = Coord(0, 0);
		isRunning = true;
		bufferedField[0] = Field();
		bufferedField[1] = Field();
		fieldIdx = 0;
		result = "";

		bufferedField[fieldIdx].print(cursor);
	}
	~CalculatorUI() {
		bufferedField[0].~Field();
		bufferedField[1].~Field();
		calculator.~Calculator();
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