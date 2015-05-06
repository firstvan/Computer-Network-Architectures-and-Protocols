#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 1024
#define PORT_NO 2001
#define error(a,b) fprintf(stderr, a, b)

int main(int argc, char *argv[] ) {// arg count, arg vector

    /* Declarations */
    int fd;	                       // socket endpt
    int flags;                      // rcv flags
    struct sockaddr_in server;	     // socket name (addr) of server
    struct sockaddr_in client;	     // socket name of client
    int server_size;                // length of the socket addr. server
    int client_size;                // length of the socket addr. client
    int bytes;    	                 // length of buffer
    int rcvsize;                    // received bytes
    int trnmsize;                   // transmitted bytes
    int err;                        // error code
    int ip;												 // ip address
    char on;                        //
    char buffer[BUFSIZE+1];         // datagram dat buffer area
    char server_addr[16];           // server address

    /* Initialization */
    on    = 1;
    flags = 0;
    server_size = sizeof server;
    client_size = sizeof client;
    sprintf(server_addr, "%s", argv[1]);
    ip = inet_addr(server_addr);
    server.sin_family      = AF_INET;
    server.sin_addr.s_addr = ip;
    server.sin_port        = htons(PORT_NO);

    /* Creating socket */
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        error("%s: Socket creation error.\n",argv[0]);
        exit(1);
    }

    /* Setting socket options */
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

    /* Connecting to the server */
    err = connect(fd, (struct sockaddr *) &server, server_size);
    if (err < 0) {
        error("%s: Cannot connect to the server.\n", argv[0]);
        exit(2);
    }

    printf("Give me the player name: \n");
    fgets(buffer, BUFSIZE, stdin);
    bytes = strlen(buffer)+1;

    /* Sending data to the server */
    trnmsize = send(fd, buffer, bytes, flags);
    if (trnmsize < 0) {
        error("%s: Cannot send data to server.\n", argv[0]);
        exit(3);
    }
    int max = 0;

    while(1) {
        bzero((char *) &buffer, BUFSIZE);
        rcvsize = recv( fd, buffer, BUFSIZE, flags );
        if (rcvsize < 0) {
            error("%s: Cannot receive data from the socket.\n", argv[0]);
            exit(4);
        }


        if(!strcmp(buffer, "Maximum")) {
            
            bzero((char *) &buffer, BUFSIZE);
            rcvsize = recv( fd, buffer, BUFSIZE, flags );
            sscanf(buffer, "%d", &max);

            if (rcvsize < 0) {
                error("%s: Cannot receive data from the socket.\n", argv[0]);
                exit(4);
            }
            
            int think;
            printf("Give a number between 1 and %d: ", max);
            scanf("%d", &think);
            while(think<1 || think > max) {
                printf("Wrong number! Give a number between 1 and %d: ", max);
                scanf("%d", &think);
            }
            printf("Waiting for the other player to give a number\n");
            sprintf(buffer,"%d", think);
            bytes = strlen(buffer) + 1;
            trnmsize = send(fd, buffer, bytes, 0);
            if (trnmsize < 0) {
                error("%s: Cannot send data to server.\n", argv[0]);
            }

            //TODO
        }
        else if (!strcmp(buffer, "Your turn!")) {

                printf("%s\n", buffer);
                bzero((char *) &buffer, BUFSIZE);
                rcvsize = recv( fd, buffer, BUFSIZE, flags );
                printf("%s", buffer);
                bzero((char *) &buffer, BUFSIZE);
                scanf("%s", buffer);

                while (!(!strcmp(buffer, "talált") || !strcmp(buffer, "kisebb") 
                     || !strcmp(buffer, "nagyobb") || !strcmp(buffer, "feladom"))){
                  printf("Wrong input! (kisebb, nagyobb, talált, feladom) :");
                  scanf("%s", buffer);
                }

                bytes = strlen(buffer)+1;

                /* Sending data to the server */
                trnmsize = send(fd, buffer, bytes, flags);
                if (trnmsize < 0) {
                    error("%s: Cannot send data to server.\n", argv[0]);
                    exit(3);
                }

                bzero((char *) &buffer, BUFSIZE);
                rcvsize = recv( fd, buffer, BUFSIZE, flags );
                printf("%s", buffer);
                if(!strcmp(buffer,"Give a number: ")){

                bzero((char *) &buffer, BUFSIZE);
                scanf("%s", buffer);
                bytes = strlen(buffer)+1;

                /* Sending data to the server */
                trnmsize = send(fd, buffer, bytes, flags);
                if (trnmsize < 0) {
                    error("%s: Cannot send data to server.\n", argv[0]);
                    exit(3);
                }
             }


            }
            else if (!strcmp(buffer, "Give a number: ")) {
                printf("%s ", buffer);
                bzero((char *) &buffer, BUFSIZE);
                scanf("%s",buffer);
                bytes = strlen(buffer)+1;

                /* Sending data to the server */
                trnmsize = send(fd, buffer, bytes, flags);
                if (trnmsize < 0) {
                    error("%s: Cannot send data to server.\n", argv[0]);
                    exit(3);
                }
            }else if(!strcmp(buffer, "end")){
               break;
            }
            else {
                printf("%s\n", buffer);
            }
    }


    close(fd);
    exit(0);
}
