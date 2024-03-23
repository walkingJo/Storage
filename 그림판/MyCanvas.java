package �׸���;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JPanel;

import java.lang.Math;
import java.util.LinkedList;
import java.util.Queue;

public class MyCanvas extends JPanel implements MouseListener, MouseMotionListener {
	private �׸��� �׸���app;
	private Point srcPt;
	private Point dstPt;
	private Point curvePt1, curvePt2;
	
	private BufferedImage img;
	private BufferedImage tempImg;
	private int imageType = BufferedImage.TYPE_INT_ARGB;
	public enum DRAW_TYPE {
		PENCIL, FILL, ERASE, EXTRACTOR, LINE,
		CURVE0, CURVE1, CURVE2,
		TRIANGLE, RECTANGLE, OVAL,
		
		AREA0, AREA1, //���� �巡�� Ÿ�Ը�
		CTRL_X, CTRL_C, CTRL_V,
		SCALE, LEAN, ROTATE,
		}
	private ImgArea area;// = new ImgArea(0, 0, imageType);
	private DRAW_TYPE drawType = DRAW_TYPE.PENCIL;
	private DRAW_TYPE oldDrawType = drawType;
	private Color penColor = Color.blue;
	private int pensize = 10;
	
	// ���⼭ ����� ȭ�鿡 �ݿ����� �ʴ���
	public void set�׸���(�׸��� �׸���app) {
		this.�׸���app = �׸���app;
	}
	public void reset() {
		img = new BufferedImage(750, 500, imageType);
		tempImg = new BufferedImage(750, 500, imageType);
		img.getGraphics().setColor(Color.green);
		img.getGraphics().fillRect(0, 0, 750, 500);
		repaint();
	}
	public MyCanvas() {
		addMouseListener(this);
		addMouseMotionListener(this);
		
		reset();
		
		setPreferredSize(new Dimension(750, 500));
	}

	@Override
	public void mouseDragged(MouseEvent e) {
		if (drawType == DRAW_TYPE.CURVE1)
			curvePt1 = new Point(e.getX(), e.getY());
		else if (drawType == DRAW_TYPE.CURVE2)
			curvePt2 = new Point(e.getX(), e.getY());
		else if (drawType == DRAW_TYPE.AREA0);
		else if (drawType == DRAW_TYPE.AREA1) {
			//area.set location
			dstPt = e.getLocationOnScreen();

			Point areaPt = area.getLocation();
			area.setLocation(new Point(
			        areaPt.x + dstPt.x - srcPt.x, 
			        areaPt.y + dstPt.y - srcPt.y));

			srcPt = dstPt;
		}
		else
			dstPt = new Point(e.getX(), e.getY());
		
		//�����̳� ��찳�� ��� : �����̳� ���찳�� ���� �������� ������ ���� �������� �������� �ǹǷ� srcPt ����
		if (drawType == DRAW_TYPE.PENCIL || drawType == DRAW_TYPE.ERASE) {
			draw(tempImg);
			srcPt = dstPt; //������ �ٽ� ���������� �����. ���� �������� �� �ִ� ����
		} else { //���ʵ� ���찳�� �̴� ��� : �����̳� ���찳�� ���� ���� ���(�׸��⸦ ����ģ ���)�� �ӽ� �̹����� �� �̹����� �ű��
			copyImg(img, tempImg); //tempImg�� ���� �̹���(img)�� �ʱ�ȭ��
			draw(tempImg);
		}
	}
	@Override
	public void mouseMoved(MouseEvent e) {}
	@Override
	public void mouseClicked(MouseEvent e) {}
	@Override
	public void mousePressed(MouseEvent e) {
		if (drawType != DRAW_TYPE.CURVE1 && drawType != DRAW_TYPE.CURVE2) {
			dstPt = srcPt = new Point(e.getX(), e.getY());
			if (drawType != DRAW_TYPE.CURVE0)
				copyImg(img, tempImg);
		}
		draw(tempImg);
	}
	@Override
	public void mouseReleased(MouseEvent e) {
		if (drawType == DRAW_TYPE.CURVE0)		drawType = DRAW_TYPE.CURVE1;
		else if (drawType == DRAW_TYPE.CURVE1)	drawType = DRAW_TYPE.CURVE2;
		else if (drawType == DRAW_TYPE.CURVE2) {drawType = DRAW_TYPE.CURVE0;
			copyImg(tempImg, img);
		} else if (drawType == DRAW_TYPE.AREA0) { //Ÿ���� ���� �巡��0�̶��, �巡���� ������ ������ ��
			//�̹��� ����
			area = new ImgArea();
			for (int col = area.getLocation().y; col < area.getLocation().y + area.getHeight(); ++col) {
				for (int row = area.getLocation().x; row < area.getLocation().x + area.getWidth(); ++row) {
					area.setRGB(col, row, img.getRGB(col, row));
					img.setRGB(col, row, 0);
				}
			}
			//�� img�� �ش� ���� ����
			drawType = DRAW_TYPE.AREA1;
		} else if (drawType == DRAW_TYPE.AREA1) {
			drawType = DRAW_TYPE.AREA0;
			copyImg(tempImg, img);
		}
		else
			copyImg(tempImg, img);
	}
	@Override
	public void mouseEntered(MouseEvent e) { }
	@Override
	public void mouseExited(MouseEvent e) { }
	
