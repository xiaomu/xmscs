
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
	char tmp[FILE_NAME_LEN];
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

	memset(tmp, 0, FILE_NAME_LEN);
	tmpnam(tmp);
	to = fopen(tmp, "w");
	if(to == NULL)
	{
		perror("fopen failed");
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
		fwrite(buf, sizeof(char), ret, to);
	}

	fclose(from);
	fclose(to);

	remove(from_path);
	rename(tmp, from_path);	

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
