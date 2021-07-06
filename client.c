#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>


int main(){
    int sockfd;
    int len;
    int result;
    struct sockaddr_un address;
    char ch;
    char resposta[1000] = "";


    while(1){ 
        printf( "Faca sua tentativa: \n" );
        scanf( " %c", &ch);

        sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
        address.sun_family = AF_UNIX;
        strcpy(address.sun_path, "server_socket18");
        len = sizeof(address);
        result = connect(sockfd, (struct sockaddr *)&address, len);
        if(result == -1){
            perror("oops: client1");
            exit(1);
        }
        write(sockfd, &ch, 1);
        read(sockfd, &resposta, 1000);
        printf("%s\n\n", resposta);
        close(sockfd);

        if(resposta[0] == '*'){
            return 0;
        }
    }
    exit(0);
}