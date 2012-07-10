#include <windows.h>
#include <iostream>
#include <UrlMon.h>
#include <tchar.h>
#include <string.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "urlmon.lib")
using namespace std;

#define URL_LEN 1024

char *char_add(char *str, char *str2);

int main(int argc, char* argv[])
{
	char num[100] = {'\0'};
	char dir[100] = {'\0'};
	char ext[100] = {'\0'};
	char pre_url[URL_LEN];
	char temp_url[URL_LEN];
	char cmd[200]= {'\0'};
	char file_name[200];
	char *p, *q;
	char *seq = NULL, *temp;
	HRESULT hr;
	int flag;

	if(argc != 2)
	{
		cout << "dddpic pic_url" << endl;
		return -1;
	}

	p = strrchr(argv[1], '.');
	if(p != NULL)
	{
		strcpy(ext, p);
		q= strrchr(argv[1], '/');
		if(q != NULL)
		{
			strncpy(num, q+1, p-q-1);
			p = q - 1;
			while((*p != '/') && (p > &argv[1][0]))
			{
				p --;
			}
			if(p == &argv[1][0])
			{
				cout << "url wrong" << endl;
				return -1;
			}
			strncpy(dir, p+1, q-p-1);
		}
		else
		{
			cout << "url wrong" << endl;
			return -1;
		}
	}
	else
	{
		cout << "url wrong" << endl;
		return -1;
	}

	memset(pre_url, 0, URL_LEN);

	if(access(dir, 0) != 0)
	{
		sprintf(cmd, "mkdir %s", dir);	
		system(cmd);
	}

	strncpy(pre_url, argv[1], p-&argv[1][0]+1);

	hr = S_OK;
	flag = 0;
	seq = num;
	while(hr == S_OK)
	{
		memset(temp_url, 0, URL_LEN);
		memset(file_name, 0, 200);
		temp = seq;
		seq = char_add(seq, "1");
		if((temp != NULL) && flag)
			free(temp);
		sprintf(file_name, "%s/%s%s", dir, seq, ext);		
		sprintf(temp_url, "%s%s", pre_url, file_name);
		hr = URLDownloadToFile(0, _T(temp_url),_T (file_name), 0,NULL);
		flag = 1;
	}

	cout << "download finish" << endl;

	return 0;

}


char *char_add(char *str, char *str2)
{
	char *ptr = NULL, *result = NULL;
	int flag;
	int i, j, temp;
	int len, len2, n;
	
	len = strlen(str) ;
	len2 = strlen(str2) ;
	n = (len >= len2?len:len2);
	
	if((str == NULL) || (str2 == NULL))
	{
		return NULL;
	}
	
	ptr = (char *)malloc((n+2) * sizeof(char));
	//	result = malloc((n+1) * sizeof(char));
	if(ptr == NULL)
	{
		printf("malloc failed.\n");
		return NULL;
	}
	
	memset(ptr, 0, n+2);
	
	
	flag = 0;
	if(len >= len2)
	{
		for(i= len-1, j=len2-1; i>=len - len2; i--, j--)
		{
			temp = (str[i] - '0') + (str2[j] - '0') + flag;
			ptr[i+1] = temp % 10 + '0';
			flag = temp/10;
		}
		for(i; i>=0; i--)
		{
			temp = (str[i] - '0') + flag;
			ptr[i+1] = temp % 10 + '0';
			flag = temp / 10;
		}
		if(flag == 1)
		{
			ptr[0] = '1';
		}
	}
	else
	{
		for(i= len-1, j=len2-1; j>=len2 - len; i--, j--)
		{
			temp = (str[i] - '0') + (str2[j] - '0') + flag;
			ptr[j+1] = temp % 10 + '0';
			flag = temp/10;
		}
		for(j; j>=0; j--)
		{
			temp = (str2[j] - '0') + flag;
			ptr[j+1] = temp % 10 + '0';
			flag = temp / 10;
		}
		if(flag == 1)
		{
			ptr[0] = '1';
		}
	}
	
	if(ptr[0] != '\0')
	{
		return ptr;
	}
	else
	{
		result = (char *)malloc((n+2) * sizeof(char));
		if(result == NULL)
		{
			printf("malloc failed.\n");
			return NULL;
		}
		memset(result, 0, n+2);
		strcpy(result, &ptr[1]);
		free(ptr);
		return result;
	}
}
