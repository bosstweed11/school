 #!/usr/bin/perl
 
 
 #Program written by Andrew Beers for CS 1550 Project 3 Summer 2014
 
use strict;
use warnings;



# Page size is 4KB
# 32 bit address space

my @frames;
my @dirty;
my @referenced;
my @referencedValues;
my @valid;
my @optArray;

my @pageNumbers;
my @modes;

my $numPageFaults = 0;
my $memAccesses = 0;
my $writesToDisk = 0;

my $refreshRate;

my @asFileRows = ();

my $print = 0;

main();

# 	main
#		This is the main function in the program that reads the command line args
#		and gets the program ready to start the virtual memory simulation
#
#
sub main
{
	
	if ( @ARGV != 5 && @ARGV != 7)
	{
		print "USAGE: perl vmsim.pl –n <numframes> -a <opt|clock|nru|rand> [-r <NRUrefresh>] <tracefile>\n";
	}
	else
	{
		if ( $ARGV[0] ne "-n" )
		{
			print "USAGE: perl vmsim.pl –n <numframes> -a <opt|clock|nru|rand> [-r <NRUrefresh>] <tracefile>\n";
		}
		else
		{
			my $numFrames = $ARGV[1];
			
			@frames = (0) x $numFrames;
			@dirty = (0) x $numFrames;
			@valid = (0) x $numFrames;
			@optArray = (-1) x $numFrames;
			
			if ( $ARGV[2] ne "-a" )
			{
				print "USAGE: perl vmsim.pl –n <numframes> -a <opt|clock|nru|rand> [-r <NRUrefresh>] <tracefile>\n";
			}
			else
			{
				my $memoryAlgo = $ARGV[3];
				my $traceFile;
				
				# If we are using NRU our command line args will be a little different
				if ( $memoryAlgo eq "nru" || $memoryAlgo eq "NRU" || $memoryAlgo eq "aging" )
				{
				
					if ( $ARGV[4] ne "-r" )
					{
						print "USAGE: perl vmsim.pl –n <numframes> -a <opt|clock|nru|rand> [-r <NRUrefresh>] <tracefile>\n";
					}
					else
					{
						$refreshRate = $ARGV[5];
					}
					
					$traceFile = $ARGV[6];
				}
				else
				{
					$traceFile = $ARGV[4];
				}
				
				# Initialize the referenced bits appropriately depending on he algorithm
				if ( $memoryAlgo eq "aging" )
				{
					@referenced = (0) x $numFrames;
					@referencedValues = (0) x $numFrames;
				}
				else
				{
					@referenced = (0) x $numFrames;
				}
				
				# At this point we've loaded all of our command line args. print them out for now
				if ( $print )
				{
					print("Number of frames:\t$numFrames\n");
					print("Algorithm being used:\t$memoryAlgo\n");
					print("File name:\t\t$traceFile\n");
				}
				
				
				
				open( FILE, "<", $traceFile) or die("Can't open file");
		
				@asFileRows = <FILE>;
		
				close ( FILE );
		
				my $ii = 0;
				
				
				# Each iteration is one instruction
				for ( $ii = 0; $ii < @asFileRows; $ii++ )
				{
					
					# Split the line in the file so we can get the address and if its a read or a write
					my @instructionFields = split( " ", $asFileRows[$ii] );
			
					my $address = $instructionFields[0];
					my $mode = $instructionFields[1];
					
					accessMemory( $address, $mode, $memoryAlgo, $numFrames );
					
					# Increase the number of memory accesses, since all instructions are memory accesses
					$memAccesses++;
					my $jj;
					for( $jj = 0; $jj < $numFrames; $jj++)
					{
						if ( $optArray[$jj] < $memAccesses && $optArray[$jj] > 0 )
						{	
							#print "Updating!\n";
							#getc();
							updateOptArray( $frames[$jj], $jj);
						}
					}
				}
				
				print "Number of Frames:\t$numFrames\n";
				print "Total memory accesses:\t$memAccesses\n";
				print "Total page faults:\t$numPageFaults\n";
				print "Total writes to disk:\t$writesToDisk\n";
			}
		}
	}
}

