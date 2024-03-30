package �׸���;

import java.awt.Color;
import java.awt.Container;
import java.awt.FlowLayout;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.attribute.BasicFileAttributes;

import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JColorChooser;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.SwingConstants;
import javax.swing.filechooser.FileNameExtensionFilter;

import �׸���.MyCanvas;
import �׸���.MyCanvas.DRAW_TYPE;

public class �׸��� extends JFrame implements ActionListener {
	private JMenuItem[] fileItems = new JMenuItem[5];
	private JMenuItem[] editItems = new JMenuItem[3];
	private JMenuItem[] toolItems = new JMenuItem[5];
	private JMenuItem[] figureItems = new JMenuItem[5];
	private JMenuItem[] sizeItems = new JMenuItem[3];
	private ShortcutButton[] shortcutButtons = new ShortcutButton[15];
	private MyCanvas canvas;
	private String filePath;
	private boolean fileSaved;
	
 	public �׸���() {
		super("�׸���"); // �����ϰ� �ִ� ������ �̸����� �ٲ� �� ����?
		initializeComponents();
	}
	
	private void initializeComponents() {
		Container c = getContentPane();
		c.setLayout(new BoxLayout(c, BoxLayout.Y_AXIS));
		
		JPanel shortcuts = new JPanel();
		shortcuts.setLayout(new BoxLayout(shortcuts, BoxLayout.X_AXIS));

		for (int i = 0; i < shortcutButtons.length; ++i) {
			shortcutButtons[i] = new ShortcutButton(i);
			shortcuts.add(shortcutButtons[i]);
		}
		c.add(shortcuts);
		
		canvas = new MyCanvas();
		canvas.set�׸���(this);
		c.add(canvas);

		filePath = "";
		fileSaved = true;
		
		constructMenu();
		
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setLocation(100, 100);
		setSize(720, 620);
		setVisible(true);
	}

	private void constructMenu() {
		JMenuBar bar = new JMenuBar();
		//����, ����, ����, ����, ũ��
		
		JMenu fileMenu = new JMenu("����(F)"); fileMenu.setMnemonic('F');
		String[] fileItemName = { "���� �����(N)", "����(O)", "����(S)", "�ٸ� �̸����� ����(A)", "�Ӽ�(E)" };
		char[] fileItemMnemonic = { 'N', 'O', 'S', 'A', 'E' };
		int[] fileItemShortcuts = { KeyEvent.VK_N, KeyEvent.VK_O, KeyEvent.VK_S, KeyEvent.VK_A, KeyEvent.VK_E };
		for(int i = 0; i < fileItems.length; ++i) {
			fileItems[i] = new JMenuItem(fileItemName[i]);
			fileItems[i].setMnemonic(fileItemMnemonic[i]);
			fileItems[i].setAccelerator(KeyStroke.getKeyStroke(fileItemShortcuts[i], ActionEvent.CTRL_MASK));
			fileItems[i].addActionListener(this);
			fileMenu.add(fileItems[i]);
		}
		bar.add(fileMenu);
		
		JMenu editMenu = new JMenu("����(E)"); editMenu.setMnemonic('E');
		String[] editItemName = { "�߶󳻱�(X)", "����(C)", "�ٿ��ֱ�(V)" };
		int[] editItemShortcuts = { KeyEvent.VK_X, KeyEvent.VK_C, KeyEvent.VK_V };
		for(int i = 0; i < editItems.length; ++i) {
			editItems[i] = new JMenuItem(editItemName[i], editItemShortcuts[i]);
			editItems[i].setAccelerator(KeyStroke.getKeyStroke(editItemShortcuts[i], ActionEvent.CTRL_MASK));
			editItems[i].addActionListener(this);
			editItems[i].setEnabled(false);
			editMenu.add(editItems[i]);
		}
		bar.add(editMenu);
		
		JMenu toolMenu = new JMenu("����(T)"); toolMenu.setMnemonic('T');
		String[] toolItemName = { "����(A)", "ä���(B)", "���찳(C)", "�� ����(D)", "�� ũ��" };
		char[] toolItemMnemonic = { 'A', 'B', 'C', 'D', 'E' };
		for(int i = 0; i < toolItems.length; ++i) {
			toolItems[i] = new JMenuItem(toolItemName[i]);
			toolItems[i].setMnemonic(toolItemMnemonic[i]);
			toolItems[i].addActionListener(this);
			toolMenu.add(toolItems[i]);
		}
		bar.add(toolMenu);
		
		JMenu figureMenu = new JMenu("����(P)"); figureMenu.setMnemonic('P');
		String[] figureName = { "����(A)", "�(B)", "�ﰢ��(C)", "�簢��(D)", "Ÿ��(E)" };
		char[] figureMnemonic = { 'A', 'B', 'C', 'D', 'E' };
		for(int i = 0; i < figureItems.length; ++i) {
			figureItems[i] = new JMenuItem(figureName[i]);
			figureItems[i].setMnemonic(figureMnemonic[i]);
			figureItems[i].addActionListener(this);
			figureMenu.add(figureItems[i]);
		}
		bar.add(figureMenu);
		
		JMenu sizeMenu = new JMenu("ũ��(S)"); sizeMenu.setMnemonic('S');
		String[] sizeName = { "ũ�� ����(S)", "����̱�(D)", "ȸ��(T)" };
		char[] sizeMnemonic = { 'S', 'D', 'T' };
		for (int i = 0; i < sizeItems.length; ++i) {
			sizeItems[i] = new JMenuItem(sizeName[i]);
			sizeItems[i].setMnemonic(sizeMnemonic[i]);
			sizeItems[i].addActionListener(this);
			sizeMenu.add(sizeItems[i]);
		}
		bar.add(sizeMenu);
		/*
		 * ũ�� ����(�����¿� �������� �ø���)
		 * ����̱�(�����, �ȼ�)
		 * ȸ��(��)
		 */
		
		setJMenuBar(bar);
	}

