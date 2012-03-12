#include<windows.h>
#include <stdio.h>
#include <string.h>

// 思想： 和check_pro一样，做成一个命令行工具

int is_digit(const char *str);

/* 
  return value
  -1: error
   0: port is open
   1: port is closed
*/

int main(int argc, char *argv[])
{

	FILE *fp;
	char buf[2048];
	int port = 8899; // default value
	char check[10] = {'\0'};
	char port_str[8] = {'\0'};

	if(argc > 1)
	{
		if(is_digit(argv[1]))
		{
			printf("port value should be a num.\n");
			return -1;
		}
		port = atoi(argv[1]);
		if((port > 65535) || (port < 0))
		{
			printf("port value should be a num between 1 and 65535.\n");
			return -1;
		}
		
	}
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
	

int is_digit(const char *str)
{
        size_t len = strlen(str);
        while(len > 0) {
                if (*str < '0' || *str > '9') {
                        return -1;
                }
                str++;
                len--;
        }
        return 0;
}