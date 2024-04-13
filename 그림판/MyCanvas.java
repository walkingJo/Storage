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
	private ImgArea area;
	private boolean canCtrlX = false;
	private boolean canCtrlC = false;
	private boolean canCtrlV = false;
	
	public enum DRAW_TYPE {
		PENCIL  , FILL     , ERASE , EXTRACTOR, LINE,
		CURVE0  , CURVE1   , CURVE2,
		TRIANGLE, RECTANGLE, OVAL  ,
		
		AREA0   , AREA1    , AREA9 , //���� �巡�� Ÿ�Ը�
		SCALE   , LEAN     , ROTATE,
	}
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
		img.getGraphics().setColor(Color.white);
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
			curvePt1 = e.getPoint();
		else if (drawType == DRAW_TYPE.CURVE2)
			curvePt2 = e.getPoint();
		else if (drawType == DRAW_TYPE.AREA1) {
			dstPt = e.getPoint();

			Point areaPt = area.getLocation();
			area.setLocation(new Point(
			        areaPt.x + dstPt.x - srcPt.x, 
			        areaPt.y + dstPt.y - srcPt.y));

			srcPt = dstPt;
		}
		else
			dstPt = e.getPoint();
		
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
			dstPt = srcPt = e.getPoint();
			if (drawType != DRAW_TYPE.CURVE0)
				copyImg(img, tempImg);
			/*
			 * if (drawType == DRAW_TYPE.AREA1 && !area.isInsideOfArea(e.getPoint()))
			 * drawType = DRAW_TYPE.AREA9;
			 */
			if (drawType == DRAW_TYPE.AREA1 && !area.isInsideOfArea(e.getPoint())) {
//				drawType = DRAW_TYPE.AREA9;
				draw(tempImg);
				copyImg(tempImg, img);
				drawType = DRAW_TYPE.AREA0;
			}
		}
		draw(tempImg);
	}
	@Override
	public void mouseReleased(MouseEvent e) {
		canCtrlX = false;
		canCtrlC = false;
		
		if (drawType == DRAW_TYPE.CURVE0)		drawType = DRAW_TYPE.CURVE1;
		else if (drawType == DRAW_TYPE.CURVE1)	drawType = DRAW_TYPE.CURVE2;
		else if (drawType == DRAW_TYPE.CURVE2) {drawType = DRAW_TYPE.CURVE0;
			copyImg(tempImg, img);
		} else if (drawType == DRAW_TYPE.AREA0) { //Ÿ���� ���� �巡��0�̶��, �巡���� ������ ������ ��
			if (srcPt.x != dstPt.x && srcPt.y != dstPt.y) {
				Point newSrcPt = clampPt(srcPt);
				Point newDstPt = clampPt(dstPt);
				area = new ImgArea(newSrcPt, newDstPt, img);
				//�� img�� �ش� ���� ����
				canCtrlX = true; 
				canCtrlC = true;
				drawType = DRAW_TYPE.AREA1;
			}
		} else if (drawType == DRAW_TYPE.AREA1) {
			drawType = DRAW_TYPE.AREA9;
			draw(tempImg);
			drawType = DRAW_TYPE.AREA1;
		} /*
			 * else if (drawType == DRAW_TYPE.AREA9) { drawType = DRAW_TYPE.AREA0;
			 * copyImg(tempImg, img); }
			 */
		else
			copyImg(tempImg, img);
		
		�׸���app.setCtrlXEnabled(canCtrlX);
		�׸���app.setCtrlCEnabled(canCtrlC);
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
			curvePt1 = srcPt;
			curvePt2 = dstPt;						drawCurve(graphics); }
		else if (drawType == DRAW_TYPE.CURVE1)		drawCurve(graphics);
		else if (drawType == DRAW_TYPE.CURVE2)		drawCurve(graphics);

		else if (drawType == DRAW_TYPE.TRIANGLE)	drawTriangle(graphics);
		else if (drawType == DRAW_TYPE.RECTANGLE)	drawRectangle(graphics);
		else if (drawType == DRAW_TYPE.OVAL)		drawOval(graphics);

		else if (drawType == DRAW_TYPE.AREA0)		drawDottedRectangle(graphics); //�ƹ��͵� �׸��� �ʴ´�
		else if (drawType == DRAW_TYPE.AREA1) {
			//�̹����� �̵��� ���� �׸���
			area.drawImg(img);
//			drawAreaWithDottedRect(img, area);
		}
		else if (drawType == DRAW_TYPE.AREA9) {
			area.drawImg(img);
			drawAreaWithDottedRect(img, area);
		}
