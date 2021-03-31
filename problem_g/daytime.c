#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<ctype.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<netinet/in.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <resolv.h>
#include "openssl/ssl.h"
#include "openssl/err.h"
#define FAIL    -1
#define    HOSTNAMELEN 40 // maximal host name length
#define BUFLEN  1024    // maximum response size 
#define PORT  13 // port of daytime server 
SSL_CTX* Init_CTX_Server(void){
    SSL_METHOD *method;
	SSL_CTX *ctx;
	OpenSSL_add_all_algorithms();	/* load & register cryptos */
	SSL_load_error_strings();	/* load all error messages */
	method = TLSv1_2_server_method();	/* create server instance */
	ctx = SSL_CTX_new(method);	/* create context */
    if (ctx==NULL){
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}


void Load_Server_Certificates(SSL_CTX* ctx, char* CertFile, char* KeyFile){
    /* set the local certificate from CertFile */
    if(SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM)<=0){
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* set the private key from KeyFile (may be the same as CertFile) */
    if(SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM)<=0){
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* verify private key */
    if(!SSL_CTX_check_private_key(ctx)){
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
}




 int main(int argc, char *argv[]) {
	SSL_library_init();//Initialize the SSL library
	/* Initialize the OpenSSL library */
    SSL_CTX *ctx = Init_CTX_Server();
	/* Load Server's Certificates */
	Load_Server_Certificates(ctx, "mycert.pem", "mycert.pem");
    int nSystemCallReturn;  // system calls return value storage
    int nSockDesc;          // socket descriptor
    char cBuffer[BUFLEN+1]; // buffer server answer
    char* pc;               // pointer into the buffer
    struct sockaddr_in sa;  // Internet address struct
    struct hostent* hen;    // host-to-IP translation
    if (argc < 2) {
        fprintf(stderr, "host name missing\n");
        exit (1);
    }
    hen = gethostbyname(argv[1]);
    if (!hen) {
        perror("couldn't resolve host name");
    }
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(PORT);
    memcpy(&sa.sin_addr.s_addr, hen->h_addr_list[0], hen->h_length);
    nSockDesc = socket(AF_INET, SOCK_STREAM, 0);
    if (nSockDesc < 0) {
        perror("socket: allocation failed");
    }
    nSystemCallReturn = connect(nSockDesc, (struct sockaddr *)&sa, sizeof(sa));
    if (nSystemCallReturn) {
        perror("connect");
    }
    pc = cBuffer;
    while (nSystemCallReturn = read(nSockDesc, pc, BUFLEN - (pc-cBuffer))) {
        pc += nSystemCallReturn;
    }
    close(nSockDesc);
    *pc = '\0';
    printf("\nTime: %s\n", cBuffer);
            SSL_CTX_free(ctx);	/* release context */

    return 0;

 }
/*
// Compile the Server : gcc -Wall -o server daytime.c -L/usr/lib -lssl -lcrypto

./server time.nist.gov
