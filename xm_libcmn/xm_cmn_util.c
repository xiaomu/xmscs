/*****************************************************************************
 *  Copyright          :  All Rights Reserved.
 *
 *  Date               :  2012-09-11 16:44:30
 *  Author/Corporation :  Dengzhaoqun
 *  Email              :  dengzhaoqun@163.com
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "xm_cmn_util.h"

/*
 * sum of two strings
 * eg: str="12", str2="234" return="246"
 * return value:
 * 	 if str or str2 is null, return null; else return sum;
 */
char *xm_char_add(char *str, char *str2)
{
	char *ptr = NULL, *result = NULL;
	int flag;
	int i, j, temp;
	int len, len2, n;
	
	len = strlen(str) ;
	len2 = strlen(str2) ;
	n = (len >= len2?len:len2);
	
	if((str == NULL) || (str2 == NULL))
	{
		return NULL;
	}
	
	ptr = malloc((n+2) * sizeof(char));
	if(ptr == NULL)
	{
		printf("malloc failed.\n");
		return NULL;
	}
	
	memset(ptr, 0, n+2);
	
	
	flag = 0;
	if(len >= len2)
	{
		for(i= len-1, j=len2-1; i>=len - len2; i--, j--)
		{
			temp = (str[i] - '0') + (str2[j] - '0') + flag;
			ptr[i+1] = temp % 10 + '0';
			flag = temp/10;
		}
		for(i; i>=0; i--)
		{
			temp = (str[i] - '0') + flag;
			ptr[i+1] = temp % 10 + '0';
			flag = temp / 10;
		}
		if(flag == 1)
		{
			ptr[0] = '1';
		}
	}
	else
	{
		for(i= len-1, j=len2-1; j>=len2 - len; i--, j--)
		{
			temp = (str[i] - '0') + (str2[j] - '0') + flag;
			ptr[j+1] = temp % 10 + '0';
			flag = temp/10;
		}
		for(j; j>=0; j--)
		{
			temp = (str2[j] - '0') + flag;
			ptr[j+1] = temp % 10 + '0';
			flag = temp / 10;
		}
		if(flag == 1)
		{
			ptr[0] = '1';
		}
	}
	
	if(ptr[0] != '\0')
	{
		return ptr;
	}
	else
	{
		result = malloc((n+2) * sizeof(char));
		if(result == NULL)
		{
			printf("malloc failed.\n");
			return NULL;
		}
		memset(result, 0, n+2);
		strcpy(result, &ptr[1]);
		free(ptr);
		return result;
	}
}

/*
 * 合成字符串 
 * len: 合成字符串的最大长度
 * fmt: 格式
 * ... : 可变参数 
 * 返回合成的字符串, 失败返回NULL
int main(int argc, char *argv[])
{
	char *ptr;
	ptr = xm_vsprintf_ex(100, "%s-%s-%s", "lias", "dzq", "lvo");
	printf("%s\n", ptr);
	free(ptr);
	
	return 0;
}
*/
char *xm_vsprintf_ex(int len, char *fmt, ... )
{
	va_list ap;
	char *ptr;
	
	ptr = (char *)malloc(len * sizeof(char));
	if(ptr == NULL)
	{
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}
	memset(ptr, 0, len);
	
	va_start(ap, fmt);
	vsprintf(ptr, fmt, ap);
	va_end(ap);
	
	ptr[len-1] = '\0';
	
	return ptr;
}

/*
 * 如果#define了DEBUG, 则printf相关内容
 * format 格式
 * ... : 可变参数
*/
int debug_printf(char *format, ...)
{
	#ifdef DEBUG
		va_list ap;
		#define FORMAT_LEN 1024
		char buf[FORMAT_LEN] = {'\0'};

		va_start(ap, format);
		vsprintf(buf, format, ap);
		va_end(ap);

		printf("%s", buf);
	#endif

	return 0;
}

/*
 * 显示字符串数组中的帮助信息
 * menu: 字符串数组指针,指向的字符串以NULL结束;
 * eg:
 int main()
{
	char *menu[] = {
		"dnppi filename\n",
		NULL
	};
	
	help(menu);
	return 0;
}
 */
int help(char *menu[])
{
	char **ptr;

	ptr = menu;
	while(*ptr != NULL)
	{
		printf("%s", *ptr);
		*ptr ++;
	}

	return 0;
}

/*
 * 如果str,str2指向的字符串内容相同或者都为NULL, 返回1;  否则返回0
 */
int equal_or_bnull(char *str, char *str2)
{
	if(((str == NULL) && (str2 == NULL)) 
		|| ((str != NULL) && (str2 != NULL) && (!(strcmp(str, str2)))))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
 * 将时间格式化为 %Y-%m-%d %H:%M:%S 格式, dt存放格式化后的字符串 
 * myt: 时间值, 如果为NULL, 则返回当前时间的格式化值;
 * dt: 存放格式化字符串的空间
 */
void date_time(time_t myt, DATE_TIME_T *dt)
{
	struct tm *mytm;
	if(myt == 0)
	{
		time(&myt);
	}
	mytm = localtime(&myt);
	
	memset(dt, 0, DATE_TIME_LEN);
	strftime((char *)dt, DATE_TIME_LEN, "%Y-%m-%d %H:%M:%S", mytm);
}

/*
 * 将32位的整形值转换为字符串
 * val: 整数值 
 * base: 进制数
 * 使用后需要free
 */
char* itoa(int val, int base)
{
	char *buf;
	int i = 30;
	
	buf = (char *)malloc(32 * sizeof(char));
	if(buf == NULL)
	{
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}
	memset(buf, 0, 32);
	
	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];
	return &buf[i+1];
}

