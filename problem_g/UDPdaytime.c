#define __USE_BSD 1
#include <stdarg.h>
#include <errno.h>
#include <netinet/in.h>
#include <time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define BUFSIZE 64
#define UNIXEPOCH 2208988800 
#define MSG "what time is it?\n"

#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif
 extern int errno;
typedef unsigned short u_short;
extern int errno;
int errexit(const char *format, ...);


typedef unsigned long u_long;
extern int errno;

int connectsock(const char *host, const char *service, const char *transport);
int connectUDP(const char *host, const char *service);

int errexit(const char *format, ...);

int main(int argc, char *argv[])
{
char *host = "localhost";
char *service = "time";
time_t now;
int s, n;

switch (argc)
{
case 1:
host = "localhost";
break;
case 3:
service = argv[2];
case 2:
host = argv[1];
break;
default:
fprintf(stderr, "usage: UDPtime [host [port]]\n");
exit(1);
}


s = connectUDP(host, service);

(void) write(s, MSG, strlen(MSG));

n = read(s, (char *)&now, sizeof(now));
if (n < 0)
errexit("read failed: %s\n", strerror(errno));

now = ntohl((u_long)now);
now -= UNIXEPOCH;
printf("%s", ctime(&now));
exit(0);
}

int connectUDP(const char *host, const char *service )
{
return connectsock(host, service, "udp");
}


int connectsock(const char *host, const char *service, const char *transport )
{
struct hostent *phe;
struct servent *pse;
struct protoent *ppe;
struct sockaddr_in sin;
int s, type;

memset(&sin, 0, sizeof(sin));
sin.sin_family = AF_INET;


if ( pse = getservbyname(service, transport) )
	sin.sin_port = pse->s_port;
else if ( (sin.sin_port = htons((u_short)atoi(service))) == 0 )
	errexit("can't get \"%s\" service entry\n", service);


if ( phe = gethostbyname(host) )
	memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
	errexit("can't get \"%s\" host entry\n", host);


if ( (ppe = getprotobyname(transport)) == 0)
	errexit("can't get \"%s\" protocol entry\n", transport);


if (strcmp(transport, "udp") == 0)
	type = SOCK_DGRAM;
else
	type = SOCK_STREAM;


s = socket(PF_INET, type, ppe->p_proto);

if (s < 0)
	errexit("can't create socket: %s\n", strerror(errno));


if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	errexit("can't connect to %s.%s: %s\n", host, service);

strerror(errno);

return s;
}


errexit(const char *format, ...)
{
va_list args;
va_start(args, format);
vfprintf(stderr, format, args);
va_end(args);
exit(1);
}
