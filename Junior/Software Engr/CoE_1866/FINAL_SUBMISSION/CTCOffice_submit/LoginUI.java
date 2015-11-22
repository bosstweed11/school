import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import net.miginfocom.swing.MigLayout;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.JPasswordField;


@SuppressWarnings("serial")
public class LoginUI extends JDialog {

	private final JPanel contentPanel = new JPanel();
	private JTextField textField;
	private JPasswordField passwordField;

	private LoginModel lm = null;
	private boolean valid = false;
	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		try {
			LoginUI dialog = new LoginUI();
			dialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
			dialog.setVisible(true);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * Create the dialog.
	 */
	public LoginUI() {
		this.setTitle("Login");
		lm = new LoginModel();
		try {
			lm.load();
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		Toolkit toolkit = Toolkit.getDefaultToolkit();
		Dimension dim = toolkit.getScreenSize();
		setBounds(100, 100, 450, 300);
		setLocation(dim.width / 2 - this.getWidth() / 2, dim.height
				/ 2 - this.getWidth() / 2);
		getContentPane().setLayout(new BorderLayout());
		contentPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		getContentPane().add(contentPanel, BorderLayout.CENTER);
		contentPanel.setLayout(new MigLayout("", "[][grow]", "[grow][][][][][]"));
		{
			JPanel panel = new JPanel();
			contentPanel.add(panel, "cell 0 0 2 4,grow");
		}
		{
			JLabel lblUsername = new JLabel("Username:");
			contentPanel.add(lblUsername, "cell 0 4,alignx trailing");
		}
		{
			textField = new JTextField();
			contentPanel.add(textField, "cell 1 4,growx");
			textField.setColumns(10);
		}
		{
			JLabel lblPassword = new JLabel("Password:");
			contentPanel.add(lblPassword, "cell 0 5,alignx trailing");
		}
		{
			passwordField = new JPasswordField();
			contentPanel.add(passwordField, "cell 1 5,growx");
		}
		{
			JPanel buttonPane = new JPanel();
			buttonPane.setLayout(new FlowLayout(FlowLayout.RIGHT));
			getContentPane().add(buttonPane, BorderLayout.SOUTH);
			{
				JButton okButton = new JButton("Login");
				okButton.setActionCommand("OK");
				okButton.addActionListener(new ActionListener(){

					@Override
					public void actionPerformed(ActionEvent arg0) {
						lm.setUserName(textField.getText());
						lm.setPassword(new String(passwordField.getPassword()));
						if (lm.submit()) {
							valid = true;
							setVisible(false);
						} else if (lm.getAttempts() >= LoginModel.MAX_ATTEMPTS) {
							valid = false;
							setVisible(false);
						}
						else{
							JOptionPane.showMessageDialog(new JFrame(), "Username or password did not match, try again.");
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
					public void actionPerformed(ActionEvent e) {
						valid = false;
						setVisible(false);
					}
					
				});
				buttonPane.add(cancelButton);
			}
		}
	}
	
	public void clear(){
		textField.setText("");
		passwordField.setText("");
		valid = false;
	}
	
	public boolean showLogin(){
		setVisible(true);
		return valid;
	}

}
