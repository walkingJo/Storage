package clipboard;

import java.awt.Toolkit;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.image.BufferedImage;
import java.io.File;
/**
 * https://stackoverflow.com/questions/49761432/how-to-write-image-to-disk-from-clipboard-in-java
 */

import javax.imageio.ImageIO;

public class ClipboardToImageData {

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		System.err.println("usage: java clipimg [filename]");
        String outputfile = "/temp/1.png";
        if (args.length > 0)
            outputfile = args[0];
        copyTo(outputfile);
	}

	static int copyTo(String filename) throws Exception {
        Transferable content = Toolkit.getDefaultToolkit().getSystemClipboard().getContents(null);
        if (content == null) {
            System.err.println("error: nothing found in clipboard");
            return 1;
        }
        if (!content.isDataFlavorSupported(DataFlavor.imageFlavor)) {
            System.err.println("error: no image found in clipbaord");
            return 2;
        }
        BufferedImage img = (BufferedImage) content.getTransferData(DataFlavor.imageFlavor);
        String ext = ext(filename);
        if (ext == null) {
            ext = "png";
            filename += "." + ext;
        }
        File outfile = new File(filename);
        ImageIO.write(img, ext, outfile);
        System.err.println("image copied to: " + outfile.getAbsolutePath());
        return 0;
    }

    static String ext(String filename) {
        int pos = filename.lastIndexOf('.') + 1;
        if (pos == 0 || pos >= filename.length())
            return null;
        return filename.substring(pos);
    }
}
