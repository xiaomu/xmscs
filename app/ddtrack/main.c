#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "getopt.h"

#define LEN 1024

int make_infor();
int post_infor(char *server, char *user, char *pwd, char *to);

int main(int argc, char *argv[])
{
	// parse the arguement
	int c;
	int n;
	char *server = NULL;
	char *user = NULL;
	char *pwd = NULL;
	char *to = NULL;

	n = 0;
	while(1)
	{
		c = getopt(argc, argv, "s:u:p:t:");
		if(c == -1)
			break;
		switch(c)
		{
			case 's':
				server = optarg;
				n++;
				break;
			case 'u':
				user = optarg;
				n++;
				break;
			case 'p':
				pwd = optarg;
				n++;
				break;
			case 't':
				to = optarg;
				n++;
				break;
		}
	}
	if(n != 4)
	{
		printf("ddtrack -s smtp_server -u username -p password -t receive_user\n");
		printf("example:\n");
		printf("\tddtrack -s smtp.163.com -u abc@163.com -p 121221 -t bbc@qq.com\n");
		return 0;
	}

	make_infor();
	post_infor(server, user, pwd, to);
	return 0;
}

int make_infor()
{
	system("printf \"%s   %s\\n\" user: %username% > infor");
	system("printf \"%s   %s\\n\\n\" time: %time% >> infor");
	system("ipconfig /all >> infor");
	system("tracert www.baidu.com >> infor");
	return 0;
}

int post_infor(char *server, char *user, char *pwd, char *to)
{
	char cmd[LEN] = {'\0'};

	sprintf(cmd, "blat -install %s %s 3 25", server, user);
	system(cmd);

	memset(cmd, 0, LEN);
	sprintf(cmd, "blat %s -to %s -s %s -u %s -pw %s", "infor", to, "ddtrack__report", user, pwd);
	system(cmd);

	return 0;
}
	

