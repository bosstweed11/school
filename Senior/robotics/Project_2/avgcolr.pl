 #!/usr/bin/perl
 
 
 use strict;
 use warnings;
 
    
print "--Beginning assembler...\n\n";

findAverageColor();



# 	assemble
#		This is the main function in the program that takes a text file 
#		of instructions and converts that to hex for a mips processor
#
#
sub findAverageColor
{
	
	if ( @ARGV != 3)
	{
		print "USAGE: assembler.pl <input file>  xMax yMax\n";
		
	}
	else
	{
	
		my $file = $ARGV[0];
		my $xMax = $ARGV[1];
		my $yMax = $ARGV[2];
		
		my $avgR = 0;
		my $avgG = 0;
		my $avgB = 0;
		
		my @asFileRows = ();
		open( FILE, "<", $file) or die("Can't open file");
		
		@asFileRows = <FILE>;
		
		close ( FILE );
		
		
		
		for ( my $ii = 0; $ii < $xMax; $ii++ )
		{
			my @asPixel = split(",  ", $asFileRows[$ii] );
			
			for ( my $jj = 0; $jj < $yMax; $jj++ )
			{
				#print " $asPixel[$jj]\n";
				my @colorField = split(" ", $asPixel[$jj]);
				
				#print "Field 0: $colorField[0]\nField 1: $colorField[1]\nField 2: $colorField[2]\n" ;
				my $lastColor = substr $colorField[2], 0 ,2;
				$avgB += hex( $colorField[0]);
				$avgG += hex( $colorField[1]);
				$avgR += hex( $lastColor );
				
				#print "Blue: $avgB \nGreen: $avgG \nRed: $avgR \n";

			}
		}
		
		my $divisor = $xMax * $yMax;
		
		$avgB = sprintf("%x", int($avgB / $divisor) );
		$avgG = sprintf("%x", int($avgG / $divisor) );
		$avgR = sprintf("%x", int($avgR / $divisor) );
		
		print "Blue: $avgB \nGreen: $avgG \nRed: $avgR \n";
		
		#my $hexB = sprintf( "%x", $avgB);
		#print "\n$hexB\n";
		#sprintf("Blue: %x \nGreen: %x \nRed: %x \n", $avgB, $avgG, $avgR );
		
		#sprintf("%x", 255);
		#print "hello";
		
	}
}

    