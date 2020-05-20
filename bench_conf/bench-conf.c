#include "data.h"
#include "bench.h"
#include <getopt.h>


static struct option options[] = {
	{"data",    required_argument,  0,  'd' },
	{"timer",   optional_argument,  0,  't' },
	{"cpu",     optional_argument,  0,  'c' },
	{0,         0,                  0,  0 }
};

benchConfigT* benchParseArgs(int argc, char* argv[]) 
{
	benchConfigT* config = calloc(1, sizeof(benchConfigT));
	int option;
	int index = 0;

	while(1)
	{	
		option = getopt_long(argc, argv, "vp:m:", options, &index);

		/*  Detect the en of the option */
		if (option == -1) break;

		switch(option) {
			case 'd' :
				config->numberData = atoi(optarg);
				break;
				
			case 't' :
				config->timer = 1;
				break;

			case 'c' :
				config->cpu = 1;
				break;
			
			default :
				break;
		}
	}

	return config;
}