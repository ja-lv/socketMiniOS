#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

// takes in host name and port number
//compiled as g++ sample_client.cpp -o ../build/sample_client
//run: ../build/sample_client
int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);

    while(1){
        int status;
        int cid = fork();
        if(cid == 0){
            //setup our socket based on port
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
            bcopy((char *)server->h_addr, 
                (char *)&serv_addr.sin_addr.s_addr,
                server->h_length);
            serv_addr.sin_port = htons(portno);
            if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
                error("ERROR connecting");

            printf("Please enter the message: ");
            bzero(buffer,256);
            //get a message from stdin into our buffer
            fgets(buffer,255,stdin);
            //write to the server, child does the following

            n = write(sockfd,buffer,strlen(buffer));
            if (n < 0) 
                error("ERROR writing to socket");
            bzero(buffer,256);
            //read incoming info from the server
            n = read(sockfd,buffer,255);
            if (n < 0) 
                error("ERROR reading from socket");
            printf("%s\n",buffer);
        }
        else{
            //wait until child is done
            pid_t cpid = wait(&status);
        }
    }

    //close the server socket
    close(sockfd);
    return 0;
}