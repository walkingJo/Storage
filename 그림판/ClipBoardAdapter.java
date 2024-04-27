package 그림판;

import java.awt.Image;
import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.ClipboardOwner;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.image.BufferedImage;
import java.io.IOException;

public class ClipBoardAdapter implements ClipboardOwner {
	public void CopyImagetoClipBoard(BufferedImage img) {
		TransferableImage transImg = new TransferableImage(img);
		Clipboard clipboard = getClipboard();
		clipboard.setContents(transImg, this);
	}
	
	public BufferedImage CopyImagefromClipBoard() {
		Transferable content = getClipboard().getContents(null);
		
		if (content == null || !content.isDataFlavorSupported(DataFlavor.imageFlavor))
			return null;
		
		try {
			BufferedImage img = (BufferedImage) content.getTransferData(DataFlavor.imageFlavor);
			return img;
		} catch (UnsupportedFlavorException | IOException e) {
			e.printStackTrace();
			return null;
		}
	}
	
	private Clipboard getClipboard() {
		return Toolkit.getDefaultToolkit().getSystemClipboard();
	}
	
	@Override
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
