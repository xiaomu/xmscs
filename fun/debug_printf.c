// #define <stdio.h>// #define <stdarg.h>int debug_printf(char *format, ...){	#ifdef DEBUG		va_list ap;		#define FORMAT_LEN 1024		char buf[FORMAT_LEN] = {'\0'};		va_start(ap, format);		vsprintf(buf, format, ap);		va_end(ap);		printf("%s", buf);	#endif	return 0;}