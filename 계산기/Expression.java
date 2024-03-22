package calculator;

import javax.swing.JOptionPane;
import javax.swing.JTextField;

public class Expression extends JTextField {
	private StringBuilder[] elements;
	private int elementIndex;

	public Expression() {
		elements = new StringBuilder[20];
		for (int i = 0; i < elements.length; ++i)
			elements[i] = new StringBuilder();
		elementIndex = 0;
	}

	private void refresh() {
		String buff = "";
		for (int i = 0; i <= elementIndex; ++i)
			buff += elements[i] + " ";
		this.setText(buff);
	}
	private void deleteLastElement() {
		elements[elementIndex].setLength(0);
		if (elementIndex > 0)
			elementIndex--;
	}
	private boolean isLastElementNumber() {
		if (elementIndex % 2 == 0)
			return true;
		else
			return false;
	}
	private boolean isLastElementOperator() {
		if (elementIndex % 2 == 1)
			return true;
		else
			return false;
	}
	public void clear() {
		int tempCount = elementIndex;
		for (int i = 0; i <= tempCount; ++i)
			deleteLastElement();
		refresh();
	}
	public void backspace() {
		if (isLastElementOperator()) {
			deleteLastElement();
		} else if (isLastElementNumber()) {
			elements[elementIndex].deleteCharAt(elements[elementIndex].length() - 1);
			if (elements[elementIndex].length() == 0)
				elementIndex--;
		}
		refresh();
	}
	// "0." 만을 입력하고 연산자를 입력하면 "0" 또는 "0.0"으로 바꿔야 하는데, 이것을 어떤 함수에 배정할 것인가...
	// >> addOperator 함수에 배정

	public void addNumber(String text) {
		if (isLastElementOperator())
			elementIndex++;
		elements[elementIndex].append(text);
		refresh();
	}
	public void addDot() {
		if (isLastElementOperator())
			elementIndex++;

		String str = "";
		if (elements[elementIndex].toString().contains("."))
			return;
		else if (elements[elementIndex].length() == 0)
			str = "0.";
		else
			str = ".";
		elements[elementIndex].append(str);
		refresh();

		// text 입력(문자열 변경 등) 신경쓰기
	}
	public void addOperator(String text) {
		if (elements[elementIndex].length() == 0 && elementIndex == 0)
			return;
		else if (isLastElementOperator()) {
			elements[elementIndex].setLength(0);
		} else if (isLastElementNumber()) {
			if (elements[elementIndex].charAt(elements[elementIndex].length() - 1) == '.')
				addNumber("0");
			elementIndex++;
		}

		elements[elementIndex].append(text);
		refresh();
	}

	private void move2block(String[] str, int srcIdx) {
		for (int i = srcIdx; i <= str.length - 3; ++i)
			str[i] = str[i + 2];
		for (int i = str.length - 2; i < str.length; ++i)
			str[i] = "";
	}
	public void carculate() {
		// 전체 식을 계산하여 str 수정하기.
		String[] buffs = new String[20];
		int buffIndex = elementIndex;
		for(int i = 0; i <= 19; ++i)
			buffs[i] = elements[i].toString();
		
		for (int flag = 1; flag <= 2; ++flag) {
			for (int i = 1; i <= buffIndex; i += 2) {
				String operator = buffs[i];
				double num1 = Double.parseDouble(buffs[i - 1]);
				double num2 = Double.parseDouble(buffs[i + 1]);
				
				switch (flag) {
				case 1:
					if (operator.equals("*"))
						buffs[i - 1] = Double.toString( num1 * num2 );
					else if (operator.equals("/"))
						buffs[i - 1] = Double.toString( num1 / num2 );
					else
						continue;
					break;
				case 2:
					if (operator.equals("+"))
						buffs[i - 1] = Double.toString( num1 + num2 );
					else if (operator.equals("-"))
						buffs[i - 1] = Double.toString( num1 - num2 );
					else
						continue;
					break;
				}
				
				// 연산 진행시 실행되는 코드
				move2block(buffs, i);
				buffIndex -= 2;
				i -= 2;
			}
		}

		clear();
		elements[0] = new StringBuilder(buffs[0]);
		refresh();
	}
}
