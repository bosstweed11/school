import java.awt.BorderLayout;
import java.awt.FlowLayout;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import net.miginfocom.swing.MigLayout;
import javax.swing.JLabel;
import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

import javax.swing.Box;
import javax.swing.JTextField;
import javax.swing.JComboBox;


public class Request extends JDialog {

	private final JPanel contentPanel = new JPanel();
	private JTextField speedText;
	private JTextField authorityText;
	private boolean valid = false;
	private int speedRequest;
	private int authorityRequest;
	private int trainID = -1;
	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		try {
			Request dialog = new Request(100, 100, 0);
			dialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
			dialog.setVisible(true);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * Create the dialog.
	 * @param trainComboBox 
	 */
	public Request(int x, int y, int trainID) {
		super();
		this.trainID = trainID;
		setTitle("New Request");
		setBounds(x - 280/2, y - 150/2, 400, 240);
		getContentPane().setLayout(new BorderLayout());
		contentPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		getContentPane().add(contentPanel, BorderLayout.CENTER);
		contentPanel.setLayout(new MigLayout("", "[][][grow][][][][][][][][][][][][]", "[][][][][][][]"));
		{
			JLabel lblSpeedRequest = new JLabel("Speed Request:");
			contentPanel.add(lblSpeedRequest, "cell 0 0 2 1");
		}
		{
			speedText = new JTextField();
			contentPanel.add(speedText, "cell 2 0,growx");
			speedText.setColumns(10);
		}
		{
			Component verticalStrut = Box.createVerticalStrut(20);
			contentPanel.add(verticalStrut, "cell 8 0");
		}
		{
			JLabel lblAuthorityRequest = new JLabel("Authority Request:");
			contentPanel.add(lblAuthorityRequest, "cell 0 1 2 1");
		}
		{
			authorityText = new JTextField();
			contentPanel.add(authorityText, "cell 2 1,growx");
			authorityText.setColumns(10);
		}
		{
			JLabel lblTrainid = new JLabel("TrainID:");
			contentPanel.add(lblTrainid, "cell 0 3 2 1");
		}
		{
			JLabel label = new JLabel(Integer.toString(trainID));
			contentPanel.add(label, "cell 2 3");
		}
		{
			Component verticalStrut = Box.createVerticalStrut(20);
			contentPanel.add(verticalStrut, "cell 9 3");
		}
		{
			Component horizontalStrut = Box.createHorizontalStrut(20);
			contentPanel.add(horizontalStrut, "cell 0 6");
		}
		{
			Component horizontalStrut = Box.createHorizontalStrut(20);
			contentPanel.add(horizontalStrut, "cell 1 6");
		}
		{
			JPanel buttonPane = new JPanel();
			buttonPane.setLayout(new FlowLayout(FlowLayout.RIGHT));
			getContentPane().add(buttonPane, BorderLayout.SOUTH);
			{
				JButton okButton = new JButton("Send");
				okButton.setActionCommand("OK");
				buttonPane.add(okButton);
				okButton.addActionListener(new ActionListener(){

					@Override
					public void actionPerformed(ActionEvent e) {
						try{
							speedRequest = Integer.parseInt(speedText.getText());
							authorityRequest = Integer.parseInt(authorityText.getText());
							valid = true;
							setVisible(false);
						}
						catch(NumberFormatException nfe){
							JOptionPane.showMessageDialog(new JFrame(), "Can only input numeric values.");
							valid = false;
						}
					}
					
				});
				getRootPane().setDefaultButton(okButton);
			}
			{
				JButton cancelButton = new JButton("Cancel");
				cancelButton.setActionCommand("Cancel");
				cancelButton.addActionListener(new ActionListener(){

					@Override
					public void actionPerformed(ActionEvent arg0) {
						valid = false;
						setVisible(false);
					}
					
				});
				buttonPane.add(cancelButton);
			}
		}
	}

	public int getSpeedRequest(){
		return speedRequest;
	}
	
	public int getAuthorityRequest(){
		return authorityRequest;
	}
	public boolean showRequest(){
		this.setVisible(true);
		return valid;
	}
}