/*
 *合并两个字符串,返回新的字符串 
 * 使用后需要free
 */
char *strcat_ex(const char *str, const char *str2)
{
	int len;
	char *ptr;
	
	len = strlen(str) + strlen(str2);
	ptr = (char *)malloc((len+1) * sizeof(char));
	if(ptr == NULL)
	{
		fprintf(stderr, "malloc failed.\n");
		return NULL;
	}
	memset(ptr, 0, len+1);
	strcat(ptr, str);
	strcat(ptr, str2);
	
	return ptr;
}

/*
 *	key 中间不能有 " 字符
 * 支持中文
 */
int xm_line_key_val(char *line, char *key, int key_len, char *val, int val_len)
{
	char *p_start, *p_end;
	int cpy_len;
	
	p_start = strchr(line, '"');
	if(p_start == NULL)
	{
		return -1;
	}
	p_end = p_start + 1;
	p_end = strchr(p_end, '"');
	if(p_end == NULL)
	{
		return -1;
	}
	cpy_len = ((key_len-1) > (p_end - p_start + 1))?(p_end - p_start + 1):(key_len - 1);
	strncpy(key, p_start, cpy_len);
	key[cpy_len] = '\0';
	
	p_start = strchr(p_end+1, '"');
	p_end = strrchr(line, '"');
	cpy_len = ((val_len-1) > (p_end - p_start + 1))?(p_end - p_start + 1):(val_len - 1);
	strncpy(val, p_start, cpy_len);
	val[cpy_len] = '\0';
	return 0;
}

/*
 * 分配内存,判断是否成功,设置分配的空间为'\0';
 * 失败返回NULL, 输出错误信息;
 */
void *xm_malloc(FILE *out, const int len) 
{ 
	void *ptr; 
	ptr = (void *)malloc(len * sizeof(char)); 
	if(ptr == NULL) 
	{
		fprintf(out, "malloc size %d failed\n", len);  
		return NULL;
	} 
	memset(ptr, 0, len); 
	return ptr; 
}

/*
 * 打开分件,判断是否成功, 成功返回文件流指针,失败返回null, 输出错误信息
 */
FILE *xm_fopen(FILE *out, const char *path, const char *mode)
{
	FILE *fp;
	fp = fopen(path, mode);
	if(fp == NULL)
	{
		fprintf(out, "fopen %s failed with mode %s\n", path, mode);
		return NULL;
	}
	return fp;	
}

/*
 *	去除字符串两边带有的 " 字符或者 isspace 字符
 */
int trim_boundary_quoto(char *str)
{
	char *bak;
	int len;
	char *p_start, *p_end;
	
	len = strlen(str);
	bak = (char *)xm_malloc(stderr, len+1);
	if(bak == NULL)
	{
		return -1;
	}
	strncpy(bak, str, len);
	bak[len] = '\0';
	
	p_start = &bak[0];
	while(isspace(*p_start))
	{
		p_start ++;
	}
	if(*p_start == '"')
	{
		p_start ++;
	}
	p_end = &bak[len-1];
	while(isspace(*p_end))
	{
		p_end --;
	}
	if(*p_end == '"')
	{
		p_end --;
	}
	strncpy(str, p_start, p_end-p_start+1);
	str[p_end - p_start + 1] = '\0';
	
	return 0;
}

char *tr ( char *s )
{
	int i = 0;
	int j = strlen ( s ) - 1;
	int k = 0;

	while ( isspace ( s[i] ) && s[i] != '\0' )
	i++;

	while ( isspace ( s[j] ) && j >= 0 )
	j--;

	while ( i <= j )
	s[k++] = s[i++];

	s[k] = '\0';

	return s;
}

/*
 *	将日志信息前加上 datetime 输出到标准错误流
 */
void xm_log(char *format, ... )
{
	DATE_TIME_T dt;
	va_list ap;
	char buf[FORMAT_LEN] = {'\0'};
	
	date_time(&dt, 0);
	fprintf(stderr, "\n%s:\n", (char *)&dt);
	
	va_start(ap, format);
	vsnprintf(buf, FORMAT_LEN, format, ap);
	va_end(ap);
	
	fprintf(stderr, "%s", buf);
	fflush(stderr);
}

/*
 *	作者: xm
 *	日期: 2012-11-02
 *  作用: 重定向 标准输出流 和 标准错误流 到一个文件
 */
int xm_redirect(const char *path)
{
	int fd;
	
	if(path == NULL)
	{
		return 0;
	}
	
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC);
	if(fd == -1)
	{
		fprintf(stderr, "open %s failed: %s\n", path, strerror(errno));
		return -1;
	}
	
	close(1);
	close(2);
	dup(fd);
	dup(fd);
	close(fd);
	
	return 0;
}


