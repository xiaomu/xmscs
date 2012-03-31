#include <stdio.h>
#include <stdlib.h>
#include "getopt.h"


#define FLAG_INTERACT   0x0001        
#define FLAG_FORCE      0x0002        
#define FLAG_RECURSIVE  0x0004        

int flags = 0;                        
int verbose = 5;                      
const char* in_fname = NULL;          
const char* out_fname = NULL;         

int arg_to_int(const char* arg, int min, int max, int defalt, const char* opt)
{
	int i = defalt;
	int rv;

	if(!arg) goto done;

	rv = sscanf(arg, "%d", &i);
	if(rv != 1) {
		fprintf(stderr, "%s: integer argument required.\n", opt);
		i = defalt;
		goto done;
	}

	if(i < min || max < i) {
		fprintf(stderr, "%s: argument out of integer range.\n", opt);
		i = defalt;
		goto done;
	}
done:
	return i;
}

void help()
{
	printf(
	"getopt test program\n"
	"Usage: test [OPTION] [INPUT]\n"
	"   INPUT           set input filename (doesn't do anything)\n"
	"   -h              help menu (this screen)\n"
	"   -i              interactive mode (doesn't do anything)\n"
	"   -f              force mode (doesn't do anything)\n"
	"   -r              recursive mode (doesn't do anything)\n"
	"   -v[level]       set verbosity level (5 is default; doesn't do anything)\n"
	"   -o filename     set output filename (doesn't do anything)\n"
	);
}

int main(int argc, char* argv[])
{

	while(1) {
		int c = getopt(argc, argv, "-ifrhv::o:");
		if(c == -1) break;
		switch(c) {
		case 'i': flags |= FLAG_INTERACT; break;
		case 'f': flags |= FLAG_FORCE; break;
		case 'r': flags |= FLAG_RECURSIVE; break;
		case 'h': help(); exit(0);
		case 'v': verbose = arg_to_int(optarg, 0, 10, 5, "v"); break;
		case 'o': out_fname = optarg; break;
		case 1: in_fname = optarg; break;
#ifdef DEBUG
		default:
			printf("Option '%c' (%d) with '%s'\n", c, c, optarg);
#endif
		}
	}
#ifdef DEBUG
	printf("optind at %d; argv[optind] = '%s'\n", optind, argv[optind]);
#endif

	if(flags & FLAG_INTERACT) printf("in interactive mode\n");
	else printf("not in interactive mode\n");
	if(flags & FLAG_FORCE) printf("in force mode\n");
	else printf("not in force mode\n");
	if(flags & FLAG_RECURSIVE) printf("in recursive mode\n");
	else printf("not in recursive mode\n");
	printf("verbosity level: %d\n", verbose);
	if(in_fname) printf("input filename: %s\n", in_fname);
	else printf("no input filename\n");
	if(out_fname) printf("output filename: %s\n", out_fname);
	else printf("no output filename\n");
	return 0;
}