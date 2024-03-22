package calculator;

import java.awt.Container;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class calculator extends JFrame implements ActionListener {
	private Expression expression = new Expression();
	private JButton[] buttons = new JButton[20];
	
	public calculator() {
		super("계산기");
		initializeComponents();
	}
	
//	private class numberActionListener  {
//		@Override
//		public void actionPerformed(ActionEvent e) {
//			// 숫자키 입력
//		}
//	}
	
	private void initializeComponents() {
		Container c = getContentPane();
		BoxLayout mainLayout = new BoxLayout(c, BoxLayout.Y_AXIS);
		c.setLayout(mainLayout);
		
		c.add(expression);
		
		JPanel buttonPanel = new JPanel();
		buttonPanel.setLayout(new GridLayout(5, 4));
		c.add(buttonPanel);
		
		String[] buttonNames = {
				"C", "backspace", "%", "/",
				"7", "8", "9", "*",
				"4", "5", "6", "-",
				"1", "2", "3", "+",
				"?", "0", ".", "="
		};
		for (int i = 0; i < 20; ++i ) {
			buttons[i] = new JButton(buttonNames[i]);
			buttons[i].addActionListener(this);
			buttonPanel.add(buttons[i]);
		}
		// 사칙연산
		// 숫자
		// 기타 연산자
		
		pack();
		setLocation(300, 300);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setVisible(true);
	}
	
	public static void main(String[] args) {
		calculator app = new calculator();
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		// 숫자 키 확인
		int[] numberIndexes = { 4, 5, 6, 8, 9, 10, 12, 13, 14, 17 };
		for(int i : numberIndexes)
			if (e.getSource() == buttons[i]) {
				expression.addNumber(buttons[i].getText());
			}
		
		// 사칙연산 키 확인
		int[] ffaoIndexes = { 3, 7, 11, 15 };
		for(int i : ffaoIndexes)
			if (e.getSource() == buttons[i]) {
				expression.addOperator(buttons[i].getText());
			}
		
		// 기타 연산자 키 확인
		int[] etcIndexes = { 0, 1, 2, 16, 18, 19 };
		for(int i : etcIndexes)
			if(e.getSource() == buttons[i]) {
				switch(i) {
				case 0: // 전체 삭제
					expression.clear();
					break;
				case 1: // 백스페이스
					expression.backspace();
					break;
				case 2: // 퍼센트
					expression.addOperator("%");
					break;
				case 16: // 현재 기능 미배정
					break;
				case 18: // 소숫점 찍기
					expression.addDot();
					break;
				case 19: // 결과값 계산
					expression.carculate();
					break;
				}
			}
	}

}
