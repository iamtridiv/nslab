#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdlib.h>

#define MAX_LENGTH 10240

int main()
{
	int sockfd,retval;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr;
	char buff[50];
	sockfd=socket(AF_INET,SOCK_STREAM,0);

	if(sockfd==-1)
		printf("\nSocket Creation Error");

	// printf("sockfd: %i\n",sockfd);
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(2048);
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	retval=connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));

	if(retval==-1)
		printf("Connection error");


	while(1){

		printf ("Enter the string to reverse:\n");

		char string[MAX_LENGTH];

		fgets(string, MAX_LENGTH, stdin);

		char filename[MAX_LENGTH] = "cipher.bin";

		char command[MAX_LENGTH] = "printf '";

		strcat(command, string);
		strcat(command, "' | openssl enc -aes-256-cbc -pass pass:vaibscpdi -out ");
		strcat(command, filename);

		system(command);

		sentbytes=send(sockfd, filename, sizeof(filename), 0);

		recedbytes=recv(sockfd, string, sizeof(string), 0);

		strcpy(filename, string);

		strcpy(command, "openssl enc -d -aes-256-cbc -pass pass:vaibscpdi -in ");
        strcat(command, filename);

        FILE *fp;
        char res[MAX_LENGTH] = "";
        char op[MAX_LENGTH] = "";

		fp = popen(command, "r");

        while (fgets(res, sizeof(res), fp) != NULL) {
            strcat(op, res);
        }

        pclose(fp);

		printf("\n\nReverse:%s\n\n", op);

		strcpy(command, "rm ");
        strcat(command, filename);
        system(command);

	}

	return(0);
}
