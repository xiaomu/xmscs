#ifndef _XM_CMN_UTIL_H_
#define _XM_CMN_UTIL_H_

char *xm_char_add(char *str, char *str2);
char *xm_vsprintf_ex(int len, char *fmt, ... );
int debug_printf(char *format, ...);
int help(char *menu[]);

#endif
