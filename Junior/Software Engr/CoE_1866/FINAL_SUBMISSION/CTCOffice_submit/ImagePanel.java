import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.ImageIO;
import javax.swing.JFrame;
import javax.swing.JPanel;

import java.util.HashMap;
import java.util.Set;

public class ImagePanel extends JPanel{
	private String imageDir = "C:\\Users\\trottj\\Dropbox\\CoE_1866\\Images\\TrackModel\\";
	private String trainImageDir = "C:\\Users\\trottj\\Dropbox\\CoE_1866\\Code\\CTCOfficeModule\\TrainImages\\";
	//private String imageDir = "";
	//private String trainImageDir = "";
	private String fileExt = ".png";
    private HashMap<String, BufferedImage> images;
	private BufferedImage canvas;
	private int x;
	private int y;
	
    public ImagePanel() {               
          images = new HashMap<String, BufferedImage>();
    }

	public void setCanvas(String image){
		try{
			canvas = ImageIO.read(new File( imageDir + image + fileExt));
			this.setPreferredSize(new Dimension(canvas.getWidth(), canvas.getHeight()));
		}
		catch(IOException ex){
			System.out.println("Could not open image file " + imageDir + image + fileExt + ".");
			ex.printStackTrace();
		}
	}
	public void addImage(String image){
		try{
			images.put(image,ImageIO.read(new File(trainImageDir + image + fileExt)));
		}
		catch(IOException ex){
			try{
				images.put(image,ImageIO.read(new File(trainImageDir + image + "_medres" + fileExt)));
			}
			catch(IOException ex2){
				System.out.println("Could not open image file " + trainImageDir + image + fileExt + ".");
				ex.printStackTrace();
			}
		}
	}
	
	public void removeImage(String image){
		if(images.containsKey(image)){
			images.remove(image);
		}
	}
	
	public void setSize(int x, int y){
		this.setPreferredSize(new Dimension(x, y));
	}
	
    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        g.setColor(Color.WHITE);
        g.fillRect(0, 0, 500, 500);
        g.drawImage(canvas.getScaledInstance(500, 500, Image.SCALE_FAST), 0, 0, null); // see javadoc for more info on the parameters 
		Set<String> keys = images.keySet();
		for(String k : keys){
			g.drawImage(images.get(k).getScaledInstance(500, 500, Image.SCALE_FAST), 0, 0, null);
		}
		images = new HashMap<String, BufferedImage>();
    }
	
	public static void main(String[] args){
		ImagePanel ip = new ImagePanel();
		ip.setCanvas("trackCanvas");
		ip.addImage("TG1");
		JFrame frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.add(ip);
		frame.pack();
		frame.setVisible(true);
		//ip.removeImage("TG1");
		int i = 0;
		while(true){
			i = (i + 1)%85;
			try {
				Thread.sleep(500);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			ip.removeImage("TG" + i);
			ip.addImage("TG" + (i + 1));
			frame.repaint();
		}
	}
}