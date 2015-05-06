#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "logger.h"


#define BUFSIZE 1024
#define PORT_NO 2001

typedef struct player_struct
{
    int socket;
    char name[50];
    int number;
    int win;
} PLAYER;

void errormsg(char *);
int answer(PLAYER, PLAYER, int);
void game_is_start(int, PLAYER, PLAYER);
void winner(PLAYER * , PLAYER * );
void giveup(PLAYER * , PLAYER * );
void imposture(PLAYER * , PLAYER * );

int main(int argc, char *argv[]) {
    int s;
    PLAYER player1;
    PLAYER player2;
    player1.win = 0;
    player2.win = 0;
    int t;
    int flags = 0;

    if(argc != 3) {
        printf("Usage: ./haloserver jatszmaszam maximum_szam");
        exit(1);
    }

    int plays = atoi(argv[1]);
    int maximum = atoi(argv[2]);

    LOG_PRINT("Server started with %d plays and the maximum number is %d", plays, maximum);

    struct sockaddr_in server;
    struct sockaddr_in client;

    int server_size = sizeof server;
    int client_size = sizeof client;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT_NO);
    char on = 1;
    char buffer[BUFSIZE+1];
    int bytes = BUFSIZE;
    int err;
    int recv_size;
    int trnmsize;

    s = socket(AF_INET, SOCK_STREAM, 0);

    if(s < 0) {
        LOG_PRINT("Socket can not be created.");
        errormsg("Socket can not be created.");
    } else {
        LOG_PRINT("Socket is created.");
    }

    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

    err = bind(s, (struct sockaddr *) &server, server_size);

    if(err < 0) {
        LOG_PRINT("Cannot bind to the socket.");
        errormsg("Cannot bind to the socket.");
    }
    else {
        LOG_PRINT("Binded to the socket");
    }

    err = listen(s, 2);
    if(err < 0) {
        LOG_PRINT("Cannot listen to the socket.");
        errormsg("Cannot listen to the socket.");
    }
    else
    {
        LOG_PRINT("Listening to the socket.");
    }

    player1.socket = accept(s, (struct sockaddr *)&client, &client_size);
    if(player1.socket < 0) {
        LOG_PRINT("Cannot accept on socket.");
        errormsg("Cannot accept on socket");
    }
    else
    {
        LOG_PRINT("Accept on socket");
    }


    recv_size = recv(player1.socket, buffer, bytes, flags);
    if(recv_size < 0)
    {
        errormsg("Cannot receive from the socket");
        LOG_PRINT("Cannot receive from the socket");
    }
    else
    {
        LOG_PRINT("Received from the socket.");
    }

    sscanf(buffer, "%s", player1.name);

    //printf("%i bytes have been received from client.\n Message: %s \n", recv_size-1, buffer);
    sprintf(buffer,"Hello %s! \nConnection is created. Waiting for the other player.",
            player1.name);
    bytes = strlen(buffer) + 1;
    trnmsize = send(player1.socket, buffer, bytes, flags);
    if (trnmsize < 0) {
        errormsg("Cannot send data to the client.");
        LOG_PRINT("Cannot send data to the client.");
    }
    else
    {
        LOG_PRINT("Acknowledgement has been sent to the first client");
    }




    player2.socket = accept(s, (struct sockaddr *)&client, &client_size);
    if(player2.socket < 0) {
        LOG_PRINT("Cannot accept on socket.");
        errormsg("Cannot accept on socket");
    }
    else
    {
        LOG_PRINT("Accept on socket");
    }


    recv_size = recv(player2.socket, buffer, bytes, flags);
    if(recv_size < 0)
    {
        errormsg("Cannot receive from the socket");
        LOG_PRINT("Cannot receive from the socket");
    }
    else
    {
        LOG_PRINT("Received from the socket.");
    }

    sscanf(buffer, "%s", player2.name);
    //printf("%i bytes have been received from client.\n Message: %s \n", recv_size-1, buffer);
    sprintf(buffer,"Hello %s!\n Connection is created.", player2.name);
    bytes = strlen(buffer) + 1;
    trnmsize = send(player2.socket, buffer, bytes, flags);
    if (trnmsize < 0) {
        errormsg("Cannot send data to the client.");
        LOG_PRINT("Cannot send data to the client.");
        exit(6);
    }
    else
    {
        LOG_PRINT("Acknowledgement has been sent to the second client");
    }

    int i;
    for(i = 0; i < plays; i++) {
    	
    	sleep(1);
        sprintf(buffer,"Set up before %d play.", i+1);
        bytes = strlen(buffer) + 1;
        trnmsize = send(player1.socket, buffer, bytes, 0);
        trnmsize = send(player2.socket, buffer, bytes, 0);
        if (trnmsize < 0) {
            errormsg("Cannot send data to the client.");
            LOG_PRINT("Cannot send data to the client.");
        }

        sleep(1);

        sprintf(buffer,"Maximum");
        bytes = strlen(buffer) + 1;
        trnmsize = send(player1.socket, buffer, bytes, 0);
        if (trnmsize < 0) {
            errormsg("Cannot send data to the client.");
            LOG_PRINT("Cannot send data to the client.");
        }

        sleep(1);

        sprintf(buffer,"%d", maximum);
        bytes = strlen(buffer) + 1;
        trnmsize = send(player1.socket, buffer, bytes, 0);
        if (trnmsize < 0) {
            errormsg("Cannot send data to the client.");
            LOG_PRINT("Cannot send data to the client.");
        }

        recv_size = recv(player1.socket, buffer, bytes, flags);

        if(recv_size < 0)
        {
            errormsg("Cannot receive from the socket");
            LOG_PRINT("Cannot receive from the socket");
        }
        else
        {
            LOG_PRINT("Received from the socket.");
        }

        sscanf(buffer, "%d", &player1.number);

        sprintf(buffer,"Maximum");
        bytes = strlen(buffer) + 1;
        trnmsize = send(player2.socket, buffer, bytes, 0);
        if (trnmsize < 0) {
            errormsg("Cannot send data to the client.");
            LOG_PRINT("Cannot send data to the client.");
        }

		sleep(1);

        sprintf(buffer,"%d", maximum);
        bytes = strlen(buffer) + 1;
        trnmsize = send(player2.socket, buffer, bytes, 0);
        if (trnmsize < 0) {
            errormsg("Cannot send data to the client.");
            LOG_PRINT("Cannot send data to the client.");
        }

        recv_size = recv(player2.socket, buffer, bytes, flags);

        if(recv_size < 0)
        {
            errormsg("Cannot receive from the socket");
            LOG_PRINT("Cannot receive from the socket");
        }
        else
        {
            LOG_PRINT("Received from the socket.");
        }

        sscanf(buffer, "%d", &player2.number);


        game_is_start(i, player1, player2);

        sprintf(buffer,"Give a number: ");
        bytes = strlen(buffer) + 1;
        trnmsize = send(player1.socket, buffer, bytes, 0);
        if (trnmsize < 0) {
            errormsg("Cannot send data to the client.");
            LOG_PRINT("Cannot send data to the client.");
        }

        bzero((char *) &buffer, BUFSIZE);
    	recv_size = recv(player1.socket, buffer, bytes, 0);
    	if(recv_size < 0)
    	{
        	errormsg("Cannot receive from the socket");
        	LOG_PRINT("Cannot receive from the socket");
    	}
    	else
    	{
        	LOG_PRINT("Received from the socket.");
    	}
    	sscanf(buffer, "%d", &t);

        while(1) {
            t = answer(player2, player1, t);
            printf("%d\n", t);
            if(t == 0){
            	winner(&player1, &player2);
            	break;
            } else if(t == -1){
            	giveup(&player2, &player1);
            	break;
            }else if(t == -2){
            	imposture(&player2, &player1);
            	break;
            }
            
            printf("%d\n", t);
            t = answer(player1, player2, t);
            if(t == 0){
            	winner(&player2, &player1);
            	break;
            } else if(t == -1){
            	giveup(&player1, &player2);
            	break;
            } else if(t == -2){
            	imposture(&player1, &player2);
            	break;	
            }
        }
    }

    sleep(1);
    sprintf(buffer,"end");
    bytes = strlen(buffer) + 1;
    trnmsize = send(player1.socket, buffer, bytes, 0);
    trnmsize = send(player2.socket, buffer, bytes, 0);
    if (trnmsize < 0) {
        errormsg("Cannot send data to the client.");
        LOG_PRINT("Cannot send data to the client.");
    }

    close(player1.socket);
    close(player2.socket);
    close(s);
    exit(0);
}

