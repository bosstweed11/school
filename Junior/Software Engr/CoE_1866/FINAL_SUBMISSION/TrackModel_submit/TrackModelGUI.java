/**************************************************
	Author:		Zach Olawski
	Course:		CoE_1186
	Detail:		Final Project
					Track Model Module
					User Interface Layout
**************************************************/

import javax.swing.JPanel;
import java.awt.GridBagLayout;
import java.awt.BorderLayout;
import javax.swing.JRadioButton;
import java.awt.GridBagConstraints;
import java.awt.Insets;
import javax.swing.ButtonGroup;
import javax.swing.JComboBox;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.border.TitledBorder;
import java.awt.Component;
import javax.swing.Box;
import java.awt.GridLayout;
import javax.swing.SwingConstants;
import javax.swing.BoxLayout;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.io.File;

import javax.swing.UIManager;


public class TrackModelGUI extends JPanel{

	private
		final ButtonGroup buttonGroup = new ButtonGroup();
		JRadioButton rdbtnGreen, rdbtnRed;
		JButton btnLoadTrack, updateViewButton, createFailureButton, createRemoveFailureButton, heaterButton;
		JPanel trackModelPanel, viewPanel, failurePanel, statsPanel, statusPanel, logPanel, trackPanel;
		JLabel viewLineLabel, viewBlockLabel, status, blockStatus, section, blockSection;
		JLabel speed, blockSpeed, length, blockLength, grade, blockGrade;
		JLabel elevation, blockElevation, power, blockPower, blockHeater;
		JLabel failureTypeLabel, failureBlockLabel, lblAllClear, lblTrackLog, lblWaiting;
		JLabel infrastructure, blockInfrastructure;
		JComboBox<String> viewBlockBox;
		JComboBox<String> failureTypeBox;
		JComboBox<String> failureBlockBox;
		String failureString[] = {"Broken Rail", "Track Circuit Failure", "Power Failure"};	
		TrackModel myTrackModelData;
		final String TRACK_LAYOUT_DIRECTORY = "C:\\Users\\trottj\\Dropbox\\CoE_1866\\Code\\TrackModel";
		ImagePanel ip;
	
