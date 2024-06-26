package clipboard;

import java.awt.AWTException;
import java.awt.Dimension;
import java.awt.Image;
import java.awt.Rectangle;
import java.awt.Robot;
import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.ClipboardOwner;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.image.BufferedImage;
import java.io.IOException;

/**
 * https://stackoverflow.com/questions/4552045/copy-bufferedimage-to-clipboard
 */

public class CopyImagetoClipBoard implements ClipboardOwner {
	public CopyImagetoClipBoard() {
		try {
			Robot robot = new Robot();
			Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
			Rectangle screen = new Rectangle(screenSize);
			BufferedImage i = robot.createScreenCapture(screen);
			TransferableImage trans = new TransferableImage(i);
			Clipboard c = Toolkit.getDefaultToolkit().getSystemClipboard();
			c.setContents(trans, this);
		} catch (AWTException x) {
			x.printStackTrace();
			System.exit(1);
		}
	}

	public static void main(String[] arg) {
		CopyImagetoClipBoard ci = new CopyImagetoClipBoard();
	}

	public void lostOwnership(Clipboard clip, Transferable trans) {
		System.out.println("Lost Clipboard Ownership");
	}

	private class TransferableImage implements Transferable {

		Image i;

		public TransferableImage(Image i) {
			this.i = i;
		}
		@Override
		public Object getTransferData(DataFlavor flavor) throws UnsupportedFlavorException, IOException {
			if (flavor.equals(DataFlavor.imageFlavor) && i != null) {
				return i;
			} else {
				throw new UnsupportedFlavorException(flavor);
			}
		}
		@Override
		public DataFlavor[] getTransferDataFlavors() {
			DataFlavor[] flavors = new DataFlavor[1];
			flavors[0] = DataFlavor.imageFlavor;
			return flavors;
		}
		@Override
		public boolean isDataFlavorSupported(DataFlavor flavor) {
			DataFlavor[] flavors = getTransferDataFlavors();
			for (int i = 0; i < flavors.length; i++) {
				if (flavor.equals(flavors[i])) {
					return true;
				}
			}

			return false;
		}
	}
}