//		else if (drawType == DRAW_TYPE.AREA9)		area.drawImg(img);
		
		else if (drawType == DRAW_TYPE.SCALE);
		else if (drawType == DRAW_TYPE.LEAN);
		else if (drawType == DRAW_TYPE.ROTATE);
		
		DRAW_TYPE editingTypes[] = {
				DRAW_TYPE.PENCIL   , DRAW_TYPE.FILL     ,
				DRAW_TYPE.ERASE    , DRAW_TYPE.LINE     ,
				DRAW_TYPE.CURVE0   , DRAW_TYPE.CURVE1   , DRAW_TYPE.CURVE2   ,
				DRAW_TYPE.TRIANGLE , DRAW_TYPE.RECTANGLE, DRAW_TYPE.OVAL     ,
				DRAW_TYPE.SCALE    , DRAW_TYPE.LEAN     , DRAW_TYPE.ROTATE   ,
				};
		for (DRAW_TYPE dt : editingTypes)
			if (drawType == dt)
				�׸���app.setFileSaved(false);
		
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
	private int clamp(int value, int min, int max) {
		int temp = (min > value ? min : value);
		return temp > max ? max : temp;
	}
	private Point clampPt(Point pt) {
		return new Point(
				clamp(pt.x, 0, 750 - 1),
				clamp(pt.y, 0, 500-1));
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
	private void dottedLine(Graphics graphics, Point src, Point dst) {
		if (src.x == dst.x && src.y != dst.y) {
			int x = src.x;
			int y1 = src.y < dst.y ? src.y : dst.y;
			int y2 = src.y > dst.y ? src.y : dst.y;
			for (int y = y1; y <= y2; ++y) {
				if (y % 8 < 4)
					graphics.fillRect(x, y, 2, 2);
			}
		}
		else if (src.x != dst.x && src.y == dst.y) {
			int x1 = src.x < dst.x ? src.x : dst.x;
			int x2 = src.x > dst.x ? src.x : dst.x;
			int y = src.y;
			for (int x = x1; x <= x2; ++x) {
				if (x % 8 < 4)
					graphics.fillRect(x, y, 2, 2);
			}
		}
		else {
			graphics.fillRect(src.x, src.y, 2, 2);
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
	private void drawDottedRectangle(Graphics graphics) {
		drawDottedRectangle(graphics, srcPt, dstPt);
	}
	private void drawDottedRectangle(Graphics graphics, Point srcPt, Point dstPt) {
		graphics.setColor(Color.black);
		Point pt1 = new Point(srcPt.x, dstPt.y);
		Point pt2 = new Point(dstPt.x, srcPt.y);
		dottedLine(graphics, srcPt, pt1);
		dottedLine(graphics, srcPt, pt2);
		dottedLine(graphics, dstPt, pt1);
		dottedLine(graphics, dstPt, pt2);
		graphics.setColor(penColor);
	}
	private void drawAreaWithDottedRect(BufferedImage img, ImgArea area) {
		area.drawImg(img);
		Point rectSrc = area.getLocation();
		Point rectDst = new Point(rectSrc.x + area.getWidth(), rectSrc.y + area.getHeight());
		drawDottedRectangle(img.getGraphics(), rectSrc, rectDst);
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

	/*
	 * ctrlX, ctrlC, ctrlV �� ������� ���� editItem�� Ȱ��ȭ & ��Ȱ��ȭ �ǵ��� ��ġ
	 * 			ctrlX	ctrlC	ctrlV
	 * canX		X		O		O
	 * canC		X		O		O
	 * canV		O		O		O
	 */
	public void ctrlX() {
		if(!canCtrlX) return;
		
		//area�� ������ Ŭ������� �����Ѵ�
//		BufferedImage clipboard = area;
		
		canCtrlX = false; �׸���app.setCtrlXEnabled(canCtrlX);
		canCtrlC = false; �׸���app.setCtrlCEnabled(canCtrlC);
		canCtrlV = true; �׸���app.setCtrlVEnabled(canCtrlV);
		
		drawType = DRAW_TYPE.AREA0;
		
		�׸���app.setFileSaved(false);
	}
	public void ctrlC() {
		if(!canCtrlC) return;
		
		ctrlX();
		ctrlV();
		
		�׸���app.setFileSaved(true);
	}
	public void ctrlV() {
		if(!canCtrlV) return;
		
		//Ŭ�������� �̹����� area�� �����Ѵ�

		canCtrlX = true; �׸���app.setCtrlXEnabled(canCtrlX);
		canCtrlC = true; �׸���app.setCtrlCEnabled(canCtrlC);
		canCtrlV = true; �׸���app.setCtrlVEnabled(canCtrlV);
		
		drawType = DRAW_TYPE.AREA1;

		�׸���app.setFileSaved(false);
	}
	
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
		g.drawImage(img, 0, 0, 750, 500, this);
	}

	public void setDrawType(DRAW_TYPE type) {
		if (drawType == DRAW_TYPE.AREA1) {
//			drawType = DRAW_TYPE.AREA9;

//			draw(tempImg);
//			copyImg(tempImg, img);

			draw(img);
			drawType = DRAW_TYPE.AREA0;
		} else if (drawType == DRAW_TYPE.CURVE1 ||
				drawType == DRAW_TYPE.CURVE2) {
			drawType = DRAW_TYPE.CURVE2;
			draw(img);
			drawType = DRAW_TYPE.CURVE0;
		}
		//�� �� draw()�Լ� ����� ��, draw(tempTmg) + swapImg(); �� �����ϱ�
		
		oldDrawType = drawType;
		drawType = type;
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
	public boolean canCtrlX() { return canCtrlX; }
	public boolean canCtrlC() { return canCtrlC; }
	public boolean canCtrlV() { return canCtrlV; }
	
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
	
}

class ImgArea extends BufferedImage {
	private Point location;
	
	public ImgArea(Point srcPt, Point dstPt, BufferedImage img) {
		super(Math.abs(srcPt.x - dstPt.x),
				Math.abs(srcPt.y - dstPt.y),
				BufferedImage.TYPE_INT_ARGB);
		location = new Point(
				srcPt.x < dstPt.x ? srcPt.x : dstPt.x,
				srcPt.y < dstPt.y ? srcPt.y : dstPt.y);

		copyImg(srcPt, dstPt, img);
	}
	private void copyImg(Point srcPt, Point dstPt, BufferedImage img) {
		for (int row = 0; row < getHeight(); ++row) {
			for (int col = 0; col < getWidth(); ++col) {
				int rgb = img.getRGB(col + location.x, row + location.y);
				setRGB(col, row, rgb);
				img.setRGB(col + location.x, row + location.y, Color.white.getRGB());
			}
		}
	}

	public void drawImg(BufferedImage img) {
		for (int row = 0; row < getHeight(); ++row) {
			for (int col = 0; col < getWidth(); ++col) {
				int rgb = getRGB(col, row);
				int x = col + location.x;
				int y = row + location.y;
				if (0 <= x && x < 750 && 0 <= y && y < 500)
					img.setRGB(x, y, rgb);
			}
		}
	}
	
	public boolean isInsideOfArea(Point mousePt) {
		if (location.x <= mousePt.x && mousePt.x < location.x + getWidth() &&
			location.y <= mousePt.y && mousePt.y < location.y + getWidth())
			return true;
		else
			return false;
	}
	
	public Point getLocation() { return location; }
	public void setLocation(Point location) { this.location = location; }
}
	