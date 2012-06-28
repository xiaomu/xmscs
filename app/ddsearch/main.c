/*****************************************************************************
 *  Copyright          :  2012, All Rights Reserved.
 *
 *  Date               :  2012-06-28 10:39:58
 *  Author/Corporation :  Dengzhaoqun/
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "getopt.h"
#include "url_encode.h"

#define CMD_LEN 2048
#define URL_LEN 1024
#define BUF_LEN 1024

int help();
int do_wget(char *query, int pn, char *out_file);
int do_parse(char *out_file, int n);

int main(int argc, char *argv[])
{
	int c;
	char *query, *site=NULL;
	char ue_query[URL_LEN] = {'\0'};
	char wd[URL_LEN] = {'\0'};
	int n = 10;		//默认返回搜索的条数
	int flag, ret, i;
	char *out_file = "ddsearch_temp";
	int pages, left;

	flag = 0;
	while(1)
	{
		c = getopt(argc, argv, "q:n:s:");
		if(c == -1)
		{
			break;
		}
		switch(c)
		{
			case 'q':
				query = optarg;
				flag = 1;
				break;
			case 'n':
				n = atoi(optarg);
				break;
			case 's':
				site = optarg;
				break;
		}
	}

	if(!flag)
	{
		help();
		return -1;
	}

	if(site == NULL)
	{
		sprintf(wd, "inurl:%s", query);
	}
	else
	{
		sprintf(wd, "site:%s inurl:%s", site, query);
	}
	
	ret = URLEncode(wd, strlen(wd), ue_query, URL_LEN);
	if(ret == 0)
	{
		printf("URLEncode failed\n");
		return -1;
	}

	pages = n/10;	// 百度搜索一页有10条信息
	left = n%10;
	for(i=0; i<pages; i++)
	{
		do_wget(ue_query, i, out_file);
		do_parse(out_file, 10);
	}
	if(left > 0)
	{
		do_wget(ue_query, pages, out_file);
		do_parse(out_file,left);
	}


	return 0;
}


int help()
{
	char *menu[] = {
		"--* author:dengzhaoqun *--\n",
		"--* date:2012-06-28 10:39:58 *--\n\n",
		"ddsearch -q \"query string\" [-s \"site\"] [-n number]\n",
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


int do_wget(char *query, int pn, char *out_file)
{
	char *temp = "temp";
	char cmd[CMD_LEN] = {'\0'};
	sprintf(cmd, "wget --output-document=%s \"http://www.baidu.com/s?wd=%s&pn=%d\" > %s 2>&1", 
		out_file, query, pn*10, temp);
	//printf("wget cmd: %s\n", cmd);
	system(cmd);

	return 0;
}

int do_parse(char *out_file, int n)
{
	FILE *fp;
	char buf[BUF_LEN] = {'\0'};
	int i;
	char *cls = "class=\"result\"";
	char *href = "href=";
	int len_cls, len_href;

	fp = fopen(out_file, "r");
	if(fp == NULL)
	{
		printf("fopen %s failed.\n", out_file);
		return -1;
	}

	len_cls = strlen(cls);
	len_href = strlen(href);

	for(i=0; i<n; i++)
	{
		while(fscanf(fp, "%s", buf) != EOF)
		{
			//memset(buf, 0, BUF_LEN);
			if(strncmp(cls, buf, len_cls) == 0)
			{
				while(fscanf(fp, "%s", buf) != EOF)
				{
					//memset(buf, 0, BUF_LEN);
					if(strncmp(href, buf, len_href) == 0)
					{
						buf[strlen(buf)-1]='\0';
						printf("%s\n", &buf[6]);
						break;
					}
				}
			}
		}
	}

	fclose(fp);

	return 0;
}



