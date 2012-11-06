/*****************************************************************************
 *  Copyright          :  All Rights Reserved.
 *
 *  Date               :  2012-09-11 17:52:25
 *  Author/Corporation :  Dengzhaoqun
 *  Email              :  dengzhaoqun@163.com
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>

/*
 * ��ȡ�������е�ip
 * ���ص�ipֵ֮���� , �ָ�
 * eg:
 int main()
{
	char *ips;
	ips = get_ip();
	free(ips);
	return 0;
}
 */
char *get_ip()
{
	int fd, num;
	struct ifreq ifq[16];
	struct ifconf ifc;
	int i;
	char *ips, *tmp_ip;
	char *delim = ",";
	int val;
	
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(fd < 0)
	{
		fprintf(stderr, "socket failed\n");
		return NULL;
	}
	ifc.ifc_len = sizeof(ifq);
	ifc.ifc_buf = (caddr_t)ifq;
	if(ioctl(fd, SIOCGIFCONF, (char *)&ifc))
	{
		fprintf(stderr, "ioctl failed\n");
		return NULL;
	}
	num = ifc.ifc_len / sizeof(struct ifreq);
	if(ioctl(fd, SIOCGIFADDR, (char *)&ifq[num-1]))
	{
		fprintf(stderr, "ioctl failed\n");
		return NULL;
	}
	close(fd);
	
	val = 0;
	for(i=0; i<num; i++)
	{
		tmp_ip = inet_ntoa(((struct sockaddr_in*)(&ifq[i].ifr_addr))-> sin_addr);
		if(strcmp(tmp_ip, "127.0.0.1") != 0)
		{
			val++;
		}
	}
	
	ips = (char *)malloc(val * 16 * sizeof(char));
	if(ips == NULL)
	{
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}
	memset(ips, 0, val * 16 * sizeof(char));
	val = 0;
	for(i=0; i<num; i++)
	{
		tmp_ip = inet_ntoa(((struct sockaddr_in*)(&ifq[i].ifr_addr))-> sin_addr);
		if(strcmp(tmp_ip, "127.0.0.1") != 0)
		{
			if(val > 0)
			{
				strcat(ips, delim);
			}
			strcat(ips, tmp_ip);
			val ++;
		}
	}
	
	return ips;
}

/*
 * ��host����ȡip��ַ
 * host: ����
 * ip:ָ����ip�Ŀռ�(>=16byte)
 * ��������0, ʧ�ܷ���-1
 */
int get_ip_from_host(char *host,  char *ip)
{
	struct hostent *hent;
	
	hent = gethostbyname(host);
	if(hent == NULL)
	{
		return -1;
	}
	strcpy(ip, inet_ntoa(*(struct in_addr*)(hent->h_addr_list[0])));
	
	return 0;
}

/*
 *�ж�һ���ļ��Ƿ��ǿ��ļ�
 * �ļ������ǿ��ļ�����1
 * �ļ����ڲ��ǿ��ļ�����0
 * ������ -1
 */
int xm_is_empty_file(char *path)
{
	struct stat st;
	
	trace_fun();
	
	if(stat(path, &st) == 0)
	{
		if(st.st_size == 0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return -1;
	}
}

in_addr_t resolve_host_name(char *hname)
{
	in_addr_t inetaddr;
	struct hostent *h_ent;

	if((inetaddr = inet_addr(hname)) == -1)
	{
		if(!(h_ent = gethostbyname (hname)))
		{
			fprintf(stderr, "[!] can't resolve host %.100s\n", hname);
			return(-1);
		}

		bcopy(h_ent->h_addr, (char *)&inetaddr, h_ent->h_length);
	}

	return(inetaddr);
}

