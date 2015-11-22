import java.awt.Dimension;
import java.awt.EventQueue;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;

import javax.swing.JFrame;
import net.miginfocom.swing.MigLayout;

import javax.swing.ImageIcon;
import javax.swing.JFileChooser;
import javax.swing.JMenuBar;
import javax.swing.JMenu;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.Timer;
import javax.swing.border.TitledBorder;
import javax.swing.JComboBox;
import javax.swing.UIManager;
import java.awt.Component;
import javax.swing.Box;
import javax.swing.JCheckBox;
import javax.swing.JToggleButton;
import javax.swing.JTabbedPane;
import java.awt.Color;
import java.awt.event.WindowEvent;
import java.io.File;
import java.util.ArrayList;
import java.util.Date;
import java.util.Set;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

import javax.swing.JMenuItem;

public class CTCOfficeUI {
	private LoginUI loginui = null;
	private JFrame frame;
	
	private static final String GREEN = "Green";
	private static final String RED = "Red";
	
	private CTCOfficeControl ctcOffice = null;
	private JComboBox<Integer> trainComboBox = null;
	private JComboBox<Integer> trackComboBox = null;
	
	//Map panels
	private JTabbedPane tabbedPane = null;
	
	JCheckBox chckbxEnableSimulation = null;
	
	//Train Panel labels
	private JLabel jlbTrainBlock = null;
	private JLabel jlbTrainAuthority = null;
	private JLabel jlbTrainSpeed = null;
	private JLabel jlbTrainCount = null;
	private JLabel jlbTrainRoute = null; 
	private JLabel isFullLabel = null;
	
	//Track Panel Labels
	private JLabel jlbTrackGrade = null;
	private JLabel jlbTrackBlkElevation = null;
	private JLabel jlbTrackSpeedLimit = null;
	private JLabel jlbTrackLength  = null;
	private JLabel jlbTrackSection = null;
	private JLabel jlbInfrastructure = null;
	
	private ImagePanel ip = null;
	
	private Date date = new Date(System.currentTimeMillis());
	private JLabel time = new JLabel(date.toString());
	
	private int trainIdCounter = 0;
	
	private JFrame trainControllerPanel = null;
	private JFrame trackControllerPanel = null;
	private JFrame trainModelPanel = null;
	private JFrame trackModelPanel = null;
	
	private ArrayList<String> destinations = null;
	
	private String currentTrackFocus = GREEN;
	
	private final String TRACK_LAYOUT_DIRECTORY = "C:\\Users\\trottj\\Dropbox\\CoE_1866\\Code\\TrackModel";
	//private final String TRACK_LAYOUT_DIRECTORY = "C:\\Users\\beersa\\Dropbox\\CoE_1866\\Code\\TrackModel";
	
	private boolean addingToTrackJCombo = false;
	
	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		final CTCOfficeUI app = new CTCOfficeUI();
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					if(app.loginui.showLogin()){
						System.out.println("Successfully Logged in");
						app.initTrack();
						app.initialize();
						app.show(true);
					}
					else{
						System.out.println("Failed to log in, system will close");
						System.exit(0);
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
		app.update();
	}

