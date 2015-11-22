/* CS 1501
   Primitive chat client. 
   This client connects to a server so that messages can be typed and forwarded
   to all other clients.  Try it out in conjunction with ImprovedChatServer.java.
   You will need to modify / update this program to incorporate the secure elements
   as specified in the Assignment sheet.  Note that the PORT used below is not the
   one required in the assignment -- be sure to change that and also be sure to
   check on the location of the server program regularly (it may change).
*/
import java.util.*;
import java.io.*;
import java.net.*;
import javax.swing.*;
import java.awt.event.*;
import java.awt.*;
import java.math.*;

public class ImprovedChatClient extends JFrame implements Runnable, ActionListener {

    public static final int PORT = 8765;

    BufferedReader myReader;
    PrintWriter myWriter;
    JTextArea outputArea;
    JLabel prompt;
    JTextField inputField;
    String myName, serverName;
	Socket connection;
	SymCipher theCipher;
	ObjectOutputStream tempWriter;
	ObjectInputStream tempReader;
	
	
    public ImprovedChatClient ()
    {
        try {
		
		
        myName = JOptionPane.showInputDialog(this, "Enter your user name: ");
        serverName = JOptionPane.showInputDialog(this, "Enter the server name: ");
        InetAddress addr =
                InetAddress.getByName(serverName);
        connection = new Socket(addr, PORT);   // Connect to server with new
        
         myReader =
             new BufferedReader(
                 new InputStreamReader(
                     connection.getInputStream()));   // Get Reader and Writer

        myWriter =
             new PrintWriter(
                 new BufferedWriter(
                     new OutputStreamWriter(connection.getOutputStream())), true);
                     
        tempWriter =
							  new ObjectOutputStream(connection.getOutputStream());
		tempWriter.flush();
		tempReader =
				  new ObjectInputStream(connection.getInputStream());
				  
		BigInteger E = new BigInteger("1");
		E = (BigInteger)tempReader.readObject();
		
		BigInteger N = new BigInteger("1");
		N = (BigInteger)tempReader.readObject();
		
		System.out.println("E is :");
		System.out.println(E);

		System.out.println("N is :");
		System.out.println(N);
		
		String type = new String("");
		type = (String)tempReader.readObject();
		int size;
		if ( type.equals("Sub"))
		{
			System.out.println("\nSubstitue Cipher Added\n");
			theCipher = new Substitute();
			size = 256;
		}
		else
		{
			System.out.println("\nAdd128 Cipher Added\n");
			theCipher = new Add128();
			size = 128;
		}
		
		byte[] theKey = new byte[size];
		
		theKey = theCipher.getKey();
		
		BigInteger initKey = new BigInteger(1,theKey);
		BigInteger encrypted = new BigInteger("1");
		
		System.out.println("\nInitial Key: " + initKey + "\n");
		
		encrypted = initKey.modPow(E,N);
		
		tempWriter.writeObject(encrypted);tempWriter.flush();

        byte[] nameArray = theCipher.encode(myName);
        
        tempWriter.writeObject(nameArray);tempWriter.flush();
        
        this.setTitle(myName);      // Set title to identify chatter

        Box b = Box.createHorizontalBox();  // Set up graphical environment for
        outputArea = new JTextArea(8, 30);  // user
        outputArea.setEditable(false);
        b.add(new JScrollPane(outputArea));

        outputArea.append("Welcome to the Chat Group, " + myName + "\n");

        inputField = new JTextField("");  // This is where user will type input
        inputField.addActionListener(this);

        prompt = new JLabel("Type your messages below:");
        Container c = getContentPane();

        c.add(b, BorderLayout.NORTH);
        c.add(prompt, BorderLayout.CENTER);
        c.add(inputField, BorderLayout.SOUTH);

        Thread outputThread = new Thread(this);  // Thread is to receive strings
        outputThread.start();                    // from Server

		addWindowListener(
                new WindowAdapter()
                {
                    public void windowClosing(WindowEvent e)
                    {
                    	String end= new String("CLIENT CLOSING");
                     
						byte[] msgArray = theCipher.encode(end);
						try
						{
							System.out.println("Terminating");
							tempWriter.writeObject(msgArray);tempWriter.flush();
						}
						catch ( Exception k )
						{
							System.out.println("Problem ending client!");
						
						}
						
						
                        System.exit(0);
                     }
                }
            );

        setSize(500, 200);
        setVisible(true);

        }
        catch (Exception e)
        {
            System.out.println("Problem starting client!");
        }
    }

    public void run()
    {
        while (true)
        {
             try {
				byte [] message = (byte[])tempReader.readObject();
				
				System.out.println("Before Decode:");
				for (int i = 0; i < message.length; i++ )
				{
					
					System.out.print(message[i]);
					System.out.print("\t");	
				
				}
				
				System.out.println("");	
				String theMsg = theCipher.decode(message);
				
				byte[] stringArray = new byte[message.length];
		
				stringArray = theMsg.getBytes();
				
				System.out.println("After Decode:");
				for (int i = 0; i < message.length; i++ )
				{
					
					System.out.print(stringArray[i]);
					System.out.print("\t");	
				
				}
				
				
				
				System.out.println("\nMessage: " + theMsg);
				
				
				
			    outputArea.append(theMsg+"\n");
             }
             catch (Exception e)
             {
                System.out.println(e +  ", closing client!");
                break;
             }
        }
        System.exit(0);
    }

    public void actionPerformed(ActionEvent e)
    {
        String currMsg = e.getActionCommand();      // Get input value
        try
        {
			inputField.setText("");
			
			String theMsg = myName + ":" + currMsg;   // Add name and send it
			
			
			int length = theMsg.length();
			
			byte[] stringArray = new byte[length];
		
			stringArray = theMsg.getBytes();
			System.out.println("\nMessage: " + theMsg);
			
			System.out.println("Before Encode:");
			for ( int i = 0; i < length; i++ )
			{
				System.out.print(stringArray[i]);
				System.out.print("\t");	
				
			}
			System.out.println("");	
			byte[] msgArray = theCipher.encode(theMsg);
			
			System.out.println("After Encode:");
			for ( int i = 0; i < length; i++ )
			{
				System.out.print(msgArray[i]);
				System.out.print("\t");	
				
			}
			System.out.println("");	
			
			tempWriter.writeObject(msgArray);tempWriter.flush();
		}
		 catch (Exception a)
        {
            System.out.println("Problem starting client!");
        }
     
    }                                               // to Server

    public static void main(String [] args)
    {
         ImprovedChatClient JR = new ImprovedChatClient();
         JR.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
    }
}

