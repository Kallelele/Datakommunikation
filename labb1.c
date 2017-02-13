#include <sys/types.h>					/*Thís is the server code*/
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_PORT 12345				/* arbitrary, but client & server must agree */
#define BUF_SIZE 100000			/* block transfer size */
#define QUEUE_SIZE 10
#define PSIZE 1000

void fatal(char *s){
	printf("%s",s);
	exit(1);
}

void get_reqline(char* buf, char* pname) {
		int i = 5;
		int j =0;
		while(buf[i] != ' ') {
			pname[j] = buf[i];
			i++;
			j++;
		}
}

//void comp_ext(char* pname) {
	//char

int main(int argc, char *argv[]) {
	
	int s, b, l, fd, sa, bytes, on=1;
	//char buf[BUF_SIZE];					/* buffer for outgoing file */
	//char pname[PSIZE];					/* request line */
	struct sockaddr_in channel;			/* holds IP adress */
	char ok[] =   "HTTP/1.1 200 OK\r\n";
	char png[] =  "Content-Lengt: 100\r\n"
				  "Content-Type: image/png\r\n"
				  "Connection: Closed\r\n"
				  "\n";
			  
	char jpg[] =  "Content-Type: image/jpeg\r\n"
				  "\n";
	char web[] =  "Content-Type: text/html\r\n"
				  "\n";
	char gif[] =  "Content-Type: image/gif\n"
				  "Connection: close\n";
				  
	char knas[] = "HTTP/1.1 404 Not Found\r\n"
				  "Content-Length: 100\r\n"
				  "Content-Type: text/html\r\n"
				  "Connection: Closed\r\n"
				  "\n";
	char error[]= "<!DOCTYPE html>\n"
				  "<html>\n"
  				  "<body>\n"
      			  "<h1>404 Not found!</h1>\n"
            	  "</body>\n"
				  "</html>\n"
				  "\n";
				  
	char hej[] = "apple";
	/* Build address structure to bind to socket. */
	memset(&channel, 0, sizeof(channel));	/* zero channel */
	channel.sin_family = AF_INET;
	channel.sin_addr.s_addr = htonl(INADDR_ANY);
	channel.sin_port = htons(SERVER_PORT);
	
	/* Passive open. Wait for connection */
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	/* create socket */
	if(s < 0) {
		fatal("socket failed.");
		}
		
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*) &on, sizeof(on));
	
	b = bind(s, (struct sockaddr*) &channel, sizeof(channel));
	if(b < 0) {
		fatal("bind failed.");
		}
		
	l = listen(s, QUEUE_SIZE);			/* specify queue size */
	if(l < 0) {
		fatal("listen failed");
		}
	
	/* Sockets is now set up and bound. Wait for connection and process it. */
	while(1) {
		sa = accept(s, 0, 0);			/* block for connection request */
		if(sa<0) {
			fatal("accept failed");
			}	
		char buf[BUF_SIZE] = "00000";	
		char ext[5]="0000"; 
		char pname[PSIZE] = "0000";
		char fel[] = "page/notfound.html"; 
		read(sa, buf, BUF_SIZE);		/*read file name from socket */
		
		get_reqline(buf, pname);
		
		int k;
		for(k=0;k<PSIZE;k++){
			printf("%c", pname[k]);
		}
		printf("\n");
		/* Get and return the file */
		fd = open(pname, O_RDONLY);		/* open the file to be sent back */
		if(fd < 0) {
			write(sa,knas,strlen(knas));
			write(sa,error,strlen(error));
			//fatal("open failed");
			}
		int i = 0;
		int j =0;		
		if(fd>0){
		write(sa,ok,strlen(ok));
		while(pname[i]!='.'){
			i++;
		}
		while(pname[i]!='\0'){
			ext[j] = pname[i];
			i++;
			j++;
		}
		}
		
		if(strcmp(ext,".jpg")==0){
				write(sa,jpg,strlen(jpg));
				//write(sa, pname, bytes);	
			} else if(strcmp(ext,".html")==0){
				write(sa,web,strlen(web));
				//write(sa, pname, bytes);
			} else if(strcmp(ext,".png")==0){
				write(sa,png,strlen(png));
				//write(sa, pname, bytes);
			} else if(ext == ".gif"){
				write(sa,gif,strlen(gif));
				//write(sa, pname, bytes);
			}

		while(1) {
			bytes = read(fd, pname, BUF_SIZE);	/* read from file */
			printf("bytes = %d \n",bytes);
			if(bytes <= 0) {
				//if (bytes<0) { 
				//write(sa,knas,strlen(knas));
				//write(sa,fel,strlen(fel));}
				break;
				}	
			printf("%s\n",ext);
			write(sa, pname,bytes);	
							/* check for end of file */
						/*write bytes to socket */
		}
		
		close(fd);			/* close file */
		close(sa);			/* close connection */
	}
}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