	public static void main(String[] args) {
		�׸��� app = new �׸���();
	}

	private int ruWantSave() { //�������� ����
		int response = JOptionPane.showConfirmDialog(null, "���� ������ �����Ͻðڽ��ϱ�?\n(�������� ������ ���� ������ �����˴ϴ�.)", "�׸���", JOptionPane.YES_NO_CANCEL_OPTION);
		return response;
	}
	private void newFile() {
		if (!fileSaved) {
			switch(ruWantSave()) {
			case JOptionPane.YES_OPTION:
				if (saveFile() != JFileChooser.APPROVE_OPTION) {
					//���⿡ ���� ��� ����(����ã������!!)
					return;
				}
				break;
			case JOptionPane.CANCEL_OPTION:
				return;
			}
		}
		canvas.reset();
		filePath = "";
		fileSaved = true;
	}
	private void openFile() {
		if (!fileSaved) {
			//Ž���⿡�� ��ҳ� �����⸦ ������ ��
			switch(ruWantSave()) {
			case JOptionPane.YES_OPTION:
				if (saveFile() != JFileChooser.APPROVE_OPTION) {
					//���⿡ ���� ��� ����(����ã������!!)
					return;
				}
				break;
			case JOptionPane.CANCEL_OPTION:
				return;
			}
		}
		
		JFileChooser jfc = new JFileChooser();
		FileNameExtensionFilter filter = new FileNameExtensionFilter("Image File Format (*.bmp, *.png)", "bmp", "png");
		jfc.setFileFilter(filter);
		int response = jfc.showOpenDialog(null);
		if (response == JFileChooser.APPROVE_OPTION) {
			File newFile = jfc.getSelectedFile();
			filePath = newFile.getPath();
			canvas.setNewImage(newFile);
			fileSaved = true;
		}
		else {
			JOptionPane.showConfirmDialog(null, "������ ã�� �� �����ϴ�.", "�׸���", JOptionPane.CLOSED_OPTION);
			return;
		}
		
		fileSaved = true;
		return;
	}
	private int saveFile() {
		int response;
		if (filePath != "") { // �׳� ���� ��ο� ����
			response = JFileChooser.APPROVE_OPTION;
			canvas.saveImage(new File(filePath));
		} else {
			response = saveNewFile();
		}

		if(response == JFileChooser.APPROVE_OPTION)
		fileSaved = true;
		return response;
	}
	private int saveNewFile() {
		int response;
		
		JFileChooser jfc = new JFileChooser();
		FileNameExtensionFilter filter = new FileNameExtensionFilter("Image File Format (*.bmp, *.png)", "bmp", "png");
		jfc.setFileFilter(filter);
		response = jfc.showSaveDialog(null);
		if (response == JFileChooser.APPROVE_OPTION) {
			File newFile = jfc.getSelectedFile();
			filePath = newFile.getPath();
			//���� �̸��� �Է��ؼ� ���� ����, ���� �ּҰ� ���������� ��ϵǴ��� Ȯ��
			canvas.saveImage(newFile);
		}
		else {
			JOptionPane.showConfirmDialog(null, "���� ��θ� ã�� �� �����ϴ�.", "�׸���", JOptionPane.CLOSED_OPTION);
			return response;
		}
		
		fileSaved = true;
		return response;
	}
	private void showAttribute() { //�Ӽ�(Ctrl+E | ���� Ư�� : ������ ����, ��ũ ũ��, �ػ� | �̹��� ũ�� : �ʺ�, ����)
		if (filePath == "") {
			JOptionPane.showConfirmDialog(null, "�̹����� ���Ϸ� ����Ǿ� ���� �ʾ� ������ �ҷ��� �� �����ϴ�.", "�׸���", JOptionPane.CLOSED_OPTION);
			return;
		}
		
		BasicFileAttributes attrb = null;
		Path path = Paths.get(filePath);
		try {
			attrb = Files.readAttributes(path, BasicFileAttributes.class);
		} catch(IOException e) {
			e.printStackTrace();
		}
		
		AttributeDialog ad = new AttributeDialog (this, attrb);
	}
	
