#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h> /* time for randomizer*/ 


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, n;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    int guess, aim, flag;
    char buffer[10], answer[1];
    
    flag = 0;

    /* generate random integer from 0 to 99 */
    srand(time(NULL));
    aim = rand() % 100;

    printf("This is aim: %d\n", aim);   

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
       error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");

    listen(sockfd, 1);

    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) 
        error("ERROR on accept");

    while (flag == 0) {
		
        bzero(buffer, sizeof(buffer));
        n = read(newsockfd, buffer, sizeof(buffer));
        if (n < 0) 
			error("ERROR reading from socket");
        guess = atoi(buffer);
        printf("Client's guess: %d\n", guess);

        if (guess > aim)
            answer[0] = 'L';
        else if (guess < aim)
            answer[0] = 'H';
        else if (guess == aim) {
            answer[0] = 'E';
            flag = 1;
        }

        n = write(newsockfd, answer, 1);
        if (n < 0) 
            error("ERROR on accept");
    }  
    printf("Correct\n");   
    close(newsockfd);   
    close(sockfd);

    return 0; 
}
