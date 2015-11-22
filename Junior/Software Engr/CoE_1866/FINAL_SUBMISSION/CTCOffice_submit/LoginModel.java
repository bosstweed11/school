

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Set;


public class LoginModel {
	private HashMap<String, String> userLib = null;
	private String fileName = "C:\\Users\\trottj\\Dropbox\\CoE_1866\\Code\\CTCOfficeModule\\username_passwords.txt";
	//private String fileName = "C:\\Users\\beersa\\Dropbox\\CoE_1866\\Code\\CTCOfficeModule\\username_passwords.txt";
	private static final String SPLITTER = "\\|";
	protected static final int MAX_ATTEMPTS = 3;
	private static int attempts = 0;
	private String curPassword = null;
	private String curUsername = null;

	private boolean successful = false;

	public LoginModel() { 
		super();
		userLib = new HashMap<String, String>();
	}

	public void load() throws IOException {
		FileInputStream fis = new FileInputStream(fileName);
		DataInputStream din = new DataInputStream(fis);
		BufferedReader br = new BufferedReader(new InputStreamReader(din));
		String str;

		while ((str = br.readLine()) != null) {
			String[] unAndpw = str.split(SPLITTER);
			userLib.put(unAndpw[0], unAndpw[1]);
		}

		br.close();
		din.close();
		fis.close();
	}

	public void setUserName(String un) {
		this.curUsername = un;
	}

	public void setPassword(String pw) {
		this.curPassword = pw;
	}

	public String getUsername() {
		return curUsername;
	}

	public String getPassword() {
		return curPassword;
	}

	public boolean isSuccessful() {
		return successful;
	}

	public int getAttempts() {
		return attempts;
	}

	public boolean submit() {
		attempts++;
		Set<String> keys = (Set<String>) userLib.keySet();
		for (String k : keys) {
			if (k.compareToIgnoreCase(curUsername) == 0) {
				String password = userLib.get(k);
				if (password.compareTo(curPassword) == 0) {
					successful = true;
					return successful;
				} else {
					successful = false;
					return successful;
				}
			}
		}
		successful = false;
		return successful;
	}
}
