#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "port.h"
#include "utils.h"

int isPortOpen(int port)
{
	FILE *fp;
	char buf[2048];
	char check[10] = {'\0'};
	char port_str[8] = {'\0'};

	sprintf(port_str, "%d", port);
	check[0] = ':';
	strncat(check, port_str, 8);

	system("netstat -an>temp");

	fp = fopen("temp", "r");
	if(NULL == fp)
	{
		printf("fopen 'temp' failed.\n");
		return -1;
	}

	while(fscanf(fp, "%s", buf) != EOF)
	{
		if(strstr(buf, check) != NULL)
		{
			printf("port %s is open.\n", port_str);
			return 0;
		}
	}

	printf("port %s is closed.\n", port_str);

	fclose(fp);
		
	return 1;
}