	private void draw(BufferedImage img) { //�̹��� ������ ���ڷ� �޾�, drawType�� �ش��ϴ� ������ �׸���.
		Graphics graphics = img.getGraphics();
		graphics.setColor(penColor);

		if (drawType == DRAW_TYPE.PENCIL)			drawPencil(graphics);
		else if (drawType == DRAW_TYPE.FILL)		fill(img, dstPt);
		else if (drawType == DRAW_TYPE.ERASE)		erase(graphics);
		else if (drawType == DRAW_TYPE.EXTRACTOR)	extractor(graphics);
		else if (drawType == DRAW_TYPE.LINE)		drawLine(graphics);
		else if (drawType == DRAW_TYPE.CURVE0) {
			drawLine(graphics);
			curvePt1 = srcPt;
			curvePt2 = dstPt;
		}
		else if (drawType == DRAW_TYPE.CURVE1)		drawCurve(graphics);
		else if (drawType == DRAW_TYPE.CURVE2)		drawCurve(graphics);

		else if (drawType == DRAW_TYPE.TRIANGLE)	drawTriangle(graphics);
		else if (drawType == DRAW_TYPE.RECTANGLE)	drawRectangle(graphics);
		else if (drawType == DRAW_TYPE.OVAL)		drawOval(graphics);

		else if (drawType == DRAW_TYPE.AREA0); //�ƹ��͵� �׸��� �ʴ´�
		else if (drawType == DRAW_TYPE.AREA1) {
			//�̹����� �̵��� ���� �׸���
		}
		
		else if (drawType == DRAW_TYPE.CTRL_X);
		else if (drawType == DRAW_TYPE.CTRL_C);
		else if (drawType == DRAW_TYPE.CTRL_V);
		
		else if (drawType == DRAW_TYPE.SCALE);
		else if (drawType == DRAW_TYPE.LEAN);
		else if (drawType == DRAW_TYPE.ROTATE);
		
		DRAW_TYPE editingTypes[] = { 
				DRAW_TYPE.CTRL_X   , DRAW_TYPE.CTRL_V   , 
				DRAW_TYPE.PENCIL   , DRAW_TYPE.FILL     ,
				DRAW_TYPE.ERASE    , DRAW_TYPE.LINE     ,
				DRAW_TYPE.CURVE0   , DRAW_TYPE.CURVE1   , DRAW_TYPE.CURVE2   ,
				DRAW_TYPE.TRIANGLE , DRAW_TYPE.RECTANGLE, DRAW_TYPE.OVAL     ,
				DRAW_TYPE.SCALE    , DRAW_TYPE.LEAN     , DRAW_TYPE.ROTATE   ,
				};
		for (DRAW_TYPE dt : editingTypes)
			if (drawType == dt)
				�׸���app.setFileSaved(false);
		
		// ���⿡�� Ŀ�� �̹��� �׸���
		this.getGraphics().drawImage(img, 0, 0, 750, 500, this);
	}
	
