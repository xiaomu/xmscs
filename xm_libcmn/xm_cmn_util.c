#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

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
 * �ϳ��ַ��� 
 * len: �ϳ��ַ�������󳤶�
 * fmt: ��ʽ
 * ... : �ɱ���� 
 * ���غϳɵ��ַ���, ʧ�ܷ���NULL
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
 * ���#define��DEBUG, ��printf�������
 * format ��ʽ
 * ... : �ɱ����
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
 * ��ʾ�ַ��������еİ�����Ϣ
 * menu: �ַ�������ָ��,ָ����ַ�����NULL����;
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

