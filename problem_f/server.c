#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>

#define MAX_LENGTH 10240

int main()
{
	int sockfd,newsockfd,retval;
	socklen_t actuallen;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr,clientaddr;

	char buff[50];
	int a=0;
	sockfd=socket(AF_INET,SOCK_STREAM,0);

	if(sockfd==-1)
		printf("\nSocket creation error");

	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(2048);
	serveraddr.sin_addr.s_addr=htons(INADDR_ANY);
	retval=bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));

	if(retval==-1)
	{
		printf("Binding error");
		close(sockfd);
	}

	retval=listen(sockfd,1);
	printf("Connection created successfully\n");
	printf("I am listening...");
	if(retval==-1)
		close(sockfd);


	actuallen=sizeof(clientaddr);
	newsockfd=accept(sockfd,(struct sockaddr*)&clientaddr,&actuallen);


	if(newsockfd==-1)
		close(sockfd);

	
	while(1){

		char buff[MAX_LENGTH];

		char filename[MAX_LENGTH] = "";

		char command[MAX_LENGTH] = "";

		recedbytes=recv(newsockfd, buff, sizeof(buff), 0);

		strcpy(filename, buff);

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

        strcpy(buff, op);

		int n = strlen(buff);

		for (int i = 0; i < n / 2; i++){
	        char temp = buff[i];
	    	buff[i] = buff[n - i - 1];
	    	buff[n - i - 1] = temp;
	    }
		
		strcpy(command, "printf '");
		strcat(command, buff);
		strcat(command, "' | openssl enc -aes-256-cbc -pass pass:vaibscpdi -out ");
		strcat(command, filename);

		system(command);

		sentbytes=send(newsockfd, filename, sizeof(filename), 0);


	}
		
	

	return(0);
}

