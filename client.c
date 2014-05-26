#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    int guess, flag, num_of_attempts;
    char buffer[10], answer[1];

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    flag = 0;
    num_of_attempts = 0;

    while (flag != 1) 
    {
        num_of_attempts++;
        printf(">");
        bzero(buffer, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0)
             error("ERROR writing to socket");
             
        n = read(sockfd, answer, sizeof(buffer));
        if (n < 0) 
             error("ERROR reading from socket");

        if (answer[0] == 'L')
            printf("Lower\n");
        else if (answer[0] == 'H')
            printf("Higher\n");
        else if (answer[0] == 'E') {
            printf("Correct\n");
            flag = 1;
        }
        else
            error("ERROR Wrong message received");
    }

    printf ("Number of attempts: %d\n", num_of_attempts);
    close(sockfd);
    
    return 0;
}

