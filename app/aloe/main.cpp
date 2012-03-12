
/*
	company: pps
	project name: aloe
	author: dengzhaoqun
	date:	2012/03/12	
*/

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "port.h"
#include "process.h"
#include "utils.h"

int parse_opt(char *opt, char *val);

#define MAX_LEN 1000
static int time = 10;	// default value
static char path[MAX_LEN] = {'\0'}; // default value
static int port = 8899; // default value
static char *file = "state.xml"; // default value

int main(int argc, char *argv[])
{
	char proName[MAX_LEN] = {'\0'};
	char buf[MAX_LEN] = {'\0'};
	int ret;

	// parse argument
	if(argc < 2)
	{
		help();
		return -1;
	}

	strncpy(proName, argv[1], MAX_LEN);
	{
		if(strlen(proName) == 0)
		{
			printf("proName not exist");
			return -1;
		}
	}

	if(argc > 3)
	{
		parse_opt(argv[2], argv[3]);
	}
	if(argc > 5)
	{
		parse_opt(argv[4], argv[5]);
	}
	if(argc > 7)
	{
		parse_opt(argv[6], argv[7]);
	}
	if(argc > 9)
	{
		parse_opt(argv[8], argv[9]);
	}



	while(1)
	{
		// check the process
		ret = isProExist(proName);
		if(ret == 0)
		{
			startPro(proName, path);
		}

		

		if(!isPortOpen(port))
		{
			sprintf(buf,"%d", port);
			alterInXml("port", buf, file);
		}
		else
		{
			sprintf(buf,"%d", 0);
			alterInXml("port", buf, file);
		}

		Sleep(time * 1000);
	}


	return 0;
}


int parse_opt(char *opt, char *val)
{
	if(opt[0] == '-')
	{
		switch(opt[1])
		{
		case 't':
			if(is_digit(val))
			{
				time = atoi(val);
				if(time == 0)
				{
					time = 10;
				}
			//	printf("time: %d\n", time);
			}
			else
			{
				printf("time should be a num.\n");
				return -1;
			}
			break;
		case 'p':
			memset(path, 0, MAX_LEN);
			strncpy(path, val, MAX_LEN);
			break;
		case 'o':
			if(is_digit(val))
			{
				port = atoi(val);
				if((port > 65535) || (port < 0))
				{
					printf("port value should be a num between 1 and 65535.\n");
					return -1;
				}
			}
			else
			{
				printf("port value should be a num between 1 and 65535.\n");
				return -1;
			}
			break;
		case 'f':
			file = val;
			break;

		default:
			NULL;
		}
	}

	return 0;
}