void errormsg(char * msg) {
    printf("%s\n", msg);
    exit(1);
}

int answer(PLAYER player, PLAYER second, int n) {

    char buffer[BUFSIZE+1];
    int bytes = BUFSIZE;
    int recv_size;
    int trnmsize;

    sprintf(buffer,"Your turn!");
    bytes = strlen(buffer) + 1;
    trnmsize = send(player.socket, buffer, bytes, 0);
    if (trnmsize < 0) {
        errormsg("Cannot send data to the client.");
        LOG_PRINT("Cannot send data to the client.");
    }

	sleep(1);
    sprintf(buffer,"The %s say: %d. Reply: ", second.name, n);
    bytes = strlen(buffer) + 1;
    trnmsize = send(player.socket, buffer, bytes, 0);
    if (trnmsize < 0) {
        errormsg("Cannot send data to the client.");
        LOG_PRINT("Cannot send data to the client.");
    }


    bzero((char *) &buffer, BUFSIZE);
    recv_size = recv(player.socket, buffer, bytes, 0);
    if(recv_size < 0)
    {
        errormsg("Cannot receive from the socket");
        LOG_PRINT("Cannot receive from the socket");
    }
    else
    {
        LOG_PRINT("Received from the socket.");
    }
    char temp[BUFSIZE];

    sprintf(temp, "The enemy's number is: %s", buffer);
    bytes = sizeof(temp);
    trnmsize = send(second.socket, temp, bytes, 0);
    if (trnmsize < 0) {
        errormsg("Cannot send data to the client.");
        LOG_PRINT("Cannot send data to the client.");
    }
    
    if(!strcmp(buffer, "kisebb")){
    	if(player.number  >= n){
    		sprintf(buffer, "%s is n impostor!", player.name);
    		LOG_PRINT(buffer);		
    		return -2;
    	}
    }else if(!strcmp(buffer, "nagyobb")){
    	if(player.number <= n){
    		sprintf(buffer, "%s is n impostor!", player.name);
    		LOG_PRINT(buffer);
    		return -2;
    	}
    }else if(!strcmp(buffer, "talált")){
    	if(player.number != n){
    		sprintf(buffer, "%s is n impostor!", player.name);
    		LOG_PRINT(buffer);
    		return -2;
    	}
    	return 0;
    }else if(!strcmp(buffer, "feladom")){
    	return -1;
    }


    sprintf(buffer,"Give a number: ");
    bytes = strlen(buffer) + 1;
    trnmsize = send(player.socket, buffer, bytes, 0);
    if (trnmsize < 0) {
        errormsg("Cannot send data to the client.");
        LOG_PRINT("Cannot send data to the client.");
    }

    bzero((char *) &buffer, BUFSIZE);
    recv_size = recv(player.socket, buffer, bytes, 0);
    if(recv_size < 0)
    {
        errormsg("Cannot receive from the socket");
        LOG_PRINT("Cannot receive from the socket");
    }
    else
    {
        LOG_PRINT("Received from the socket.");
    }
    sscanf(buffer, "%d", &n);
    
    return n;
}

