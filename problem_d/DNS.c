//To develop a client that contacts a given DNS server(name to ip address mapping is present in dns.txt) to resolve a given hostname.(Note:
//client refer dns.txt locally before request sent to DNS server)

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	FILE *fp;
	//Reading that dns.txt file 
	fp = fopen("/home/tridiv/nslab/problem_d/dns.txt","r");
	int counter=0;
	char buffer[50];
	char command[50] = "nslookup ";	
	while(!feof(fp))
	{
		char ch;
		while((ch=fgetc(fp))!='\n' && !feof(fp))
			buffer[counter++] = ch;
		buffer[counter] = '\0';
		counter = 0;
		strcat(command,buffer);
		system(command);
		strcpy(command,"nslookup ");
	}
}