# 	accessMemory
#		Inputs: address, mode
#		Description: attempt to read this address from the Memory, if the address exists
#		this is a hit, if there is a miss, load the address into the memory in it's spot in the memory
#		if the memory is full, handle that with the appropriate page replacement algorithm
#
#
sub accessMemory($$$$)
{

	my ($address, $mode, $algo, $numFrames) = @_;
	
	# Counter variable to iterate over our frames
	my $jj; 

	# Variable to break out of our loop sooner if we finish before the last frame
	my $done;
	
	# We want to store the page number so we shift out the offset with 4KB pages is 12
	my $pageNumber = hex($address) >> 12;
	
	# Recognize if we had a hit or not
	my $hit = 0;
	
	# If we are going to evict something
	my $evict = 1;
	
	
	
	# If we are on NRU, refresh if its time
	if ( ( $algo eq "nru" || $algo eq "NRU" || $algo eq "aging" ) && ( $memAccesses % $refreshRate == 0 ) )
	{
		# If we are resetting aging we must shift all the elements over 1 and increase the MSB if it 
		# was referenced
		if ( $algo eq "aging" )
		{
			for ( $jj = 0; $jj < $numFrames; $jj++)
			{
				$referencedValues[$jj] = $referencedValues[$jj] >> 1;
				
				if ($referenced[$jj] == 1 )
				{
					$referencedValues[$jj] += 256;
				}
			}
		}
		
		for( $jj = 0; $jj < $numFrames; $jj++ )
		{
			$referenced[$jj] = 0;
		}
	}
	
	# Check if the page is in one of our frames
	for ( $jj = 0; $jj < $numFrames; $jj++ )
	{
		#print "$pageNumber :: $frames[$jj]\n";
		
		if ( $pageNumber == $frames[$jj] )
		{	
			$hit = 1;
			
			if ( $algo eq "nru" || $algo eq "NRU" || $algo eq "aging" )
			{
				$referenced[$jj] = 1;
			}
			$evict = 0;
			print "hit\n";
			last;
		}
		
	}
	
	# If it is not in one of our frames check if we can put it in one
	if ( $hit == 0 )
	{
		$numPageFaults++;
		
		for ( $jj = 0; $jj < $numFrames; $jj++ )
		{
			# Compulsory miss, we havent seen this before
			if ( $valid[$jj] == 0 )
			{
				$valid[$jj] = 1;
				if ( $algo eq "aging" )
				{
					$referenced[$jj] = 1;
				}
				else
				{
					$referenced[$jj] = 1;
				}
				$frames[$jj] = $pageNumber;
				$evict = 0;
				
				if ( $mode eq "W" )
				{
					$dirty[$jj] = 1;
				}
				else
				{
					$dirty[$jj] = 0;
				}
				print "Page fault - no eviction\n";
				last;
				
			}
		}
	}
	
	# If we we didn't find our page in the table, AND all the pages are valid, we must evict
	if ( $evict == 1 )
	{
		# Maybe this is a switch case.... decide how to evict based on algo
		if ( $algo eq "opt" )
		{
			my $ii;
			my $max = -1;
			my $maxIndex = -1;
			
			# If something is never referenced again, evict it
			for ( $ii = 0; $ii < $numFrames; $ii++ )
			{
				if ( $optArray[$ii] == -1 )
				{
					$maxIndex = $ii;
					last;
				}
			}
			
			# else we have to find the max in our opt array
			if ( $maxIndex == -1 )
			{
				for ( $ii = 0; $ii < $numFrames; $ii++) 
				{
					
					if ( $max == -1 || $optArray[$ii] > $max )
					{
						$max = $optArray[$ii];
						$maxIndex = $ii;
					}
				}
			}
			
			evictLocation( $maxIndex, $pageNumber, $mode, $algo );
	
		}
		elsif ( $algo eq "aging" )
		{
			
			my $minIndex = 0;
			my $min = -1;
			my $same = 1;
			
			# find our item referenced furthest in the past 
			for ( $jj = 0; $jj < @referencedValues; $jj++) 
			{
				my $curVal = 512 * $referenced[$jj] + $referencedValues[$jj];
				
				if ( $min == -1 || $curVal < $min )
				{
					$min = $curVal;
					$minIndex = $jj;
				}
				elsif ( $curVal == $min )
				{
					$same++;
				}
			}
			
			if ( $same == $numFrames )
			{
				NRUalgo($pageNumber, $mode, $algo, $numFrames);
			}
			else
			{
				evictLocation( $minIndex, $pageNumber, $mode, $algo );
			}
		}
		elsif ( $algo eq "nru" || $algo eq "NRU" )
		{
			NRUalgo( $pageNumber, $mode, $algo, $numFrames );
		}
		elsif ( $algo eq "rand" )
		{
			# is this all we need to do here?
			evictLocation( int(rand($numFrames)), $pageNumber, $mode, $algo );
		}
		else
		{
			print "The algorithm is unrecognized.\nThese are the available algorithms: opt, aging, nru, rand.\n";
			exit();
		}
	}
}

