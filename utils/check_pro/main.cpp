#include<windows.h>
#include<tlhelp32.h>
#include<stdio.h>
#include <string.h>

#define MAX_LEN 1000
static int time = 10;	// default value
static char path[MAX_LEN]; // default value

int GetProName(char *proName);
int is_digit(const char *str);
int parse_opt(char *opt, char *val);
int help();

int main(int argc, char *argv[])
{
	char proName[MAX_LEN] = {'\0'};
	char fullName[MAX_LEN + MAX_LEN] = {'\0'};
	char buf[MAX_LEN];
	int ret;
	bool exist = false;
	HINSTANCE hInstance;
	PROCESSENTRY32 pe32;



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

    
    pe32.dwSize = sizeof(pe32);

	while(true)
	{
		HANDLE hProcessSnap =::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if(hProcessSnap == INVALID_HANDLE_VALUE)
		{
			perror("CreateToolhelp32Snapshot failed");
			return -1;
		}
		//遍历进程快照。轮流显示每个进程的信息
		BOOL bMore =::Process32First(hProcessSnap, &pe32);
		while(bMore)
		{
			memset(buf, 0, MAX_LEN);
			strncpy(buf, pe32.szExeFile, MAX_LEN);
			
			ret = strnicmp(buf, proName, MAX_LEN);
			if(ret == 0)
			{
				printf("%s is already running\n", proName);
				exist = true;
				break;
			}

			bMore =::Process32Next(hProcessSnap, &pe32);
		}

		if(!exist)
		{
			memset(fullName, 0, MAX_LEN + MAX_LEN);
			strncat(fullName, path, MAX_LEN);
			strncat(fullName, proName, MAX_LEN);
			hInstance = ShellExecute(NULL,"open", fullName, NULL,NULL,SW_SHOWNORMAL); 
			if((int)hInstance <= 32)
			{
				printf("Start %s failed.\n", fullName);

			}
			else
			{
				printf("Start %s succeed.\n", fullName);
			}
			
		}


		//清除snapshot对象
		::CloseHandle(hProcessSnap);

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
			if(!is_digit(val))
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
		default:
			NULL;
		}
	}

	return 0;
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

int help()
{
	char *menu[] = {
		"\ncheck_pro proName [-t time(second)] [-p path]\n\n",
		"example:\n",
		"\tcheck_pro test.exe\n",
		"\tcheck_pro test.exe -t 3\n",
		"\tcheck_pro test.exe -t 2 -p d:\\app\\\n\n",
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


	