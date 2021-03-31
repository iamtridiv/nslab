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

#define MAXSIZE 9000

int main()
{
	int sockfd,retval;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr;
	char buff[MAXSIZE];
	sockfd=socket(AF_INET,SOCK_STREAM,0);

	if(sockfd==-1)
		printf("\nSocket Creation Error");

	// printf("sockfd: %i\n",sockfd);
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(8888);
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	retval=connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));

	if(retval==-1)
		printf("Connection error");

	char string[MAXSIZE] = "";
	recedbytes=recv(sockfd,string,MAXSIZE,0);

	printf("%s\n", string);
	while(1){
		printf("Enter something:\n");

		fgets(buff, MAXSIZE, stdin);
		strtok(buff, "\n");

		sentbytes=send(sockfd,buff,sizeof(buff),0);

		

		recedbytes=recv(sockfd,string,MAXSIZE,0);
		string[strlen(buff)] = '\0';


		char filename[MAXSIZE] = "cipher.bin";

		FILE *fp;
		char res[MAXSIZE] = "";
		char op[MAXSIZE] = "";

		char command[MAXSIZE] = "openssl enc -d -aes-256-cbc -pass pass:vaibscpdi -in ";

		strcat(command, filename);

		fp = popen(command, "r");

		while (fgets(res, sizeof(res), fp) != NULL) {
		    strcat(op, res);
		}

		pclose(fp);

		printf("\n%s\n\n\n", op);

		strcpy(command, "rm ");
		strcat(command, filename);
		system(command);

		// string[strlen(buff)] = '\0';
		// printf("%s\n", string);
		// printf("%ld\n", strlen(buff));
	}

	close(sockfd);

	return(0);
}
