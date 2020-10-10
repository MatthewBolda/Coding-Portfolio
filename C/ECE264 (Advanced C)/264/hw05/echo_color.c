#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clog.h"
#include "miniunit.h"

int main(int argc, char* argv[]) {
	//the command line should be ./echo_color "color" "string" and only that method should work
	if( argc == 3) {
		char* color = argv[1];
		if(!(strcmp(color, "red"))){
			log_red(argv[2]);
			__mu_log_red(argv[2]);
		}
		if(!(strcmp(color, "green"))){
			log_green(argv[2]);
			__mu_log_green(argv[2]);
		}	
		if(!(strcmp(color, "yellow"))){
			log_yellow(argv[2]);
			__mu_log_yellow(argv[2]);
		}
		if(!(strcmp(color, "blue"))){
			log_blue(argv[2]);
			__mu_log_blue(argv[2]);
		}
		if(!(strcmp(color, "magenta"))){
			log_magenta(argv[2]);
			__mu_log_magenta(argv[2]);
		}
		if(!(strcmp(color, "cyan"))){
			log_cyan(argv[2]);
			__mu_log_cyan(argv[2]);
		}
		
	}
	else {
		log_invalid(argv[0]);
		__mu_log_invalid(argv[0]);
		//fprintf(stderr,"Usage: %%s <\"red\"|\"green\"|\"yellow\"|\"blue\"|\"magenta\"|\"cyan\"> STRING");
	}

	return EXIT_SUCCESS;

}

/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
