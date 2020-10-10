#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "clog.h"

#	define mu_start() 	int __mu_line_number = 0;			\
						bool __mu_check_fail = true;		\

#	define mu_check(condition) do{				\
			if((condition)) {					\
				__mu_line_number = 0;			\
			}									\
			else {								\
				__mu_line_number = __LINE__;	\
				__mu_check_fail  = false;		\
			}									\
		}while(false)							\
			

#	define mu_run(function) do{															\
			if(!(function()) ) {														\
				__mu_log_green("Test passed: function");					\
			}																			\
			else {																		\
				__mu_log_red("Test failed: function at line ");		\
			}																			\
		}while(false)																	\

#	define mu_end() do{					\
		if(!(__mu_check_fail)){			\
			return __mu_line_number;	\
		}								\
		else {							\
			return 0;					\
		}								\
	}while(false)						\

#	ifdef DEBUG 	//removing macros when debug is flagged

#		define __mu_log_invalid(...)
#		define __mu_log_red(...)
#		define __mu_log_green(...)
#		define __mu_log_yellow(...)
#		define __mu_log_blue(...)
#		define __mu_log_magenta(...)
#		define __mu_log_cyan(...)

#	else 			//to activate macros so echo_color and mu_run still print without debug flag

#		define __mu_log_invalid(...) fprintf(stderr,"Usage: %s <\"red\"|\"green\"|\"yellow\"|\"blue\"|\"magenta\"|\"cyan\"> STRING\n", __VA_ARGS__)

# 		define __mu_log_red(...) do{				\
			fprintf(stderr, ANSI_RED);				\
			fprintf(stderr, "%s\n", __VA_ARGS__);	\
			fprintf(stderr, ANSI_RESET);			\
			}while(false)
# 		define __mu_log_green(...) do{				\
			fprintf(stderr, "%s\n", __VA_ARGS__);	\
			}while(false)
# 		define __mu_log_yellow(...) do{				\
			fprintf(stderr, ANSI_YELLOW);			\
			fprintf(stderr, "%s\n", __VA_ARGS__);	\
			fprintf(stderr, ANSI_RESET);			\
			}while(false)
# 		define __mu_log_blue(...) do{				\
			fprintf(stderr, ANSI_BLUE);				\
			fprintf(stderr, "%s\n", __VA_ARGS__);	\
			fprintf(stderr, ANSI_RESET);			\
			}while(false)
# 		define __mu_log_magenta(...) do{			\
			fprintf(stderr, ANSI_MAGENTA);			\
			fprintf(stderr, "%s\n", __VA_ARGS__);	\
			fprintf(stderr, ANSI_RESET);			\
			}while(false)
# 		define __mu_log_cyan(...) do{				\
			fprintf(stderr, ANSI_CYAN);				\
			fprintf(stderr, "%s\n", __VA_ARGS__);	\
			fprintf(stderr, ANSI_RESET);			\
			}while(false)

#	endif
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
