Andrew Beers acb75

Known Issues:
	Opt runs very slowly, my algorithm is to find the next references of all objects and evict the max
	I could improve the speed by placing all my page numbers in an array instead of splitting and getting the page
	number every time I access it, but I feel like my time is better spent finishing my write up. Also opt takes roughly
	2 - 3 hours to run now and im not sure that doing that would improve my performance to <5 minutes anyway
	
	
I wrote the program in perl, how to run:

perl vmsim.pl -n <numframes> -a <algo> [ accepts rand, opt, aging, nru ] {-r <refresh rate> [only if nru /aging]} <trace file>