void game_is_start(int play, PLAYER player1, PLAYER player2) {
    int i = 0;
    char buffer[BUFSIZE+1];
    int bytes = BUFSIZE;
    int recv_size;
    int trnmsize;

    sprintf(buffer,"%d. game is beginning", play+1);
    bytes = strlen(buffer) + 1;
    trnmsize = send(player1.socket, buffer, bytes, 0);
    trnmsize = send(player2.socket, buffer, bytes, 0);
    if (trnmsize < 0) {
        errormsg("Cannot send data to the client.");
        LOG_PRINT("Cannot send data to the client.");
    }

    sleep(1);

    for(i = 3 ; i >= 1; i--) {
        sprintf(buffer,"%d", i);
        bytes = strlen(buffer) + 1;
        trnmsize = send(player1.socket, buffer, bytes, 0);
        trnmsize = send(player2.socket, buffer, bytes, 0);
        if (trnmsize < 0) {
            errormsg("Cannot send data to the client.");
            LOG_PRINT("Cannot send data to the client.");
        }

        sleep(1);
    }

}
   	
   	void winner(PLAYER * winner_p, PLAYER * looser){
    	
    	char buffer[BUFSIZE+1];
   		int bytes = BUFSIZE;
    	int recv_size;
    	int trnmsize;

    	winner_p->win += 1;
    	sprintf(buffer,"%s won the play.\n%s wins: %d, %s wins: %d\n", winner_p->name,
    					winner_p->name, winner_p->win, looser->name, looser->win);
        bytes = strlen(buffer) + 1;
        trnmsize = send(winner_p->socket, buffer, bytes, 0);
        trnmsize = send(looser->socket, buffer, bytes, 0);
        if (trnmsize < 0) {
            errormsg("Cannot send data to the client.");
            LOG_PRINT("Cannot send data to the client.");
        }
    }

    void imposture(PLAYER * impostor, PLAYER * other){
    	char buffer[BUFSIZE+1];
   		int bytes = BUFSIZE;
    	int trnmsize;

    	sprintf(buffer,"%s is an impostor!", impostor->name);
        bytes = strlen(buffer) + 1;
        trnmsize = send(impostor->socket, buffer, bytes, 0);
        trnmsize = send(other->socket, buffer, bytes, 0);
        if (trnmsize < 0) {
            errormsg("Cannot send data to the client.");
            LOG_PRINT("Cannot send data to the client.");
        }
        sleep(1);
    	winner(other, impostor);
    }

    void giveup(PLAYER * rabbit, PLAYER * other){
    	char buffer[BUFSIZE+1];
   		int bytes = BUFSIZE;
    	int trnmsize;

    	sprintf(buffer,"%s give up the play!", rabbit->name);
        bytes = strlen(buffer) + 1;
        trnmsize = send(rabbit->socket, buffer, bytes, 0);
        trnmsize = send(other->socket, buffer, bytes, 0);
        if (trnmsize < 0) {
            errormsg("Cannot send data to the client.");
            LOG_PRINT("Cannot send data to the client.");
        }
        sleep(1);
    	winner(other, rabbit);
    }