	/**
	 * Create the application.
	 */
	public CTCOfficeUI() {
		loginui = new LoginUI();
		loginui.setModal(true);
		ctcOffice = new CTCOfficeControl();
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		destinations = new ArrayList<String>();
		destinations.add("DEST_1");
		destinations.add("DEST_2");
		frame = new JFrame("CTC Office");
		frame.setBounds(100, 100, 1337, 962);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().setLayout(new MigLayout("", "[469px][grow][grow][grow][566px,grow]", "[301px,grow][352px,grow][160px,grow]"));
		
		//ImagePanel panel_4 = new ImagePanel("C:\\Users\\Jim\\Dropbox\\CoE_1866\\Images\\TrackLayout.png");
		JPanel panel_4 = new JPanel();
		panel_4.setBorder(new TitledBorder(null, "Track Layout", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		frame.getContentPane().add(panel_4, "cell 0 0 4 2,grow");
		panel_4.setLayout(new MigLayout("", "[grow]", "[grow]"));
		
		ip = new ImagePanel();
		ip.setCanvas("base");
		panel_4.add(ip, "cell 0 0,grow");
		
		JPanel panel_5 = new JPanel();
		panel_5.setBorder(new TitledBorder(UIManager.getBorder("TitledBorder.border"), "Train Interface", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		frame.getContentPane().add(panel_5, "cell 4 0,grow");
		panel_5.setLayout(new MigLayout("", "[grow]", "[grow]"));
		
		JPanel panel_2 = new JPanel();
		panel_5.add(panel_2, "cell 0 0,grow");
		panel_2.setLayout(new MigLayout("", "[][][][][][][][][][][][][][][grow]", "[][][][][][][][][]"));
		
		JLabel lblTrainId = new JLabel("Train ID:");
		panel_2.add(lblTrainId, "cell 0 1 2 1,alignx left");
		
		trainComboBox = new JComboBox<Integer>();
		trainComboBox.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent e) {
				Integer item = (Integer)trainComboBox.getSelectedItem();
				jlbTrainAuthority.setText(Integer.toString(ctcOffice.getAuthority(item)));
				jlbTrainSpeed.setText(Double.toString(Math.ceil(ctcOffice.getSpeed(item))));
				jlbTrainBlock.setText(Integer.toString(ctcOffice.getBlock(item)));
				//System.out.println("CTCOFFICE calls GETBLOCK in COMBOBOXLISTENER");
				jlbTrainCount.setText(Integer.toString(ctcOffice.getPassengerCount(item)));
				jlbTrainRoute.setText(ctcOffice.getRoute(item));
			}
			
		});
		
		panel_2.add(trainComboBox, "cell 2 1,growx");
		
		JLabel lblSpeed = new JLabel("Speed:");
		panel_2.add(lblSpeed, "cell 0 2,alignx left");
		
		jlbTrainSpeed = new JLabel("<Speed>");
		panel_2.add(jlbTrainSpeed, "cell 2 2");
		
		Component verticalStrut = Box.createVerticalStrut(20);
		panel_2.add(verticalStrut, "cell 7 2");
		
		JLabel authoritylabel = new JLabel("Authority:");
		panel_2.add(authoritylabel, "cell 0 3 2 1");
		
		jlbTrainAuthority = new JLabel("<Authority>");
		panel_2.add(jlbTrainAuthority, "cell 2 3 2 1");
		
		Component verticalStrut_1 = Box.createVerticalStrut(20);
		panel_2.add(verticalStrut_1, "cell 7 3");
		
		JLabel lblBlock = new JLabel("Block:");
		panel_2.add(lblBlock, "cell 0 4 2 1,alignx left");
		
		jlbTrainBlock = new JLabel("<Block>");
		panel_2.add(jlbTrainBlock, "cell 2 4");
		
		JLabel lblRoute_1 = new JLabel("Route:");
		panel_2.add(lblRoute_1, "cell 0 5");
		
		jlbTrainRoute = new JLabel("<Route>");
		panel_2.add(jlbTrainRoute, "cell 2 5");
		
		JLabel lblPassengerCount = new JLabel("# Passengers:");
		panel_2.add(lblPassengerCount, "cell 0 6 2 1");
		
		jlbTrainCount = new JLabel("<Count>");
		panel_2.add(jlbTrainCount, "cell 2 6");
		
		JButton btnSendRequest = new JButton("Send Request");
		btnSendRequest.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent e) {
				try{
					Request req = new Request(getCenterX(), getCenterY(),(Integer)trainComboBox.getSelectedItem());
					if(req.showRequest()){
						ctcOffice.sendSpeedRequest((Integer)trainComboBox.getSelectedItem(), req.getSpeedRequest());
						ctcOffice.sendAuthorityRequest((Integer)trainComboBox.getSelectedItem(), req.getSpeedRequest());
					}
					else{
						//JOptionPane.showMessageDialog(new JFrame(), "Can only input numeric values.");
					}
				}
				catch(NullPointerException npe){
					JOptionPane.showMessageDialog(new JFrame(), "Error loading request screen. Make sure you've selected a train.");
				}
				
			}
			
		});
		panel_2.add(btnSendRequest, "cell 2 7");
		
		Component horizontalStrut_76 = Box.createHorizontalStrut(20);
		panel_2.add(horizontalStrut_76, "cell 14 7");
		
		Component horizontalStrut_64 = Box.createHorizontalStrut(20);
		panel_2.add(horizontalStrut_64, "cell 0 8");
		
		Component horizontalStrut_65 = Box.createHorizontalStrut(20);
		panel_2.add(horizontalStrut_65, "cell 1 8");
		
		Component horizontalStrut_66 = Box.createHorizontalStrut(20);
		panel_2.add(horizontalStrut_66, "cell 2 8");
		
		Component horizontalStrut_67 = Box.createHorizontalStrut(20);
		panel_2.add(horizontalStrut_67, "cell 3 8");
		
		Component horizontalStrut_68 = Box.createHorizontalStrut(20);
		panel_2.add(horizontalStrut_68, "cell 4 8");
		
		Component horizontalStrut_69 = Box.createHorizontalStrut(20);
		panel_2.add(horizontalStrut_69, "cell 5 8");
		
		Component horizontalStrut_70 = Box.createHorizontalStrut(20);
		panel_2.add(horizontalStrut_70, "cell 6 8");
		
		Component horizontalStrut_71 = Box.createHorizontalStrut(20);
		panel_2.add(horizontalStrut_71, "cell 7 8");
		
		Component horizontalStrut_72 = Box.createHorizontalStrut(20);
		panel_2.add(horizontalStrut_72, "cell 8 8");
		
		Component horizontalStrut_73 = Box.createHorizontalStrut(20);
		panel_2.add(horizontalStrut_73, "cell 9 8");
		
		Component horizontalStrut_74 = Box.createHorizontalStrut(20);
		panel_2.add(horizontalStrut_74, "cell 10 8");
		
		Component horizontalStrut_75 = Box.createHorizontalStrut(20);
		panel_2.add(horizontalStrut_75, "cell 11 8");
		
		JButton btnDispatchTrain = new JButton("Dispatch New Train");
		btnDispatchTrain.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				TrainDispatcher td = new TrainDispatcher(getCenterX(), getCenterY(), trainIdCounter);
				if(td.showDispatcher()){
					if(ctcOffice.dispatchTrain(td.getID(), td.getAuthority(), td.getRoute())){
						trainComboBox.addItem(td.getID());
						trainComboBox.setSelectedItem(td.getID());
						trainIdCounter++;
					}
					else{
						JOptionPane.showMessageDialog(new JFrame(), "Error dispatching new train. Make sure new train ID is unique.");
					}
				}
				
			}
			
		});
		panel_2.add(btnDispatchTrain, "cell 12 8,alignx center");
		
		JPanel panel_8 = new JPanel();
		panel_8.setBorder(new TitledBorder(UIManager.getBorder("TitledBorder.border"), "System Status", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		frame.getContentPane().add(panel_8, "cell 0 2 4 1,grow");
		panel_8.setLayout(new MigLayout("", "[469px]", "[160px,grow]"));
		
		JPanel panel_1 = new JPanel();
		panel_8.add(panel_1, "cell 0 0,grow");
		panel_1.setLayout(new MigLayout("", "[][]", "[][][][][][]"));
		
		Component verticalStrut_54 = Box.createVerticalStrut(20);
		panel_1.add(verticalStrut_54, "cell 0 0");
		
		JLabel lblNewLabel = new JLabel("Track Control:");
		panel_1.add(lblNewLabel, "cell 0 1");
		
		JLabel TCStatusVariable = new JLabel("<TC Status>");
		panel_1.add(TCStatusVariable, "cell 1 1");
		
		JLabel lblTrainControl = new JLabel("Train Control:");
		panel_1.add(lblTrainControl, "cell 0 2");
		
		JLabel TNCStatusVariable = new JLabel("<TNC Status>");
		panel_1.add(TNCStatusVariable, "cell 1 2");
		
		JLabel lblTrackModel = new JLabel("Track Model:");
		panel_1.add(lblTrackModel, "cell 0 3");
		
		JLabel TMStatusVariable = new JLabel("<TM Status");
		panel_1.add(TMStatusVariable, "cell 1 3");
		
		JLabel lblTrainModel = new JLabel("Train Model:");
		panel_1.add(lblTrainModel, "cell 0 4");
		
		JLabel TNMStatusVariable = new JLabel("<TNM Status>");
		panel_1.add(TNMStatusVariable, "cell 1 4");
		
		JPanel panel_6 = new JPanel();
		panel_6.setBorder(new TitledBorder(UIManager.getBorder("TitledBorder.border"), "Track Segment", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		frame.getContentPane().add(panel_6, "cell 4 1,grow");
		panel_6.setLayout(new MigLayout("", "[grow]", "[grow]"));
		
		JPanel panel_3 = new JPanel();
		panel_6.add(panel_3, "cell 0 0,grow");
		panel_3.setLayout(new MigLayout("", "[][][][][][][][][][grow]", "[][][][][][][][][][]"));
		
		Component verticalStrut_26 = Box.createVerticalStrut(20);
		panel_3.add(verticalStrut_26, "cell 9 0");
		
		JLabel lblTrackSegment = new JLabel("Segment:");
		panel_3.add(lblTrackSegment, "cell 0 1 2 1");
		
		trackComboBox = new JComboBox<Integer>();
		trackComboBox.setEditable(true);
		for(int i = 0; i < 150; i++){
			trackComboBox.addItem(i);
		}
		trackComboBox.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				if(!addingToTrackJCombo){
					jlbTrackGrade.setText(Double.toString(ctcOffice.getBlockGrade(currentTrackFocus, (Integer)trackComboBox.getSelectedItem())));
					jlbTrackBlkElevation.setText(Double.toString(ctcOffice.getBlockElevation(currentTrackFocus, (Integer)trackComboBox.getSelectedItem())));
					jlbTrackSpeedLimit.setText(ctcOffice.getBlockSpeedLimit(currentTrackFocus, (Integer)trackComboBox.getSelectedItem()).toString());
					jlbTrackLength.setText(Double.toString(ctcOffice.getBlockLength(currentTrackFocus, (Integer)trackComboBox.getSelectedItem())));
					jlbInfrastructure.setText(ctcOffice.getBlockInfrastructure(currentTrackFocus, (Integer)trackComboBox.getSelectedItem()));
					isFullLabel.setText((Boolean.toString(ctcOffice.getBlockFull(currentTrackFocus, (Integer)trackComboBox.getSelectedItem()))));
				}
			}
			
		});
		panel_3.add(trackComboBox, "cell 2 1,growx");
		
		Component horizontalStrut_78 = Box.createHorizontalStrut(20);
		panel_3.add(horizontalStrut_78, "cell 3 1");
		
		Component horizontalStrut_77 = Box.createHorizontalStrut(20);
		panel_3.add(horizontalStrut_77, "cell 4 1");
		
		Component verticalStrut_27 = Box.createVerticalStrut(20);
		panel_3.add(verticalStrut_27, "cell 9 1");
		
		JLabel lblFull = new JLabel("Full:");
		panel_3.add(lblFull, "cell 0 2");
		
		isFullLabel = new JLabel("<Full>");
		panel_3.add(isFullLabel, "cell 2 2");
		
		Component verticalStrut_2 = Box.createVerticalStrut(20);
		panel_3.add(verticalStrut_2, "cell 7 2");
		
		JLabel lblGrade = new JLabel("Grade:");
		panel_3.add(lblGrade, "cell 0 3 2 1");
		
		jlbTrackGrade = new JLabel("<Grade>");
		panel_3.add(jlbTrackGrade, "cell 2 3 2 1");
		
		Component verticalStrut_3 = Box.createVerticalStrut(20);
		panel_3.add(verticalStrut_3, "cell 7 3");
		
		JLabel elevation = new JLabel("Elevation:");
		panel_3.add(elevation, "cell 0 4 2 1");
		
		jlbTrackBlkElevation = new JLabel("<Elevation>");
		panel_3.add(jlbTrackBlkElevation, "cell 2 4 2 1");
		
		Component verticalStrut_28 = Box.createVerticalStrut(20);
		panel_3.add(verticalStrut_28, "cell 9 4");
		
		JLabel jlbtrackspeed = new JLabel("Speed Limit:");
		panel_3.add(jlbtrackspeed, "cell 0 5 2 1");
		
		jlbTrackSpeedLimit = new JLabel("<SpeedLimit>");
		panel_3.add(jlbTrackSpeedLimit, "cell 2 5 2 1");
		
		Component verticalStrut_29 = Box.createVerticalStrut(20);
		panel_3.add(verticalStrut_29, "cell 9 5");
		
		JLabel trackLength = new JLabel("Length:");
		panel_3.add(trackLength , "cell 0 6 2 1");
		
		jlbTrackLength = new JLabel("<Length>");
		panel_3.add(jlbTrackLength, "cell 2 6 2 1");
		
		Component verticalStrut_30 = Box.createVerticalStrut(20);
		panel_3.add(verticalStrut_30, "cell 9 6");
		
		JLabel lblRoute = new JLabel("Section:");
		panel_3.add(lblRoute, "cell 0 7 2 1");
		
		jlbTrackSection = new JLabel("<Section>");
		panel_3.add(jlbTrackSection, "cell 2 7 2 1");
		
		Component verticalStrut_31 = Box.createVerticalStrut(20);
		panel_3.add(verticalStrut_31, "cell 9 7");
		
		JLabel jlbInfraLabel = new JLabel("Infrastructure:");
		panel_3.add(jlbInfraLabel, "cell 0 8 2 1");
		
		jlbInfrastructure = new JLabel("<Infrastructure>");
		panel_3.add(jlbInfrastructure, "cell 2 8 3 1");
		
		Component horizontalStrut_26 = Box.createHorizontalStrut(20);
		panel_3.add(horizontalStrut_26, "cell 6 8");
		
		Component horizontalStrut_27 = Box.createHorizontalStrut(20);
		panel_3.add(horizontalStrut_27, "cell 7 8");
		
		Component horizontalStrut_28 = Box.createHorizontalStrut(20);
		panel_3.add(horizontalStrut_28, "cell 8 8");
		
		JCheckBox chckbxCloseSegment = new JCheckBox("Close Segment");
		panel_3.add(chckbxCloseSegment, "cell 9 8,alignx center");
		
		Component horizontalStrut_20 = Box.createHorizontalStrut(20);
		panel_3.add(horizontalStrut_20, "cell 0 9");
		
		Component horizontalStrut_21 = Box.createHorizontalStrut(20);
		panel_3.add(horizontalStrut_21, "cell 1 9");
		
		Component horizontalStrut_22 = Box.createHorizontalStrut(20);
		panel_3.add(horizontalStrut_22, "cell 2 9");
		
		Component horizontalStrut_23 = Box.createHorizontalStrut(20);
		panel_3.add(horizontalStrut_23, "cell 3 9");
		
		Component horizontalStrut_24 = Box.createHorizontalStrut(20);
		panel_3.add(horizontalStrut_24, "cell 4 9");
		
		Component horizontalStrut_25 = Box.createHorizontalStrut(20);
		panel_3.add(horizontalStrut_25, "cell 5 9");
		
		JToggleButton tglbtnOpencloseSegment = new JToggleButton("Open/Close Segment");
		tglbtnOpencloseSegment.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				//TODO close track segment
				
			}
			
		});
		panel_3.add(tglbtnOpencloseSegment, "cell 9 9,alignx center");
		
		JPanel panel_7 = new JPanel();
		frame.getContentPane().add(panel_7, "cell 4 2,grow");
		panel_7.setLayout(new MigLayout("", "[][][][][][][][][][][][][][][][][]", "[][][][][][]"));
		
		Component horizontalStrut_29 = Box.createHorizontalStrut(20);
		panel_7.add(horizontalStrut_29, "cell 0 0");
		
		Component horizontalStrut_30 = Box.createHorizontalStrut(20);
		panel_7.add(horizontalStrut_30, "cell 1 0");
		
		Component horizontalStrut_31 = Box.createHorizontalStrut(20);
		panel_7.add(horizontalStrut_31, "cell 2 0");
		
		Component horizontalStrut_32 = Box.createHorizontalStrut(20);
		panel_7.add(horizontalStrut_32, "cell 3 0");
		
		Component horizontalStrut_33 = Box.createHorizontalStrut(20);
		panel_7.add(horizontalStrut_33, "cell 4 0");
		
		Component horizontalStrut_34 = Box.createHorizontalStrut(20);
		panel_7.add(horizontalStrut_34, "cell 5 0");
		
		Component horizontalStrut_35 = Box.createHorizontalStrut(20);
		panel_7.add(horizontalStrut_35, "cell 6 0");
		
		Component horizontalStrut_36 = Box.createHorizontalStrut(20);
		panel_7.add(horizontalStrut_36, "cell 7 0");
		
		Component horizontalStrut_37 = Box.createHorizontalStrut(20);
		panel_7.add(horizontalStrut_37, "cell 8 0");
		
		Component horizontalStrut_38 = Box.createHorizontalStrut(20);
		panel_7.add(horizontalStrut_38, "cell 9 0");
		
		Component horizontalStrut_39 = Box.createHorizontalStrut(20);
		panel_7.add(horizontalStrut_39, "cell 10 0");
		
		Component horizontalStrut_40 = Box.createHorizontalStrut(20);
		panel_7.add(horizontalStrut_40, "cell 11 0");
		
		Component horizontalStrut_41 = Box.createHorizontalStrut(20);
		panel_7.add(horizontalStrut_41, "cell 12 0");
		
		Component horizontalStrut_42 = Box.createHorizontalStrut(20);
		panel_7.add(horizontalStrut_42, "cell 13 0");
		
		Component horizontalStrut_43 = Box.createHorizontalStrut(20);
		panel_7.add(horizontalStrut_43, "cell 14 0");
		
		JButton btnNewButton = new JButton("Track Controller Panel\r\n");
		btnNewButton.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent e) {
				trackControllerPanel.setVisible(true);
			}
			
		});
		btnNewButton.setToolTipText("This will display the Track Controller's UI in a seperate window.");
		//panel_7.add(btnNewButton, "cell 0 1,alignx center");
		
		JButton btnNewButton_1 = new JButton("Train Controller Panel");
		btnNewButton_1.setToolTipText("This will display the Train Controller's UI in a seperate window.");
		btnNewButton_1.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent e) {
				trainControllerPanel.pack();
				trainControllerPanel.setVisible(true);
				
			}
			
		});
		panel_7.add(btnNewButton_1, "cell 0 2,alignx center");
		
		JButton btnNewButton_3 = new JButton("Track Model Panel");
		btnNewButton_3.setToolTipText("This will display the Track Model's UI in a seperate window.");
		btnNewButton_3.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent e) {
				trackModelPanel.pack();
				trackModelPanel.setVisible(true);
			}
			
		});
		panel_7.add(btnNewButton_3, "cell 0 3,growx");
		date = new Date(System.currentTimeMillis());
		Timer timer = new Timer(1000, new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent e) {
				date.setTime(System.currentTimeMillis());
				time.setText(date.toString());
			}
			
		});
		timer.start();
		
		JButton btnNewButton_2 = new JButton("Train Model Panel");
		btnNewButton_2.setToolTipText("This will display the Train Model's UI in a seperate window.");
		btnNewButton_2.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent e) {
				trainModelPanel.pack();
				trainModelPanel.setVisible(true);
				
			}
			
		});
		//panel_7.add(btnNewButton_2, "cell 0 4,growx");
		
		Component verticalStrut_55 = Box.createVerticalStrut(20);
		panel_7.add(verticalStrut_55, "cell 16 4");
		
		chckbxEnableSimulation = new JCheckBox("Enable Simulation");
		chckbxEnableSimulation.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				if(chckbxEnableSimulation.isSelected()){
					ctcOffice.simulationMode(10);
				}
				else{
					ctcOffice.simulationMode(1);
				}
				
			}
			
		});
		panel_7.add(chckbxEnableSimulation, "cell 0 5");
		
		JPanel panel_10 = new JPanel();
		panel_10.setBorder(new TitledBorder(UIManager.getBorder("TitledBorder.border"), "System Time", TitledBorder.RIGHT, TitledBorder.BOTTOM, null, null));
		panel_7.add(panel_10, "cell 10 5 7 1,grow");
		panel_10.setLayout(new MigLayout("", "[][][][]", "[]"));
		time = new JLabel(date.toString());
		panel_10.add(time, "cell 0 0 4 1,alignx right");
		
		JMenuBar menuBar = new JMenuBar();
		frame.setJMenuBar(menuBar);
		
		JMenu mnFile = new JMenu("File");
		menuBar.add(mnFile);
		
		JMenuItem mntmSignOut = new JMenuItem("Sign Out");
		mntmSignOut.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				show(false);
				loginui.clear();
				if(loginui.showLogin()){
					System.out.println("Successfully Logged in");
					show(true);
				}
				else{
					System.out.println("Failed to log in, system will close");
					System.exit(0);
				}
			}
			
		});
		mnFile.add(mntmSignOut);
		
		JMenuItem mntmShutdown = new JMenuItem("Shutdown");
		mntmShutdown.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				System.exit(0);
			}
			
		});
		mnFile.add(mntmShutdown);
		
		JMenu mnOptions = new JMenu("Options");
		menuBar.add(mnOptions);
	}

	
	public void show(boolean value){
		frame.setVisible(value);
	}
	/**
	 * Update Function, this is looped until the application is exited
	 * It will go through each module and allow it to update.
	 */
	public void update(){
		while(true){
			ctcOffice.update();
			try {
				if(chckbxEnableSimulation != null){
					if( chckbxEnableSimulation.isSelected()){
						Thread.sleep(100);
					}
					else{
						Thread.sleep(1000);
					}
				}
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			ArrayList<Integer> removedTrains = ctcOffice.getRemovedTrains();
			for(Integer i : removedTrains){
				trainComboBox.removeItem(i);
				ctcOffice.removeTrain(i);
				if(ctcOffice.getRoute(i).compareToIgnoreCase(GREEN) == 0){
					ip.removeImage("TRG" + i);
				}
				else{
					ip.removeImage("TR"+i);
				}
			}
			Set<Integer> trains = ctcOffice.getTrainIDs();
			for(Integer i : trains){
				if(ctcOffice.getRoute(i).compareToIgnoreCase(GREEN) == 0){
					ip.addImage("TG" + ctcOffice.getBlock(i));
					System.out.println("CTCOFFICE calls GETBLOCK in UPDATE");
				}
				else{
					ip.addImage("TR"+ctcOffice.getBlock(i));
					System.out.println("CTCOFFICE calls GETBLOCK in UPDATE");
				}
			}
			if(trainComboBox != null){
				if(trainComboBox.getItemCount() > 0){
					Integer item = (Integer)trainComboBox.getSelectedItem();
					jlbTrainAuthority.setText(Integer.toString(ctcOffice.getAuthority(item)));
					jlbTrainSpeed.setText(Double.toString(Math.ceil(ctcOffice.getSpeed(item))));
					jlbTrainBlock.setText(Integer.toString(ctcOffice.getBlock(item)));
					//System.out.println("CTCOFFICE calls GETBLOCK in COMBOBOXLISTENER");
					jlbTrainCount.setText(Integer.toString(ctcOffice.getPassengerCount(item)));
					jlbTrainRoute.setText(ctcOffice.getRoute(item));
				}
			}
			if(ip != null){
				ip.repaint();
			}
		}
	}
	
	public void close(){
		WindowEvent wev = new WindowEvent(frame, WindowEvent.WINDOW_CLOSING);
		Toolkit.getDefaultToolkit().getSystemEventQueue().postEvent(wev);
	}
	/**
	 * This loads the track into the Track Model
	 * The user must select both track files for the app to run properly
	 */
	public void initTrack(){
		JFileChooser fc = new JFileChooser();
		fc.setFileSelectionMode(JFileChooser.FILES_AND_DIRECTORIES);
		fc.setMultiSelectionEnabled(true);
		fc.setCurrentDirectory(new File(TRACK_LAYOUT_DIRECTORY));
		int retval = fc.showOpenDialog(new JPanel());

		if (retval == JFileChooser.APPROVE_OPTION) {
			ctcOffice.initTrack(fc.getSelectedFiles());
			getPanels();
		}
		else{
			System.exit(0);
		}
	}
	
	private void getPanels(){
		trainControllerPanel = ctcOffice.getTrainControllerPanel();
		trackControllerPanel = ctcOffice.getTrackControllerPanel();
		trainModelPanel = ctcOffice.getTrainModelPanel();
		trackModelPanel = ctcOffice.getTrackModelPanel();
	}
	
	private int getCenterX(){
		return frame.getX() + frame.getWidth()/2;
	}
	
	private int getCenterY(){
		return frame.getY() + frame.getHeight()/2;
	}
}
