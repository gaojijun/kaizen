#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define MAXLINE 1024

typedef struct sockaddr SA;

int main(int argc, char **argv) {

	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[MAXLINE];
	time_t ticks;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd <= 0) {
		printf("failed to create listen socket");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(8888);	/* daytime server */

	if ( bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0 ) {
		printf("bind err");
		exit(1);
	}

	if ( listen(listenfd, 100) != 0 ) {
		printf("listen err");
		exit(1);
	}

	for ( ; ; ) {
		connfd = accept(listenfd, (SA *) NULL, NULL);
		if ( connfd <= 0 ) {
			printf("accept err");
			exit(1);
		}

        	ticks = time(NULL);
        	snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        	if ( write(connfd, buff, strlen(buff)) <= 0 ) {
			printf("write err");
			exit(1);
		}

		if ( close(connfd) != 0 ) {
			printf("close err");
			exit(1);
		}
	}
}
