/* A simple client program to interact with the myServer.c program on the Raspberry.
myClient.c
D. Thiebaut
Adapted from http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
The port number used in 51717.
This code is compiled and run on the Macbook laptop as follows:
   
    g++ -o myClient myClient.c  
    ./myClient


*/
#include <stdio.h>
#include <sys/types.h>
#include <WinSock2.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>
#include <string.h>
#include <io.h>
#include <stdlib.h>
//#include <unistd.h>
#include <errno.h>
//#include <arpa/inet.h>


void error(char *msg) {
    perror(msg);
    exit(0);
}

void sendData( int sockfd, int x ) {
	int n;

	char buffer[32];
	sprintf( buffer, "%d\n", x );
	if ( (n = write( sockfd, buffer, strlen(buffer) ) ) < 0 )
		error( const_cast<char *>( "ERROR writing to socket") );
	buffer[n] = '\0';
}

int getData( int sockfd ) {
	char buffer[32];
	int n;

	if ( (n = read(sockfd,buffer,31) ) < 0 )
		error( const_cast<char *>( "ERROR reading from socket") );
	buffer[n] = '\0';
	return atoi( buffer );
}

int main(int argc, char *argv[])
{
    int sockfd, portno = 51717, n;
    char serverIp[] = "153.106.88.83";
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    int data;

    if (argc < 3) {
		printf( "contacting %s on port %d\n", serverIp, portno );
    }
    
    // open socket
    if ( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 )
        error( const_cast<char *>( "ERROR opening socket") );
	
	// gets server
    if ( ( server = gethostbyname( serverIp ) ) == NULL ) 
        error( const_cast<char *>("ERROR, no such host\n") );
    
    // connect to server on portno
    memset( (char *) &serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if ( connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error( const_cast<char *>( "ERROR connecting") );

	sendData(sockfd, n);	

    close( sockfd );
    return 0;
}