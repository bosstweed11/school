//package trainControl.TrackController;

import java.awt.*;
import javax.swing.*;
import net.miginfocom.swing.MigLayout;

public class display extends JPanel
{
	private int width = 600;
	private int height = 150;
	
	JLabel chooseController;
	JLabel lineL;
	JLabel crossingsL;
	JLabel switchesL;
	JLabel switches;
	JLabel crossings;
	JLabel line;
	
	JTextField updates;
	
	JComboBox controllerIDList;
	
	public display()
	{
		this.setPreferredSize(new Dimension(width, height));
		this.setSize(width, height);
		setLayout(new MigLayout("fill", "", ""));
	    addComponents();
	}
	
	public void addComponents()
	{		
		chooseController = new JLabel("Choose Controller: ");
		
		lineL = new JLabel("Line: ");
		crossingsL = new JLabel("Crossings: ");
		switchesL = new JLabel("Switches: ");
		
		line = new JLabel();
		crossings = new JLabel();
		switches = new JLabel();
		
		updates = new JTextField(" ");
		
		String[] comboBoxItems = {"Green","Red"};
		DefaultComboBoxModel model = new DefaultComboBoxModel(comboBoxItems);
		controllerIDList = new JComboBox(model);
		
		add(chooseController, "split 2");
		
		add(controllerIDList, "wrap");
		
		add(lineL);
		
		add(crossingsL);
		
		add(switchesL, "wrap");
		
		add(line);
		
		add(crossings);
		
		add(switches, "wrap");
		
		//add(updates, "span 3, w 100%, h 20%");
	}

}
