#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <stdarg.h>
#include <arpa/inet.h>

#define MAXLINE 1024

typedef struct sockaddr SA;

void err_quit(char* fmt, ...)
{
    va_list args;
    va_start(args,fmt);
    printf(fmt,args);
    printf("\n");
    va_end(args);
    exit(1);
}

int main(int argc, char **argv) {

	int sockfd, n;
	char recvline[MAXLINE + 1];
	struct sockaddr_in servaddr;

	if (argc != 2)
		err_quit("usage: a.out < IPaddress >");

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_quit("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(8888);
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_quit("connect error");

	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF) 
			err_quit("fputs error");
	}
	if (n < 0)
		err_quit("read error");

	exit(0);
}
