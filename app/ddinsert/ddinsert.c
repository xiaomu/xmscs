/*****************************************************************************
 *  Copyright          :  All Rights Reserved.
 *
 *  Date               :  2012-09-05 23:50:40
 *  Author/Corporation :  Dengzhaoqun
 *  Email              :  dengzhaoqun@163.com
 *****************************************************************************/

// insert Copyright into source code files.
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUF_LEN 1024
#define FILE_NAME_LEN 1024

static char *insert = ""
"/*****************************************************************************\n"
" *  Copyright          :  All Rights Reserved.\n"
" *\n"
" *  Date               :  %s\n"
" *  Author/Corporation :  Dengzhaoqun\n"
" *  Email              :  dengzhaoqun@163.com\n"
" *****************************************************************************/\n";	

int help();

int main(int argc, char *argv[])
{
	FILE *from, *to;
	char *from_path;
	char buf[BUF_LEN];
	int ret;
	time_t myt;
	struct tm *mytm;

	if(argc < 2)
	{
		help();
		return -1;
	}

	from_path = argv[1];
	from = fopen(from_path, "r");
	if(from == NULL)
	{
		perror("fopen failed");
		return -1;
	}

	to = tmpfile();
	if(to == NULL)
	{
		perror("tmpfile failed");
		return -1;
	}

	time(&myt);
	mytm = localtime(&myt);
	memset(buf, 0, BUF_LEN);
	strftime(buf, BUF_LEN, "%Y-%m-%d %H:%M:%S", mytm);

	fprintf(to, insert, buf);

	// copy file content
	memset(buf, 0, BUF_LEN);
	while( (ret = fread(buf, sizeof(char), BUF_LEN, from)) > 0)
	{
		fwrite(buf, ret, 1, to);
	}

	fclose(from);
	
	from = fopen(from_path, "w");
	if(from == NULL)
	{
		perror("fopen failed");
		return -1;
	}
	rewind(to);
	memset(buf, 0, BUF_LEN);
	while( (ret = fread(buf, sizeof(char), BUF_LEN, to)) > 0)
	{
		fwrite(buf, ret, 1, from);
	}
	fclose(from);
	fclose(to);

	return 0;

}

int help()
{
	char *menu[] = {
		"ddinsert filename\n",
		NULL
	};

	char **ptr;

	ptr = menu;
	while(*ptr != NULL)
	{
		printf("%s", *ptr);
		*ptr ++;
	}

	return 0;
}