	public void setCtrlXEnabled(boolean b) {
		editItems[0].setEnabled(b);
	}
	public void setCtrlCEnabled(boolean b) {
		editItems[1].setEnabled(b);
	}
	public void setCtrlVEnabled(boolean b) {
		editItems[2].setEnabled(b);
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		/*
		 * fileItems = 5
		 * editItems = 3 << ctrl+X, ctrl+C, ctrl+V
		 * toolItems = 5
		 * figureItems = 5
		 * sizeItems = 3 << �̰͵� ĵ������ ����� ����
		 */
		if (e.getSource() == fileItems[0]) newFile(); // �� ����
		else if (e.getSource() == fileItems[1]) openFile(); // ����
		else if (e.getSource() == fileItems[2]) saveFile(); // ����
		else if (e.getSource() == fileItems[3]) saveNewFile(); // �ٸ� �̸����� ����
		else if (e.getSource() == fileItems[4]) showAttribute(); // �Ӽ�
		
		else if (e.getSource() == editItems[0]) { canvas.ctrlX(); canvas.repaint(); }
		else if (e.getSource() == editItems[1]) { canvas.ctrlC(); canvas.repaint(); }
		else if (e.getSource() == editItems[2]) { canvas.ctrlV(); canvas.repaint(); }

		else if (e.getSource() == toolItems[0]) canvas.setDrawType(DRAW_TYPE.PENCIL);
		else if (e.getSource() == toolItems[1]) canvas.setDrawType(DRAW_TYPE.FILL);
		else if (e.getSource() == toolItems[2]) canvas.setDrawType(DRAW_TYPE.ERASE);
		else if (e.getSource() == toolItems[3]) canvas.setDrawType(DRAW_TYPE.EXTRACTOR);
		else if (e.getSource() == toolItems[4]) {
			PensizeDialog pDialog = new PensizeDialog(this);
		}

		else if (e.getSource() == figureItems[0]) canvas.setDrawType(DRAW_TYPE.LINE);
		else if (e.getSource() == figureItems[1]) canvas.setDrawType(DRAW_TYPE.CURVE0);
		else if (e.getSource() == figureItems[2]) canvas.setDrawType(DRAW_TYPE.TRIANGLE); 
		else if (e.getSource() == figureItems[3]) canvas.setDrawType(DRAW_TYPE.RECTANGLE);
		else if (e.getSource() == figureItems[4]) canvas.setDrawType(DRAW_TYPE.OVAL);

		else if (e.getSource() == sizeItems[0]) canvas.setDrawType(DRAW_TYPE.SCALE);
		else if (e.getSource() == sizeItems[1]) canvas.setDrawType(DRAW_TYPE.LEAN);
		else if (e.getSource() == sizeItems[2]) canvas.setDrawType(DRAW_TYPE.ROTATE);
	}

	public void setFileSaved(boolean fs) {
		fileSaved = fs;
	}

	class AttributeDialog extends JDialog {
		JTextArea textArea;
		
