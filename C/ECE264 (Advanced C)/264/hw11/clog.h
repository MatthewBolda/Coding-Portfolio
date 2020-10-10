#include <stdbool.h>
#ifndef __CLOG_H__
#define __CLOG_H__


#define ANSI_RED 	  "\x1b[31m"	
#define ANSI_GREEN 	  "\x1b[32m"
#define ANSI_YELLOW   "\x1b[33m"
#define ANSI_BLUE     "\x1b[34m"
#define ANSI_MAGENTA  "\x1b[35m"
#define ANSI_CYAN     "\x1b[36m"
#define ANSI_RESET    "\x1b[0m"

#	ifdef DEBUG 	//to enable all required macros
#		define log_msg(msg) fprintf(stderr, "%s\n", (msg))

# 		define log_int(n) fprintf(stderr, "%s == %d\n", (#n), (n))

#		define log_str(s) fprintf(stderr, "s == %s\n", (s))

#		define log_char(ch) fprintf(stderr, "ch == '%c'\n", (ch))

#		define log_addr(addr) fprintf(stderr, "addr == %p\n", addr)

#		define log_invalid(...) fprintf(stderr,"Usage: %s <\"red\"|\"green\"|\"yellow\"|\"blue\"|\"magenta\"|\"cyan\"> STRING\n", __VA_ARGS__)

# 		define log_red(...) do{						\
			fprintf(stderr, ANSI_RED);				\
			fprintf(stderr, "%s\n", __VA_ARGS__);	\
			fprintf(stderr, ANSI_RESET);			\
			}while(false)
# 		define log_green(...) do{					\
			fprintf(stderr, ANSI_GREEN);			\
			fprintf(stderr, "%s\n", __VA_ARGS__);	\
			fprintf(stderr, ANSI_RESET);			\
			}while(false)
# 		define log_yellow(...) do{					\
			fprintf(stderr, ANSI_YELLOW);			\
			fprintf(stderr, "%s\n", __VA_ARGS__);	\
			fprintf(stderr, ANSI_RESET);			\
			}while(false)
# 		define log_blue(...) do{					\
			fprintf(stderr, ANSI_BLUE);				\
			fprintf(stderr, "%s\n", __VA_ARGS__);	\
			fprintf(stderr, ANSI_RESET);			\
			}while(false)
# 		define log_magenta(...) do{					\
			fprintf(stderr, ANSI_MAGENTA);			\
			fprintf(stderr, "%s\n", __VA_ARGS__);	\
			fprintf(stderr, ANSI_RESET);			\
			}while(false)
# 		define log_cyan(...) do{					\
			fprintf(stderr, ANSI_CYAN);				\
			fprintf(stderr, "%s\n", __VA_ARGS__);	\
			fprintf(stderr, ANSI_RESET);			\
			}while(false)

#	else 			//to disable all above required macros
#		define log_msg(msg)

#		define log_int(n)

#		define log_str(s)

#		define log_char(ch)

#		define log_addr(addr)

#		define log_invalid(...)
#		define log_red(...)
#		define log_green(...)
#		define log_yellow(...)
#		define log_blue(...)
#		define log_magenta(...)
#		define log_cyan(...)


#	endif

#endif
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