	public TrackModelGUI(TrackModel myTMD){
		myTrackModelData = myTMD;
		ip = new ImagePanel();
		ip.setCanvas("base");
		setLayout(new BorderLayout(0, 0));
		
		trackModelPanel = new JPanel();
		add(trackModelPanel, BorderLayout.CENTER);
		GridBagLayout gbl_panel = new GridBagLayout();
		gbl_panel.columnWidths = new int[]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		gbl_panel.rowHeights = new int[]{0, 0, 87, 0, 0, 8, 0};
		gbl_panel.columnWeights = new double[]{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, Double.MIN_VALUE};
		gbl_panel.rowWeights = new double[]{1.0, 0.0, 0.0, 0.0, 0.0, 0.0, Double.MIN_VALUE};
		trackModelPanel.setLayout(gbl_panel);
		
		btnLoadTrack = new JButton("Load Track");
		btnLoadTrack.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if(e.getSource() == btnLoadTrack){
					initTrack();
				}
			}
		});
		GridBagConstraints gbc_btnLoadTrack = new GridBagConstraints();
		gbc_btnLoadTrack.fill = GridBagConstraints.HORIZONTAL;
		gbc_btnLoadTrack.gridwidth = 8;
		gbc_btnLoadTrack.insets = new Insets(0, 0, 5, 5);
		gbc_btnLoadTrack.gridx = 0;
		gbc_btnLoadTrack.gridy = 0;
		trackModelPanel.add(btnLoadTrack, gbc_btnLoadTrack);
		/*
		trackPanel = new JPanel();
		trackPanel.setBorder(new TitledBorder(UIManager.getBorder("TitledBorder.border"), "Track", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		GridBagConstraints gbc_trackPanel = new GridBagConstraints();
		gbc_trackPanel.gridheight = 6;
		gbc_trackPanel.insets = new Insets(0, 0, 5, 0);
		gbc_trackPanel.fill = GridBagConstraints.BOTH;
		gbc_trackPanel.gridx = 9;
		gbc_trackPanel.gridy = 0;
		trackPanel.add(ip, "cell 0 0,grow");
		trackModelPanel.add(trackPanel, gbc_trackPanel);
		*/
		viewPanel = new JPanel();
		viewPanel.setBorder(new TitledBorder(UIManager.getBorder("TitledBorder.border"), "View", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		GridBagConstraints gbc_viewPanel = new GridBagConstraints();
		gbc_viewPanel.fill = GridBagConstraints.BOTH;
		gbc_viewPanel.insets = new Insets(0, 0, 5, 5);
		gbc_viewPanel.gridx = 0;
		gbc_viewPanel.gridy = 1;
		trackModelPanel.add(viewPanel, gbc_viewPanel);
		GridBagLayout gbl_viewPanel = new GridBagLayout();
		gbl_viewPanel.columnWidths = new int[]{0, 0, 0, 0};
		gbl_viewPanel.rowHeights = new int[]{0, 0, 0, 0};
		gbl_viewPanel.columnWeights = new double[]{0.0, 1.0, 0.0, Double.MIN_VALUE};
		gbl_viewPanel.rowWeights = new double[]{0.0, 0.0, 0.0, Double.MIN_VALUE};
		viewPanel.setLayout(gbl_viewPanel);
		
		viewLineLabel = new JLabel("Line");
		GridBagConstraints gbc_viewLineLabel = new GridBagConstraints();
		gbc_viewLineLabel.insets = new Insets(0, 0, 5, 5);
		gbc_viewLineLabel.gridx = 0;
		gbc_viewLineLabel.gridy = 0;
		viewPanel.add(viewLineLabel, gbc_viewLineLabel);
		
		rdbtnGreen = new JRadioButton("Green");
		rdbtnGreen.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if(rdbtnGreen.isSelected()){
					if(viewBlockBox.getItemCount() == 0){
						for(int i = 1; i <= 150; ++i){
							viewBlockBox.addItem("" + i);
							failureBlockBox.addItem("" + i);
						}
					}
					else{
						for(int i = 78; i <= 150; ++i){
							viewBlockBox.addItem("" + i);
							failureBlockBox.addItem("" + i);
						}
					}
				}
			}
		});
		GridBagConstraints gbc_rdbtnGreen = new GridBagConstraints();
		gbc_rdbtnGreen.insets = new Insets(0, 0, 5, 5);
		gbc_rdbtnGreen.gridx = 1;
		gbc_rdbtnGreen.gridy = 0;
		viewPanel.add(rdbtnGreen, gbc_rdbtnGreen);
		buttonGroup.add(rdbtnGreen);
		
		rdbtnRed = new JRadioButton("Red");
		rdbtnRed.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if(rdbtnRed.isSelected()){
					if(viewBlockBox.getItemCount() == 0){
						for(int i = 1; i <= 77; ++i){
							viewBlockBox.addItem("" + i);
							failureBlockBox.addItem("" + i);
						}
					}
					else{
						for(int i = 78; i <= 150; ++i){
							viewBlockBox.removeItem("" + i);
							failureBlockBox.removeItem("" + i);
						}
					}
				}
			}
		});
		GridBagConstraints gbc_rdbtnRed = new GridBagConstraints();
		gbc_rdbtnRed.insets = new Insets(0, 0, 5, 0);
		gbc_rdbtnRed.gridx = 2;
		gbc_rdbtnRed.gridy = 0;
		viewPanel.add(rdbtnRed, gbc_rdbtnRed);
		buttonGroup.add(rdbtnRed);
		
		viewBlockLabel = new JLabel("Block");
		GridBagConstraints gbc_viewBlockLabel = new GridBagConstraints();
		gbc_viewBlockLabel.anchor = GridBagConstraints.EAST;
		gbc_viewBlockLabel.insets = new Insets(0, 0, 5, 5);
		gbc_viewBlockLabel.gridx = 0;
		gbc_viewBlockLabel.gridy = 1;
		viewPanel.add(viewBlockLabel, gbc_viewBlockLabel);
		
		viewBlockBox = new JComboBox<String>();
		GridBagConstraints gbc_viewBlockBox = new GridBagConstraints();
		gbc_viewBlockBox.fill = GridBagConstraints.HORIZONTAL;
		gbc_viewBlockBox.gridwidth = 2;
		gbc_viewBlockBox.insets = new Insets(0, 0, 5, 0);
		gbc_viewBlockBox.gridx = 1;
		gbc_viewBlockBox.gridy = 1;
		viewPanel.add(viewBlockBox, gbc_viewBlockBox);
		
		updateViewButton = new JButton("Update View");
		updateViewButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if(e.getSource() == updateViewButton){
					String line;
					if(rdbtnGreen.isSelected()){line = "Green";}
						else{line = "Red";}
					int blockNumber = Integer.parseInt((String)viewBlockBox.getSelectedItem());
					int fail[] = (int[])myTrackModelData.getFailure(line, blockNumber);
					String failText = "";
					if(fail[0] == 1){
						failText += "| Broken Rail |";
					}
					if(fail[1] == 1){
						failText += "| Track Circuit |";
					}
					if(fail[2] == 1){
						failText += "| Power Down |";
					}
					if( (fail[0] == 0) && (fail[1] == 0) && (fail[2] == 0) ){
						blockStatus.setText("Clear");
					}
					else{
						blockStatus.setText(failText);
					}
					blockSection.setText(myTrackModelData.section(line, blockNumber));
					blockSpeed.setText(Integer.toString(myTrackModelData.speedLimit(line, blockNumber)));
					blockLength.setText(Double.toString(myTrackModelData.blockLength(line, blockNumber)));
					blockGrade.setText(Double.toString(myTrackModelData.blockGrade(line, blockNumber)));
					blockElevation.setText(Double.toString(myTrackModelData.blockElevation(line, blockNumber)));
					int switchCheck = myTrackModelData.switchPosition(line, blockNumber);
					if(switchCheck < 0){
						blockPower.setText("N/A");
					}
					else{
						blockPower.setText(Integer.toString(switchCheck));
					}
					String heat = Boolean.toString(myTrackModelData.heaterOn(line, blockNumber));
					if(heat.compareTo("true") == 0){
						blockHeater.setText("ON");
					}
					else{
						blockHeater.setText("OFF");
					}
					String inf = myTrackModelData.infrastructure(line, blockNumber);
					if(inf.compareTo("None") == 0){
						blockInfrastructure.setText("Circuit");
					}
					else{
						blockInfrastructure.setText(inf);
					}
				}
			}
		});
		GridBagConstraints gbc_updateViewButton = new GridBagConstraints();
		gbc_updateViewButton.gridwidth = 2;
		gbc_updateViewButton.insets = new Insets(0, 0, 0, 5);
		gbc_updateViewButton.gridx = 1;
		gbc_updateViewButton.gridy = 2;
		viewPanel.add(updateViewButton, gbc_updateViewButton);
		
		statsPanel = new JPanel();
		statsPanel.setBorder(new TitledBorder(UIManager.getBorder("TitledBorder.border"), "Track Stats", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		GridBagConstraints gbc_statsPanel = new GridBagConstraints();
		gbc_statsPanel.gridwidth = 7;
		gbc_statsPanel.insets = new Insets(0, 0, 5, 5);
		gbc_statsPanel.fill = GridBagConstraints.BOTH;
		gbc_statsPanel.gridheight = 2;
		gbc_statsPanel.gridx = 1;
		gbc_statsPanel.gridy = 1;
		trackModelPanel.add(statsPanel, gbc_statsPanel);
		GridBagLayout gbl_statsPanel = new GridBagLayout();
		gbl_statsPanel.columnWidths = new int[]{0, 0, 0, 0};
		gbl_statsPanel.rowHeights = new int[]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		gbl_statsPanel.columnWeights = new double[]{0.0, 0.0, 0.0, Double.MIN_VALUE};
		gbl_statsPanel.rowWeights = new double[]{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, Double.MIN_VALUE};
		statsPanel.setLayout(gbl_statsPanel);
		
		status = new JLabel("Status: ");
		status.setHorizontalAlignment(SwingConstants.LEFT);
		GridBagConstraints gbc_status = new GridBagConstraints();
		gbc_status.insets = new Insets(0, 0, 5, 5);
		gbc_status.gridx = 0;
		gbc_status.gridy = 0;
		statsPanel.add(status, gbc_status);
		
		blockStatus = new JLabel("N/A");
		GridBagConstraints gbc_blockStatus = new GridBagConstraints();
		gbc_blockStatus.insets = new Insets(0, 0, 5, 5);
		gbc_blockStatus.gridx = 1;
		gbc_blockStatus.gridy = 0;
		statsPanel.add(blockStatus, gbc_blockStatus);
		
		section = new JLabel("Section: ");
		section.setHorizontalAlignment(SwingConstants.LEFT);
		GridBagConstraints gbc_section = new GridBagConstraints();
		gbc_section.insets = new Insets(0, 0, 5, 5);
		gbc_section.gridx = 0;
		gbc_section.gridy = 1;
		statsPanel.add(section, gbc_section);
		
		blockSection = new JLabel("N/A");
		GridBagConstraints gbc_blockSection = new GridBagConstraints();
		gbc_blockSection.insets = new Insets(0, 0, 5, 5);
		gbc_blockSection.gridx = 1;
		gbc_blockSection.gridy = 1;
		statsPanel.add(blockSection, gbc_blockSection);
		
		infrastructure = new JLabel("Type: ");
		infrastructure.setHorizontalAlignment(SwingConstants.LEFT);
		GridBagConstraints gbc_infrastructure = new GridBagConstraints();
		gbc_infrastructure.insets = new Insets(0, 0, 5, 5);
		gbc_infrastructure.gridx = 0;
		gbc_infrastructure.gridy = 2;
		statsPanel.add(infrastructure, gbc_infrastructure);
		
		blockInfrastructure = new JLabel("N/A");
		GridBagConstraints gbc_blockInfrastructure = new GridBagConstraints();
		gbc_blockInfrastructure.insets = new Insets(0, 0, 5, 5);
		gbc_blockInfrastructure.gridx = 1;
		gbc_blockInfrastructure.gridy = 2;
		statsPanel.add(blockInfrastructure, gbc_blockInfrastructure);
		
		speed = new JLabel("Speed: ");
		speed.setHorizontalAlignment(SwingConstants.LEFT);
		GridBagConstraints gbc_speed = new GridBagConstraints();
		gbc_speed.insets = new Insets(0, 0, 5, 5);
		gbc_speed.gridx = 0;
		gbc_speed.gridy = 3;
		statsPanel.add(speed, gbc_speed);
		
		blockSpeed = new JLabel("N/A");
		GridBagConstraints gbc_blockSpeed = new GridBagConstraints();
		gbc_blockSpeed.insets = new Insets(0, 0, 5, 5);
		gbc_blockSpeed.gridx = 1;
		gbc_blockSpeed.gridy = 3;
		statsPanel.add(blockSpeed, gbc_blockSpeed);
		
		length = new JLabel("Length: ");
		length.setHorizontalAlignment(SwingConstants.LEFT);
		GridBagConstraints gbc_length = new GridBagConstraints();
		gbc_length.insets = new Insets(0, 0, 5, 5);
		gbc_length.gridx = 0;
		gbc_length.gridy = 4;
		statsPanel.add(length, gbc_length);
		
		blockLength = new JLabel("N/A");
		GridBagConstraints gbc_blockLength = new GridBagConstraints();
		gbc_blockLength.insets = new Insets(0, 0, 5, 5);
		gbc_blockLength.gridx = 1;
		gbc_blockLength.gridy = 4;
		statsPanel.add(blockLength, gbc_blockLength);
		
		grade = new JLabel("Grade: ");
		grade.setHorizontalAlignment(SwingConstants.LEFT);
		GridBagConstraints gbc_grade = new GridBagConstraints();
		gbc_grade.insets = new Insets(0, 0, 5, 5);
		gbc_grade.gridx = 0;
		gbc_grade.gridy = 5;
		statsPanel.add(grade, gbc_grade);
		
		blockGrade = new JLabel("N/A");
		GridBagConstraints gbc_blockGrade = new GridBagConstraints();
		gbc_blockGrade.insets = new Insets(0, 0, 5, 5);
		gbc_blockGrade.gridx = 1;
		gbc_blockGrade.gridy = 5;
		statsPanel.add(blockGrade, gbc_blockGrade);
		
		elevation = new JLabel("Elevation: ");
		elevation.setHorizontalAlignment(SwingConstants.LEFT);
		GridBagConstraints gbc_elevation = new GridBagConstraints();
		gbc_elevation.insets = new Insets(0, 0, 5, 5);
		gbc_elevation.gridx = 0;
		gbc_elevation.gridy = 6;
		statsPanel.add(elevation, gbc_elevation);
		
		blockElevation = new JLabel("N/A");
		GridBagConstraints gbc_blockElevation = new GridBagConstraints();
		gbc_blockElevation.insets = new Insets(0, 0, 5, 5);
		gbc_blockElevation.gridx = 1;
		gbc_blockElevation.gridy = 6;
		statsPanel.add(blockElevation, gbc_blockElevation);
		
		power = new JLabel("Switch: ");
		power.setHorizontalAlignment(SwingConstants.LEFT);
		GridBagConstraints gbc_power = new GridBagConstraints();
		gbc_power.insets = new Insets(0, 0, 5, 5);
		gbc_power.gridx = 0;
		gbc_power.gridy = 7;
		statsPanel.add(power, gbc_power);
		
		blockPower = new JLabel("N/A");
		GridBagConstraints gbc_blockPower = new GridBagConstraints();
		gbc_blockPower.insets = new Insets(0, 0, 5, 5);
		gbc_blockPower.gridx = 1;
		gbc_blockPower.gridy = 7;
		statsPanel.add(blockPower, gbc_blockPower);
		
		heaterButton = new JButton("Heater: ");
		heaterButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if(e.getSource() == heaterButton){
					int blockNumber = Integer.parseInt((String)viewBlockBox.getSelectedItem());
					if(rdbtnGreen.isSelected()){
						boolean set = myTrackModelData.toggleHeater("Green", blockNumber);
						blockHeater.setText(Boolean.toString(set));
						return;
					}
					boolean set = myTrackModelData.toggleHeater("Red", blockNumber);
					blockHeater.setText(Boolean.toString(set));
				}
			}
		});
		GridBagConstraints gbc_heaterButton = new GridBagConstraints();
		gbc_heaterButton.insets = new Insets(0, 0, 5, 5);
		gbc_heaterButton.gridx = 0;
		gbc_heaterButton.gridy = 8;
		statsPanel.add(heaterButton, gbc_heaterButton);
		
		blockHeater = new JLabel("N/A");
		GridBagConstraints gbc_blockHeater = new GridBagConstraints();
		gbc_blockHeater.insets = new Insets(0, 0, 5, 5);
		gbc_blockHeater.gridx = 1;
		gbc_blockHeater.gridy = 8;
		statsPanel.add(blockHeater, gbc_blockHeater);
		
		Component horizontalStrut_2 = Box.createHorizontalStrut(20);
		GridBagConstraints gbc_horizontalStrut_2 = new GridBagConstraints();
		gbc_horizontalStrut_2.insets = new Insets(0, 0, 5, 5);
		gbc_horizontalStrut_2.gridx = 8;
		gbc_horizontalStrut_2.gridy = 1;
		trackModelPanel.add(horizontalStrut_2, gbc_horizontalStrut_2);
		
		failurePanel = new JPanel();
		failurePanel.setBorder(new TitledBorder(UIManager.getBorder("TitledBorder.border"), "Failure Modes", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		GridBagConstraints gbc_failurePanel = new GridBagConstraints();
		gbc_failurePanel.fill = GridBagConstraints.BOTH;
		gbc_failurePanel.insets = new Insets(0, 0, 5, 5);
		gbc_failurePanel.gridx = 0;
		gbc_failurePanel.gridy = 2;
		trackModelPanel.add(failurePanel, gbc_failurePanel);
		GridBagLayout gbl_failurePanel = new GridBagLayout();
		gbl_failurePanel.columnWidths = new int[]{0, 0, 0};
		gbl_failurePanel.rowHeights = new int[]{0, 0, 0, 0};
		gbl_failurePanel.columnWeights = new double[]{0.0, 1.0, Double.MIN_VALUE};
		gbl_failurePanel.rowWeights = new double[]{0.0, 0.0, 0.0, Double.MIN_VALUE};
		failurePanel.setLayout(gbl_failurePanel);
		
		failureTypeLabel = new JLabel("Type");
		GridBagConstraints gbc_failureTypeLabel = new GridBagConstraints();
		gbc_failureTypeLabel.insets = new Insets(0, 0, 5, 5);
		gbc_failureTypeLabel.gridx = 0;
		gbc_failureTypeLabel.gridy = 0;
		failurePanel.add(failureTypeLabel, gbc_failureTypeLabel);
		
		failureTypeBox = new JComboBox<String>(failureString);
		GridBagConstraints gbc_failureTypeBox = new GridBagConstraints();
		gbc_failureTypeBox.fill = GridBagConstraints.HORIZONTAL;
		gbc_failureTypeBox.insets = new Insets(0, 0, 5, 0);
		gbc_failureTypeBox.gridx = 1;
		gbc_failureTypeBox.gridy = 0;
		failurePanel.add(failureTypeBox, gbc_failureTypeBox);
		
		failureBlockLabel = new JLabel("Block");
		GridBagConstraints gbc_failureBlockLabel = new GridBagConstraints();
		gbc_failureBlockLabel.insets = new Insets(0, 0, 5, 5);
		gbc_failureBlockLabel.gridx = 0;
		gbc_failureBlockLabel.gridy = 1;
		failurePanel.add(failureBlockLabel, gbc_failureBlockLabel);
		
		failureBlockBox = new JComboBox<String>();
		GridBagConstraints gbc_failureBlockBox = new GridBagConstraints();
		gbc_failureBlockBox.fill = GridBagConstraints.HORIZONTAL;
		gbc_failureBlockBox.insets = new Insets(0, 0, 5, 0);
		gbc_failureBlockBox.gridx = 1;
		gbc_failureBlockBox.gridy = 1;
		failurePanel.add(failureBlockBox, gbc_failureBlockBox);
		
		createFailureButton = new JButton("Create Failure");
		createFailureButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if(e.getSource() == createFailureButton){
					String type = (String)failureTypeBox.getSelectedItem();
					int blockNumber = Integer.parseInt((String)failureBlockBox.getSelectedItem());
					if(rdbtnRed.isSelected()){
						myTrackModelData.setFailure("Red", blockNumber, type);
						return;
					}
					myTrackModelData.setFailure("Green", blockNumber, type);
				}
			}
		});
		GridBagConstraints gbc_createFailureButton = new GridBagConstraints();
		gbc_createFailureButton.insets = new Insets(0, 0, 0, 5);
		gbc_createFailureButton.gridx = 1;
		gbc_createFailureButton.gridy = 2;
		failurePanel.add(createFailureButton, gbc_createFailureButton);
		
		createRemoveFailureButton = new JButton("Remove Failure");
		createRemoveFailureButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if(e.getSource() == createRemoveFailureButton){
					String type = (String)failureTypeBox.getSelectedItem();
					int blockNumber = Integer.parseInt((String)failureBlockBox.getSelectedItem());
					if(rdbtnRed.isSelected()){
						myTrackModelData.unsetFailure("Red", blockNumber, type);
						return;
					}
					myTrackModelData.unsetFailure("Green", blockNumber, type);
				}
			}
		});
		GridBagConstraints gbc_createRemoveFailureButton = new GridBagConstraints();
		gbc_createRemoveFailureButton.insets = new Insets(0, 0, 0, 5);
		gbc_createRemoveFailureButton.gridx = 1;
		gbc_createRemoveFailureButton.gridy = 3;
		failurePanel.add(createRemoveFailureButton, gbc_createRemoveFailureButton);
		
		statusPanel = new JPanel();
		statusPanel.setBorder(new TitledBorder(UIManager.getBorder("TitledBorder.border"), "Current Block Status", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		GridBagConstraints gbc_statusPanel = new GridBagConstraints();
		gbc_statusPanel.gridwidth = 8;
		gbc_statusPanel.fill = GridBagConstraints.BOTH;
		gbc_statusPanel.insets = new Insets(0, 0, 5, 5);
		gbc_statusPanel.gridx = 0;
		gbc_statusPanel.gridy = 3;
		trackModelPanel.add(statusPanel, gbc_statusPanel);
		statusPanel.setLayout(new GridLayout(0, 1, 0, 0));
		
		lblAllClear = new JLabel("All Clear");
		lblAllClear.setHorizontalAlignment(SwingConstants.CENTER);
		statusPanel.add(lblAllClear);
		
		Component horizontalStrut_1 = Box.createHorizontalStrut(20);
		GridBagConstraints gbc_horizontalStrut_1 = new GridBagConstraints();
		gbc_horizontalStrut_1.insets = new Insets(0, 0, 5, 5);
		gbc_horizontalStrut_1.gridx = 2;
		gbc_horizontalStrut_1.gridy = 4;
		trackModelPanel.add(horizontalStrut_1, gbc_horizontalStrut_1);
		
		logPanel = new JPanel();
		GridBagConstraints gbc_logPanel = new GridBagConstraints();
		gbc_logPanel.fill = GridBagConstraints.BOTH;
		gbc_logPanel.gridwidth = 8;
		gbc_logPanel.insets = new Insets(0, 0, 0, 5);
		gbc_logPanel.gridx = 0;
		gbc_logPanel.gridy = 5;
		trackModelPanel.add(logPanel, gbc_logPanel);
		logPanel.setLayout(new BoxLayout(logPanel, BoxLayout.X_AXIS));
		
		lblTrackLog = new JLabel("Track Log:  ");
		logPanel.add(lblTrackLog);
		
		lblWaiting = new JLabel("Waiting");
		logPanel.add(lblWaiting);
	}
	
	public void initTrack(){
		JFileChooser fc = new JFileChooser();
		fc.setFileSelectionMode(JFileChooser.FILES_AND_DIRECTORIES);
		fc.setMultiSelectionEnabled(true);
		fc.setCurrentDirectory(new File(TRACK_LAYOUT_DIRECTORY));
		int retval = fc.showOpenDialog(new JPanel());

		if (retval == JFileChooser.APPROVE_OPTION){
			File[] track = fc.getSelectedFiles();
			for(int i = 0; i < track.length; ++i){
				myTrackModelData.loadTrack(track[i]);
			}
		}
	}
	
	public JPanel getTrackModelPanel(){
		return trackModelPanel;
	}
}