	private Point lerp(float t) {
		return lerp(srcPt, dstPt, t);
	}
	private Point lerp(Point src, Point dst, float t) {
		int x = (int) ((1 - t) * src.x + t * dst.x);
		int y = (int) ((1 - t) * src.y + t * dst.y);
		return new Point(x, y);
	}
	private void circle(Graphics graphics, Point centor) {
		graphics.fillOval(centor.x - pensize, centor.y - pensize, pensize, pensize);
	}
	private void line(Graphics graphics, Point src, Point dst) {
		double length = Math.sqrt(Math.pow(src.x - dst.x, 2) + Math.pow(src.y -dst.y, 2));
		for (float t = 0; t <= 1; t += 1 / length) {
			Point drawPoint = lerp(src, dst, t);
			circle(graphics, drawPoint);
		}
	}
	private void copyImg(BufferedImage srcImg, BufferedImage dstImg) {
		for (int h = 0; h < srcImg.getHeight(); ++h) {
			for (int w = 0; w < srcImg.getWidth(); ++w) {
				dstImg.setRGB(w, h, srcImg.getRGB(w, h));
			}
		}
	}
	
	private void drawPencil(Graphics graphics) {
		line(graphics, srcPt, dstPt);
	}
	private void fill(BufferedImage img, Point pt) {
		int ocolor = img.getRGB(pt.x, pt.y);
		int pcolor = penColor.getRGB();
		if (ocolor == pcolor)
			return;

		// DFS : depth first search
		// BFS : breadth first search
		Queue<Point> qu = new LinkedList<Point>();
		qu.add(pt);
		while (qu.size() > 0) {
			Point qPt = qu.remove();

			Point qPts[] = { new Point(qPt.x - 1, qPt.y), new Point(qPt.x + 1, qPt.y), new Point(qPt.x, qPt.y - 1), new Point(qPt.x, qPt.y + 1) };
			for (Point newPt : qPts) {
				if (0 <= newPt.x && newPt.x < 750 && 0 <= newPt.y && newPt.y < 500
						&& img.getRGB(newPt.x, newPt.y) == ocolor) {
					img.setRGB(newPt.x, newPt.y, pcolor);
					qu.add(newPt);
				}
			}
		}
	}
	private void erase(Graphics graphics) {
		if (imageType == BufferedImage.TYPE_BYTE_INDEXED);
		
		graphics.setColor(Color.white);
		for (float t = 0; t <= 1; t += 0.01f) {
			Point drawPoint = lerp(t);
			graphics.fillRect(drawPoint.x - pensize, drawPoint.y - pensize, 2 * pensize, 2 * pensize);
		}
	} // ���찳
	private void extractor(Graphics graphics) {
		int tempColor = img.getRGB(dstPt.x - 1, dstPt.y - 1);
		penColor = new Color(tempColor);
		drawType = oldDrawType;
	} // �� ����
	private void drawLine(Graphics graphics) {
		line(graphics, srcPt, dstPt);
	}
	private void drawCurve(Graphics graphics) {
		for (float t = 0.0f; t <= 1; t += 0.001f) {			
			Point lerpedPt11 = lerp(srcPt, curvePt1, t);
			Point lerpedPt12 = lerp(curvePt1, curvePt2, t);
			Point lerpedPt13 = lerp(curvePt2, dstPt, t);
			
			Point lerpedPt21 = lerp(lerpedPt11, lerpedPt12, t);
			Point lerpedPt22 = lerp(lerpedPt12, lerpedPt13, t);
			
			Point lerpedPt31 = lerp(lerpedPt21, lerpedPt22, t);
			
			circle(graphics, lerpedPt31);
		}
	}
	private void drawTriangle(Graphics graphics) { 
		Point pt1 = new Point((srcPt.x + dstPt.x) / 2, dstPt.y);
		Point pt2 = new Point(dstPt.x, srcPt.y);
		line(graphics, pt1, pt2);
		line(graphics, srcPt, pt1);
		line(graphics, srcPt, pt2);
	}
	private void drawRectangle(Graphics graphics) {
		Point pt1 = new Point(srcPt.x, dstPt.y);
		Point pt2 = new Point(dstPt.x, srcPt.y);
		line(graphics, srcPt, pt1);
		line(graphics, srcPt, pt2);
		line(graphics, dstPt, pt1);
		line(graphics, dstPt, pt2);
	}
	private void drawOval(Graphics graphics) {
		Point m = new Point((srcPt.x + dstPt.x) / 2, (srcPt.y + dstPt.y) / 2);
		float a = Math.abs(srcPt.x - m.x);
		float b = Math.abs(srcPt.y - m.y);
		for (int x = -(int) a; x <= (int) a; ++x) {
			int y = (int) (b * Math.sqrt(1 - x * x / a / a));
			circle(graphics, new Point(x + m.x, +y + m.y));
			circle(graphics, new Point(x + m.x, -y + m.y));
		}
		for (int y = -(int) b; y <= (int) b; ++y) {
			int x = +(int) (a * Math.sqrt(1 - y * y / b / b));
			circle(graphics, new Point(+x + m.x, y + m.y));
			circle(graphics, new Point(-x + m.x, y + m.y));
		}
	}
	
