 #!/usr/bin/perl
 
 
 use strict;
 use warnings;
 
 
 
 	# Hash where the opcode is a RegImm
 
 	my %hOpcodes = (
        "ADD" => "100000",
        "ADDU" => "100001",
        "ADDI" => "001000",
        "ADDIU" => "001001",
        "SUB" => "100010",
        "SUBU" => "100011",
        "AND" => "100100",
        "ANDI" => "001100",
        "OR" => "100101",
        "ORI" => "001101",
        "XOR" => "100110",
        "XORI" => "001110",
        "NOR" => "100111",
        "SLL" => "000000",
        "SRL" => "000010",
        "SRA" => "000011",
        "SLLV" => "000100",
        "SRLV" => "000110",
        "SRAV" => "000111",
        "SLT" => "101010",
        "STLU" => "101011",
        "SLTI" => "001010",
        "SLTIU" => "001011",
        "LB" => "100000",
        "LBU" => "100100",
        "LH" => "100001",
        "LHU" => "100101",
        "LW" => "100011",
        "LUI" => "001111",
        "SB" => "101000",
        "SH" => "101001",
        "SW" => "101011",
        "BEQ" => "000100",
        "BNE" => "000101",
        "BLTZ" => "00000",
        "BGEZ" => "00001",
        "BLTZAL" => "10000",
        "BGEZAL" => "10001",
        "BLEZ" => "000110",
        "BGTZ" => "000111",
        "J" => "000010",
        "JAL" => "000011",
        "JR" => "001000",
        "JALR" => "001001"
    );
    
    # Hash where the opcode is a RegImm
    
    my %hRegImm = (
        "BLTZ" => "000001",
        "BGEZ" => "000001",
        "BLTZAL" => "000001",
        "BGEZAL" => "000001",
    );
    
    # Hash for I-Type Instructions
    
    my %hIType = (
        "ADDI" => "001000",
        "ADDIU" => "001001",
        "ANDI" => "001100",
        "ORI" => "001101",
        "XORI" => "001110",
        "SLTI" => "001010",
        "SLTIU" => "001011",
        "LB" => "100000",
        "LBU" => "100100",
        "LH" => "100001",
        "LHU" => "100101",
        "LW" => "100011",
        "LUI" => "001111",
        "SB" => "101000",
        "SH" => "101001",
        "SW" => "101011",
        "BEQ" => "000100",
        "BNE" => "000101",
        "BLTZ" => "000001",
        "BGEZ" => "000001",
        "BLTZAL" => "000001",
        "BGEZAL" => "000001",
        "BLEZ" => "000110",
        "BGTZ" => "000111",
    );
    
    # Hash for R-Type Instructions
    my %hRType = (
       	"ADD" => "100000",
       	"ADDU" => "100001",
       	"SUB" => "100010",
       	"SUBU" => "100011",
       	"AND" => "100100",
       	"OR" => "100101",
       	"XOR" => "100110",
       	"NOR" => "100111",
       	"SLL" => "000000",
       	"SRL" => "000010",
       	"SRA" => "000011",
       	"SLLV" => "000100",
       	"SRLV" => "000110",
       	"SRAV" => "000111",
       	"SLT" => "101010",
    	"STLU" => "101011",       	
    	"JR" => "001000",
       	"JALR" => "001001"
    );
    
    # Hash for J-Type Instructions
    my %hJType = (
      	"J" => "000010",
       	"JAL" => "000011",

    );
    
  
    
    
    print "--Beginning assembler...\n\n";
    
    assemble();
    
    
    
    # 	assemble
    #		This is the main function in the program that takes a text file 
    #		of instructions and converts that to hex for a mips processor
    #
    #
    sub assemble 
    {
    	
    	if ( @ARGV != 1 )
    	{
    		print "USAGE: assembler.pl <input DIRECTORY> \n";
    		
    	}
    	else
    	{
    	
    		my $dir = "./$ARGV[0]";
    		#print "$dir\n";
    		#getc();
			foreach my $fTestFile ( glob("$dir/*.txt") ) 
			{
    		
    			print "Filename1: $fTestFile\n";
    			my @asInstructionFile = ();
    			open(FILE, "<", $fTestFile) or die("Can't open file");
    		
				@asInstructionFile = <FILE>;
				my $sBinaryInstruction;
				my @asInitialFileName = split("/", $fTestFile);
				my $tempFilename = substr( $asInitialFileName[2], 0 , -4);
				print "Filename3: $tempFilename\n";
				my $sOutputFileName = $dir . "/" . $tempFilename . ".do";
				close(FILE);
				
				
				print "Filename: $sOutputFileName\n";
				#getc();
				open(OUTPUT, ">", $sOutputFileName) or die("Can't open file");
				
				print OUTPUT "restart -f\n";
				#print "restart -f\n";
				
				#getc();
				print OUTPUT "force -freeze /cpu_test/reset 0 0\n";
				print OUTPUT "force -freeze clock 1 0, 0 {50 ns} -r 100\n";
			
				for ( my $ii = 0; $ii < @asInstructionFile; $ii++ )
				{
			
					
					my @asCurrInstruction = split(" ", $asInstructionFile[$ii]);
				
					if ( $asCurrInstruction[0] ne "--" )
					{
						if ( exists $hIType{$asCurrInstruction[0]} )
						{
					
							# Distinguish between adds/ branches/ shifts...
							my $sFirstSix = $hOpcodes{$asCurrInstruction[0]};
							my $sSecondFive;
							my $sThirdFive;
							my $sLastSixteen;
						
							if ( $asCurrInstruction[0] eq "LUI" )
							{
								$sSecondFive = "00000";
								$sThirdFive = sprintf ("%b", $asCurrInstruction[1]);
								$sLastSixteen = sprintf ("%b", $asCurrInstruction[2]);
						
							}
							elsif ( $asCurrInstruction[0] eq "BLEZ" || 
									$asCurrInstruction[0] eq "BGTZ" ||
									$asCurrInstruction[0] eq "BGEZAL" ||
									$asCurrInstruction[0] eq "BLTZAL" ||
									$asCurrInstruction[0] eq "BGEZ" ||
									$asCurrInstruction[0] eq "BLTZ" )
							{
								$sFirstSix = "000001";
								$sSecondFive = sprintf ("%b", $asCurrInstruction[1]);
								$sThirdFive = $hOpcodes{$asCurrInstruction[0]};
								$sLastSixteen = sprintf ("%b", $asCurrInstruction[2]);
							}
							else
							{
								$sSecondFive = sprintf ("%b", $asCurrInstruction[2]);
								$sThirdFive = sprintf ("%b", $asCurrInstruction[1]);
								$sLastSixteen = sprintf ("%b", $asCurrInstruction[3]);
							}
						
		
							# Just incase we didnt use up all the bits in any of the other fields
							# we will append 0's at the beginning
							while ( length( $sSecondFive) < 5 )
							{
								$sSecondFive = "0". $sSecondFive;
							}
						
							while ( length( $sThirdFive) < 5 )
							{
								$sThirdFive = "0". $sThirdFive;
							}
						
							while ( length( $sLastSixteen) < 16 )
							{
								$sLastSixteen = "0". $sLastSixteen;
							}
						
							$sBinaryInstruction = $sFirstSix . $sSecondFive . $sThirdFive . $sLastSixteen;
							#print "Instruction: $asInstructionFile[$ii] \n";
							#print "Binary: $sBinaryInstruction \n";
							#print "Opcode: $sFirstSix \n";
							#print "Rs: $sSecondFive \n";
							#print "Rd: $sThirdFive \n";
							#print "Imm: $sLastSixteen \n";
						
						
					
						}
						elsif ( exists $hRType{$asCurrInstruction[0]} ) 
						{
							my $sFirstSix = "000000";
							my $sSecondFive;
							my $sThirdFive;
							my $sFourthFive;
							my $sFifthFive;
							my $sLastSix = $hOpcodes{$asCurrInstruction[0]};
						
							if ( $asCurrInstruction[0] eq "SLL" || $asCurrInstruction[0] eq "SRL" || $asCurrInstruction[0] eq "SRA" )
							{
								$sSecondFive = "00000";
								$sThirdFive = sprintf ("%b", $asCurrInstruction[2]);
								$sFourthFive = sprintf ("%b", $asCurrInstruction[1]);
								$sFifthFive = sprintf ("%b", $asCurrInstruction[3]); 
						
							}
							elsif( $asCurrInstruction[0] eq "JR" )
							{
								$sSecondFive = sprintf ("%b", $asCurrInstruction[1]);
								$sThirdFive = "00000";
								$sFourthFive = "00000";
								$sFifthFive = "00000";
							}
							elsif( $asCurrInstruction[0] eq "JALR" )
							{
								$sSecondFive = sprintf ("%b", $asCurrInstruction[1]);
								$sThirdFive = "00000";
								$sFourthFive = "11111";
								$sFifthFive = "00000";
							}
							else
							{
								$sSecondFive = sprintf ("%b", $asCurrInstruction[2]);
								$sThirdFive = sprintf ("%b", $asCurrInstruction[3]);
								$sFourthFive = sprintf ("%b", $asCurrInstruction[1]); 
								$sFifthFive = "00000";
							}
						
							while ( length( $sSecondFive) < 5 )
							{
								$sSecondFive = "0". $sSecondFive;
							}
						
							while ( length( $sThirdFive) < 5 )
							{
								$sThirdFive = "0". $sThirdFive;
							}
						
							while ( length( $sFourthFive) < 5 )
							{
								$sFourthFive = "0". $sFourthFive;
							}
						
							while ( length( $sFifthFive) < 5 )
							{
								$sFifthFive = "0". $sFifthFive;
							}
						
						
							$sBinaryInstruction = $sFirstSix . $sSecondFive . $sThirdFive . $sFourthFive . $sFifthFive . $sLastSix;
							my $iSize = length($sFirstSix) + length($sSecondFive) + length($sThirdFive) + length($sFourthFive) + length($sFifthFive) + length($sLastSix);
							#print "Instruction: $asInstructionFile[$ii] \n";
							#print "Binary: $sBinaryInstruction \n";
							#print "Special: $sFirstSix \n";
							#print "Rs: $sSecondFive \n";
							#print "Rd: $sThirdFive \n";
							#print "Rt: $sFourthFive \n";
							#print "dead/shiftamt: $sFifthFive \n";
							#print "OpCode: $sLastSix \n";
							#print "Size: $iSize\n";
						
						
						
						}
						elsif ( exists $hJType{$asCurrInstruction[0]} )
						{
							my $sFirstSix = $hOpcodes{$asCurrInstruction[0]};
							my $sLastTwentySix = sprintf ("%b", $asCurrInstruction[1]);
						
							while ( length( $sLastTwentySix) < 26 )
							{
								$sLastTwentySix = "0". $sLastTwentySix;
							}
						
							my $sBinaryInstruction = $sFirstSix . $sLastTwentySix;
							#print "Instruction: $asInstructionFile[$ii] \n";
							#print "Binary: $sBinaryInstruction \n";
							#print "Opcode: $sFirstSix \n";
							#print "Imm: $sLastTwentySix \n";
						
					
						}
					
						my $hex = sprintf('%X', oct("0b$sBinaryInstruction"));
						
						while ( length( $hex) < 8 )
						{
							$hex = "0". $hex;
						}
						my $actualIndex = $ii - 1;
						print OUTPUT "force /cpu_test/instruction_memory/instruction_array($actualIndex) X\"$hex\"\n";
					}
					else
					{
						print "$asInstructionFile[$ii]\n";
				
					}
			
				}
			
			
				for ( my $jj = 0; $jj < 4; $jj++ )
				{
					my $kk = 4 + $jj;
					print OUTPUT "force /cpu_test/instruction_memory/instruction_array($kk) X\"00000000\"\n";
				}
				
				print OUTPUT "run 200ns\n";
				print OUTPUT "force -freeze /cpu_test/reset 1 0\n";
				print OUTPUT "run 200ns\n";
				print OUTPUT "force -freeze /cpu_test/reset 0 0\n";
				print OUTPUT "run 1300ns\n";
		
				close(OUTPUT);
			}
		}
    }
    
    