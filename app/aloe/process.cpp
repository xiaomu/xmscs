#include <stdio.h>
#include <string.h>
#include <windows.h>
#include<tlhelp32.h>
#include "process.h"
#include "utils.h"

int isProExist(char *proName)
{

	PROCESSENTRY32 pe32;
	char buf[MAX_LEN];
	int exist = 0;
	int ret;

	pe32.dwSize = sizeof(pe32);

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
			exist = 1;
			break;
		}

		bMore =::Process32Next(hProcessSnap, &pe32);
	}

	::CloseHandle(hProcessSnap);

	return exist;
}

int startPro(char *proName, char *path)
{
	char fullName[MAX_LEN + MAX_LEN] = {'\0'};
	HINSTANCE hInstance;

	memset(fullName, 0, MAX_LEN + MAX_LEN);
	strncat(fullName, path, MAX_LEN);
	strncat(fullName, proName, MAX_LEN);
	hInstance = ShellExecute(NULL,"open", fullName, NULL,NULL,SW_SHOWNORMAL); 
	if((int)hInstance <= 32)
	{
		printf("Start %s failed.\n", fullName);
		return -1;
	}
	else
	{
		printf("Start %s succeed.\n", fullName);
		return 0;
	}
}

	