	public void setDrawType(DRAW_TYPE typenum) {
		oldDrawType = drawType;
		drawType = typenum;
	}
	public void setPenColor(Color color) {
		penColor = color;
	} // �� �� ����
	public Color getPenColor() {
		return penColor;
	} // �� �� ��ȯ
	public void setPensize(int size) {
		pensize = size;
	} // �� ũ�� ����
	public int getPensize() { return pensize; }
	
	public void setNewImage(File file) {
		reset();
		try {
			imageType = ImageIO.read(file).getType();
//			this.img = ImageIO.read(file);
			img.getGraphics().drawImage(ImageIO.read(file), 0, 0, this);
//			this.tempImg = ImageIO.read(file);
			tempImg.getGraphics().drawImage(ImageIO.read(file), 0, 0, this);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	public void saveImage(File file) {
		try {
			String fileName = file.getName();
			String extension = fileName.substring(fileName.lastIndexOf('.') + 1);
			ImageIO.write(img, extension, file);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		this.getGraphics().drawImage(img, 0, 0, 750, 500, this);
	}

	/*
	 * Ctrl+X
	 * Ctrl+C
	 * Ctrl+V
	 * 
	 * Ctrl+Z
	 * Ctrl+Y
	 * 
	 * ũ�� ����
	 * ����̱�
	 * Ȯ��
	 * 
	 * �巡�� :	tempImg
	 * ������ :	copy(img >> temp); tempImg
	 * ������ :	copy(temp >> img)
	 */
	class ImgArea extends BufferedImage {
		Point location;
		
		public ImgArea(int width, int height, int imageType) {
			super(width, height, imageType);
			location = new Point(0, 0);
		}
		public ImgArea() {
			super(Math.abs(srcPt.x - dstPt.x),
					Math.abs(srcPt.y - dstPt.y),
					imageType);
			location = new Point(srcPt.x < dstPt.x ? srcPt.x : dstPt.x, srcPt.y < dstPt.y ? srcPt.y : dstPt.y);
		}
		
		public Point getLocation() { return location; }
		public void setLocation(Point location) { this.location = location; }
	}
	
}