# 	NRUalgo
#		Inputs: page number, mode, algo, numFrames
#		Description: evict a page based on NRU
#
#
sub NRUalgo($$$$)
{

	my ( $pageNumber, $mode, $algo, $numFrames ) = @_;
	# Variables for NRU
	my $foundIdeal = -1;
	my $firstAlt = -1;
	my $secondAlt = -1;
	my $jj;
	for ( $jj = 0; $jj < $numFrames; $jj++ )
	{
		if ( $referenced[$jj] == 0 && $dirty[$jj] == 0 )
		{
			evictLocation( $jj, $pageNumber, $mode, $algo );
			$foundIdeal = 1;
			last;
		}
		elsif( $referenced[$jj] == 0 && $dirty[$jj] == 1 )
		{
			$firstAlt = $jj;
		
		}
		elsif( $referenced[$jj] == 1 && $dirty[$jj] == 0 )
		{
			$secondAlt = $jj;
		}
	}

	# check to see if we need to evict something sub-optimal and do it.
	if ( $firstAlt != -1 )
	{
		evictLocation( $firstAlt, $pageNumber, $mode, $algo );
	}
	elsif ( $secondAlt != -1 )
	{
		evictLocation( $secondAlt, $pageNumber, $mode, $algo );
	}
	elsif ( $foundIdeal != 1 )
	{
		evictLocation( int(rand($numFrames)), $pageNumber, $mode, $algo );
	}

}

# 	updateOptArray
#		Inputs: page number, index
#		Description: checks the future pages to see when this page is next referenced
#						updates this info in the optArray
#
#
sub updateOptArray($$)
{
	my ($pageNumber, $index) = @_;
	my $jj;
	my $orig = $optArray[$index];
	for ( $jj = $memAccesses + 1; $jj < @asFileRows; $jj++ )
	{
		my @instructionFields = split( " ", $asFileRows[$jj] );

		my $futurePage = hex($instructionFields[0]) >> 12;
		my $kk;
		
		if ( $futurePage == $pageNumber )
		{
			$optArray[$index] = $jj;
			last;
		}
	}
	
	if ( $orig == $optArray[$index] )
	{
		$optArray[$index] = -1;
	}
}

# 	evictLocation
#		Inputs: index, page number, mode
#		Description: evict a page a page
#
#
sub evictLocation($$$$)
{

	my ( $index, $pageNumber, $mode, $algo ) = @_;
	if ( $dirty[$index] == 1 )
	{
		$writesToDisk++;
		print "Page fault - evict dirty\n";
	}
	else
	{
		print "Page fault - evict clean\n";
	}

	$valid[$index] = 1;
		
	if ( $algo eq "aging" || $algo eq "NRU" || $algo eq "nru" )
	{
		$referenced[$index] = 1;
	}
	
	if ( $algo eq "aging" )
	{
		$referencedValues[$index] = 0;
	}
	
	$frames[$index] = $pageNumber;

	if ( $mode eq "W" )
	{
		$dirty[$index] = 1;
	}
	else
	{
		$dirty[$index] = 0;
	}
	
	if ( $algo eq "opt" )
	{
		#print "Initializing: $pageNumber\n";
		updateOptArray( $pageNumber, $index );
	}

	return;
}