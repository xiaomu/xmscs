#include <stdio.h> 
#include <winsock2.h> 
#pragma comment(lib,"ws2_32.lib")

int main(int argc, char *argv[]) 
{ 
	WSAData wsaData; 

	char host_name[255]; 

	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
	{ 
		printf("WSAStartup failed.\n");
		return -1;
	} 

	//获取本地主机名称 
	if (gethostname(host_name, sizeof(host_name)) == SOCKET_ERROR)
	{ 
		printf("gethostname failed.\n");
		return -1; 
	} 

	//从主机名数据库中得到对应的“主机” 
	struct hostent *phe = gethostbyname(host_name); 
	if (phe == 0) 
	{ 
		printf("gethostbyname failed.\n");
		return -1; 
	} 

	//循环得出本地机器所有IP地址 
	for (int i = 0; phe->h_addr_list[i] != 0; ++i) 
	{ 
		struct in_addr addr; 
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr)); 
		printf("%s\n", inet_ntoa(addr)); 
	} 

	WSACleanup(); 

	return 0; 
}