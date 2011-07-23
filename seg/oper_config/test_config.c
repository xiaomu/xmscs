#include <stdio.h>
#include <unistd.h>

#include "oper_config.h"

#define PATH "config"
#define MAX_KEY_LEN 100
#define MAX_VAL_LEN 100

int test_add(void);
int test_del(void);
int test_read(void);
int test_alter(void);

int main(int argc, char *argv[])
{
	int opt;

	if((opt = getopt(argc, argv, "ar")) != -1)
	{
		switch(opt)
		{
			case 'a':
				test_add();
				break;
			case 'r':
				test_read();
				break;
#if 0			case 'd':
				test_del();
				break;
			case 't':
				test_alter();
				break;
#endif
			case '?':
				printf("unknown option: %c\n", optopt);
				break;
		}
	}

	return(0);
}


int
test_add(void)
{
	FILE *fp;
	char key[MAX_KEY_LEN] = {'\0'};
	char value[MAX_VAL_LEN] = {'\0'};
	int ret;

	fp = fopen(PATH, "a");
	if(fp == NULL)
	{
		perror("fopen failed");
		return(-1);
	}

	printf("key to add: ");
	scanf("%s", key);
	printf("value to add: ");
	scanf("%s", value);

	if((ret = add_config(fp, key, value)) == -1)
	{
		fclose(fp);
		printf("add failed.\n");
		return(-1);
	}

	fclose(fp);
	printf("%s = %s add succeed.\n", key, value);

	return(0);
}

int
test_read(void)
{
	FILE *fp;
	char key[MAX_KEY_LEN] = {'\0'};
	char *value = NULL;

	
	fp = fopen(PATH, "r");
	if(fp == NULL)
	{
		perror("fopen failed");
		return(-1);
	}

	printf("key to read: ");
	scanf("%s", key);

	if((value = read_config(fp, key)) == NULL)
	{
		fclose(fp);
		printf("read failed.\n");
		return(-1);
	}

	fclose(fp);
	printf("%s = %s read succeed.\n", key, value);

	return(0);
}

#if 0
int
test_del(void)
{
	FILE *fp;
	char key[MAX_KEY_LEN] = {'\0'};
	int ret;

	
	fp = fopen(PATH, "w+");
	if(fp == NULL)
	{
		perror("fopen failed");
		return(-1);
	}

	printf("key to del: ");
	scanf("%s", key);

	if((ret = del_config(fp, key)) == -1)
	{
		fclose(fp);
		printf("del failed.\n");
		return(-1);
	}

	fclose(fp);
	printf("del %s succeed.\n", key);

	return(0);
}

int
test_alter(void)
{
	FILE *fp;
	char key[MAX_KEY_LEN] = {'\0'};
	char value[MAX_VAL_LEN] = {'\0'};
	int ret;

	fp = fopen(PATH, "r+");
	if(fp == NULL)
	{
		perror("fopen failed");
		return(-1);
	}

	printf("key to alter: ");
	scanf("%s", key);
	printf("value to alter: ");
	scanf("%s", value);

	if((ret = alter_config(fp, key, value)) == -1)
	{
		fclose(fp);
		printf("alter failed.\n");
		return(-1);
	}

	fclose(fp);
	printf("%s = %s alter succeed.\n", key, value);

	return(0);
}
#endif





