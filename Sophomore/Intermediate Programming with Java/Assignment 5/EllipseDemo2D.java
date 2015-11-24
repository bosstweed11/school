import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.GradientPaint;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.geom.Arc2D;
import java.awt.geom.Ellipse2D;
import java.awt.geom.GeneralPath;
import java.awt.geom.Line2D;
import java.awt.geom.Rectangle2D;
import java.awt.geom.RoundRectangle2D;

import javax.swing.JApplet;
import javax.swing.JFrame;

public class EllipseDemo2D extends JApplet {
  final static BasicStroke stroke = new BasicStroke(2.0f);  
  public void init() {
    setBackground(Color.white);
    setForeground(Color.white);
  }


  public void paint(Graphics g) {
    Graphics2D g2 = (Graphics2D) g;
    g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
        RenderingHints.VALUE_ANTIALIAS_ON);

    g2.setPaint(Color.gray);
    int x = 5;
    int y = 7;

    g2.setStroke(stroke);
    g2.draw(new Ellipse2D.Double(x, y, 200, 200));
    g2.drawString("Ellipse2D", x, 250);

  }

  public static void main(String s[]) {
    JFrame f = new JFrame("");
    f.addWindowListener(new WindowAdapter() {
      public void windowClosing(WindowEvent e) {
        System.exit(0);
      }
    });
    JApplet applet = new EllipseDemo2D();
    f.getContentPane().add("Center", applet);
    applet.init();
    f.pack();
    f.setSize(new Dimension(300, 300));
    f.show();
  }
}