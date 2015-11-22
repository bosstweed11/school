import java.awt.BorderLayout;
import java.awt.FlowLayout;

import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import javax.swing.text.DefaultFormatterFactory;
import javax.swing.text.NumberFormatter;

import net.miginfocom.swing.MigLayout;
import javax.swing.JLabel;
import javax.swing.JFormattedTextField;
import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.text.NumberFormat;

import javax.swing.Box;
import javax.swing.JRadioButton;


@SuppressWarnings("serial")
public class TrainDispatcher extends JDialog {

	private final JPanel contentPanel = new JPanel();
	private boolean valid = false;
	private int trainID = -1;
	private int authority = -1;
	private String route = null;
	
	private JRadioButton rdbtnGreen = null;
	private JRadioButton rdbtnRed = null;
	private ButtonGroup bg = null;
	
	private JFormattedTextField authorityTextField = null;
	private JFormattedTextField trainIDTextField = null;
	
	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		try {
			TrainDispatcher dialog = new TrainDispatcher(100, 100, 1);
			dialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
			dialog.setVisible(true);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * Create the dialog.
	 */
	public TrainDispatcher(int x, int y, int ID) {
		setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
		setTitle("Train Dispatcher");
		trainID = ID;
		setModal(true);
		setBounds(x - 450/2, y - 300/2, 450, 300);
		getContentPane().setLayout(new BorderLayout());
		contentPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		getContentPane().add(contentPanel, BorderLayout.CENTER);
		contentPanel.setLayout(new MigLayout("", "[][grow][][][][][][][][][][][][]", "[][][]"));
		{
			JLabel lblTrainId = new JLabel("Train ID:");
			contentPanel.add(lblTrainId, "cell 0 0,alignx right");
		}
		{
			NumberFormat f = NumberFormat.getNumberInstance(); 
			f.setMaximumIntegerDigits(3);
		}
		{
			JLabel label = new JLabel(Integer.toString(ID));
			contentPanel.add(label, "cell 1 0");
		}
		{
			Component horizontalStrut = Box.createHorizontalStrut(20);
			contentPanel.add(horizontalStrut, "cell 2 0");
		}
		{
			Component horizontalStrut = Box.createHorizontalStrut(20);
			contentPanel.add(horizontalStrut, "cell 3 0");
		}
		{
			Component horizontalStrut = Box.createHorizontalStrut(20);
			contentPanel.add(horizontalStrut, "cell 4 0");
		}
		{
			Component horizontalStrut = Box.createHorizontalStrut(20);
			contentPanel.add(horizontalStrut, "cell 5 0");
		}
		{
			Component horizontalStrut = Box.createHorizontalStrut(20);
			contentPanel.add(horizontalStrut, "cell 6 0");
		}
		{
			Component horizontalStrut = Box.createHorizontalStrut(20);
			contentPanel.add(horizontalStrut, "cell 7 0");
		}
		{
			Component horizontalStrut = Box.createHorizontalStrut(20);
			contentPanel.add(horizontalStrut, "cell 8 0");
		}
		{
			Component horizontalStrut = Box.createHorizontalStrut(20);
			contentPanel.add(horizontalStrut, "cell 9 0");
		}
		{
			Component horizontalStrut = Box.createHorizontalStrut(20);
			contentPanel.add(horizontalStrut, "cell 10 0");
		}
		{
			Component horizontalStrut = Box.createHorizontalStrut(20);
			contentPanel.add(horizontalStrut, "cell 11 0");
		}
		{
			Component horizontalStrut = Box.createHorizontalStrut(20);
			contentPanel.add(horizontalStrut, "cell 12 0");
		}
		{
			Component horizontalStrut = Box.createHorizontalStrut(20);
			contentPanel.add(horizontalStrut, "cell 13 0");
		}
		{
			JLabel lblAuthority = new JLabel("Authority:");
			contentPanel.add(lblAuthority, "cell 0 1,alignx right");
		}
		{
			authorityTextField = new JFormattedTextField();
			contentPanel.add(authorityTextField, "cell 1 1,growx");
		}
		{
			JLabel lblRoute = new JLabel("Route:");
			contentPanel.add(lblRoute, "cell 0 2,alignx right");
		}
		{
			bg = new ButtonGroup();
			rdbtnGreen = new JRadioButton("Green");
			contentPanel.add(rdbtnGreen, "cell 1 2");
			rdbtnRed = new JRadioButton("Red");
			contentPanel.add(rdbtnRed, "cell 2 2");
			bg.add(rdbtnGreen);
			bg.add(rdbtnRed);
		}
		{
			JPanel buttonPane = new JPanel();
			buttonPane.setLayout(new FlowLayout(FlowLayout.RIGHT));
			getContentPane().add(buttonPane, BorderLayout.SOUTH);
			{
				JButton okButton = new JButton("Dispatch");
				okButton.setActionCommand("OK");
				okButton.addActionListener(new ActionListener(){

					@Override
					public void actionPerformed(ActionEvent arg0) {
						authority = Integer.parseInt(authorityTextField.getText());
						if(rdbtnGreen.isSelected()){
							route = "Green";
							valid = true;
							setVisible(false);
						}
						else if(rdbtnRed.isSelected()){
							route = "Red";
							valid = true;
							setVisible(false);
						}
						else{
							valid = false;
						}
					}
					
				});
				
				buttonPane.add(okButton);
				getRootPane().setDefaultButton(okButton);
			}
			{
				JButton cancelButton = new JButton("Cancel");
				cancelButton.setActionCommand("Cancel");
				cancelButton.addActionListener(new ActionListener(){

					@Override
					public void actionPerformed(ActionEvent arg0) {
						setVisible(false);
						valid = false;
					}
					
				});
				
				buttonPane.add(cancelButton);
			}
		}
	}
	
	public boolean showDispatcher(){
		setVisible(true);
		return valid;
	}
	
	public int getID(){
		return trainID;
	}
	
	public int getAuthority(){
		return authority;
	}
	
	public String getRoute(){
		return route;
	}

}
