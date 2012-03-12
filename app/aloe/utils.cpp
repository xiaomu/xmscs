#include <stdio.h>
#include <string.h>
#include "utils.h"
/*
	return value
	0: is not digit
	1: is digit
*/

int is_digit(const char *str)
{
        size_t len = strlen(str);
        while(len > 0) {
                if (*str < '0' || *str > '9') {
                        return 0;
                }
                str++;
                len--;
        }
        return 1;
}



int help()
{
	char *menu[] = {
		"\naloe proName [-t time(second)] [-p path] [-t port] [-f file]\n\n",
		"example:\n",
		"\taloe test.exe -t 2 -p d:\\app\\ -o 22 -f d:\\app\\state.xml\n\n",
		NULL
	};

	char **p;
	p = menu;
	while(*p != NULL)
	{
		printf("%s", *p);
		*p ++;
	}

	return 0;
}

// 这个函数很有可能需要重写，暂时先写成这样一个借口
int alterInXml(char *key, char *val, char *file)
{
	FILE *fp;

	char *block1 = "<?xml version=\"1.0\" encoding=\"GB2312\"?>\n<Live State=\"1\" Port=\"";
	char *block2 = "\">\n</Live>";

	fp = fopen(file, "w");
	if(fp == NULL)
	{
		printf("fopen %s failed.\n", file);
		return -1;
	}

	fwrite(block1, strlen(block1), 1, fp);
	fwrite(val, strlen(val), 1, fp);
	fwrite(block2, strlen(block2), 1, fp);

	fclose(fp);

	return 0;
}