		AttributeDialog(JFrame frame, BasicFileAttributes attrb){
			super(frame, "�Ӽ�", true);
			/*
			 * ���� �� ���� �Ӽ� ���� :
			 *     �ػ�
			 *     �ʺ� & ����
			 */
			textArea = new JTextArea(
					"������ ���� �ð� :\t" + attrb.lastModifiedTime() + "\n" +
					"���� ũ�� :\t\t" + (attrb.size() / 1024.0f) + "KiB");
			textArea.setEditable(false);
			add(textArea);
			
			setLocation(300, 300);
			pack();
			setVisible(true);
		}
	}
	
	class PensizeDialog extends JDialog implements ActionListener {
		final String[] pensizes = { "1", "3", "10", "20", "35", "50" };
		JComboBox<String> penCombo= new JComboBox<String>(pensizes);
		JButton okButton = new JButton("ok");
		
		public PensizeDialog(JFrame frame) {
			super(frame, "�� ũ�� ����", true);
			setLayout(new FlowLayout());
			
			penCombo.setMaximumRowCount(6);
			penCombo.setSelectedIndex(1);
			add(penCombo);
			
			okButton.addActionListener(this);
			add(okButton);
			
			pack();
			setVisible(true);
		}

		@Override
		public void actionPerformed(ActionEvent e) {
			int pensize = Integer.parseInt(pensizes[penCombo.getSelectedIndex()]) + 1;
			canvas.setPensize(pensize);
			
			setVisible(false);
		}

	}
	
	class ShortcutButton extends JLabel implements MouseListener {
		private BufferedImage bImg;
		private int buttonId;
		
		ShortcutButton(){
			super();
			bImg = new BufferedImage(50, 50, BufferedImage.TYPE_INT_RGB);
			buttonId = 0;
			addMouseListener(this);
		}
		
		ShortcutButton(int index) {
			super("",
					new ImageIcon(new ImageIcon("./�׸��Ǿ�����/icon" + index + ".png").getImage().getScaledInstance(50, 50, Image.SCALE_DEFAULT)),
					SwingConstants.LEFT);
			bImg = new BufferedImage(50, 50, BufferedImage.TYPE_INT_RGB);
			buttonId = index;
			addMouseListener(this);
		}
		
		@Override
		public void mouseClicked(MouseEvent e) { //������ mouseClicked �̺�Ʈ. ��, �� ��ư�� id ���� �����Ѵ�.
			switch(buttonId) {
			case 0: newFile();	break; // �� ����
			case 1: openFile();	break; // ���� �ҷ�����
			case 2: saveFile();	break; // ���� ����
			case 3: canvas.setDrawType(DRAW_TYPE.AREA0);		break;
			case 4: canvas.setDrawType(DRAW_TYPE.PENCIL);		break;
			case 5: canvas.setDrawType(DRAW_TYPE.FILL);			break;
			case 6: canvas.setDrawType(DRAW_TYPE.ERASE);		break;
			case 7: canvas.setDrawType(DRAW_TYPE.EXTRACTOR);	break;
			case 8: canvas.setDrawType(DRAW_TYPE.LINE);			break;
			case 9: canvas.setDrawType(DRAW_TYPE.CURVE0);		break;
			case 10: canvas.setDrawType(DRAW_TYPE.TRIANGLE);	break;
			case 11: canvas.setDrawType(DRAW_TYPE.RECTANGLE);	break;
			case 12: canvas.setDrawType(DRAW_TYPE.OVAL);		break;
			case 13:
				Color c = JColorChooser.showDialog(null, "�� �� ����", canvas.getPenColor());
				canvas.setPenColor(c);
				break;
			case 14: canvas.setDrawType(DRAW_TYPE.SCALE);		break;
			}
		}
		@Override
		public void mousePressed(MouseEvent e) {
			setIcon(new ImageIcon(new ImageIcon("./�׸��Ǿ�����/icon" + buttonId + "_clicked.png").getImage().getScaledInstance(50, 50, Image.SCALE_DEFAULT)));
		}
		@Override
		public void mouseReleased(MouseEvent e) {
			setIcon(new ImageIcon(new ImageIcon("./�׸��Ǿ�����/icon" + buttonId + ".png").getImage().getScaledInstance(50, 50, Image.SCALE_DEFAULT)));
		}
		@Override
		public void mouseEntered(MouseEvent e) {}
		@Override
		public void mouseExited(MouseEvent e) {
			setIcon(new ImageIcon(new ImageIcon("./�׸��Ǿ�����/icon" + buttonId + ".png").getImage().getScaledInstance(50, 50, Image.SCALE_DEFAULT)));
		}
	}

}
