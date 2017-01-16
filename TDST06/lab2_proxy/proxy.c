#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>

// ============================================================================
// ============================== CONFIGURATION ===============================
// ============================================================================

#define URL_FILTERING 1
#define CONTENT_FILTERING 1

// Here, add all the restricted URLs
char* RESTRICTED_URL[] = {
	"http://www.example.com/",
	"http://www.example.org/test/for/url",
	""
};

// Here, add all the restricted words
char* RESTRICTED_WORD[] = {
	"word1",
	"word2",
	"SpongeBob",
	""
};

// choose max content-type size of web page that can be fetched
// max is 1 MB by default
char buffer2[1000000];

// ============================================================================
// ========================= END OF CONFIGURATION =============================
// ============================================================================


#define QUEUE_LENGTH 10
#define MAXLINE 3000


void sigterm_handler(int signal);
void sigchld_handler(int signal);
void sigint_handler(int signal);

struct sockaddr_in addr_in, cli_addr, serv_addr;
struct hostent* host;
int sockfd, newsockfd;
int option = 1, clilen, PORT;
pid_t childpid;
char** ru; // used for iterating through RECTRICTED_URL
char** rw; // used for iterating through RECTRICTED_WORDS
int filteredzone = 0;

struct sockaddr_in host_addr;
int sockfd_part2, newsockfd_part2;
int flag_before_slashes=0, n, port = 0, i, flag_filter_url = 0;
int flag_filter_content = 0;
char buffer[3000],part1[300],part2[300],part3[10];
char* token = NULL;
char* token2 = NULL;


int main(int argc, char* argv[]) {

	if (argc < 2) {
		perror("Syntax: ./proxy <port_number>\n");
		exit(-1);
	}
	// obtain port number
	PORT = atoi(argv[1]);


	// set zeroes over the whole structs
	bzero((char*)&serv_addr, sizeof(serv_addr));
	bzero((char*)&cli_addr, sizeof(cli_addr));

	/* =========================================================== *
	 * ============== CONNECTION BROWSER -> PROXY ================ *
	 * =========================================================== */

    //preparation of the socket address
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
     serv_addr.sin_port = htons(PORT);

 	 if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <0) {
         perror("Problem in creating the socket");
         exit(-1);
     }

     setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

     if (bind (sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
         perror("Bind error");
         exit(-1);
     }

     //listen to the socket by creating a connection queue, then wait for clients
     if (listen (sockfd, QUEUE_LENGTH) < 0) {
         perror("Listen error");
         exit(-1);
     }

	int clilen = sizeof(cli_addr);

 	signal(SIGCHLD, sigchld_handler);
 	signal(SIGTERM, sigterm_handler);
 	signal(SIGINT, sigint_handler);

    // at this point, the socket is listening
    while(1) {
	    //accept connection
	    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	    if (newsockfd < 0) {
	    	perror("Accept error");
	    	exit(-1);
	    }

        // this is where the data client -> proxy is going to be received
        if ((childpid = fork()) == 0) {

          if (flag_filter_content == 1) {
            bzero((char*) buffer, sizeof(buffer));
            sprintf(buffer, "%s\r\n%s\r\n\r\n", \
              "HTTP/1.1 302 Found", \
              "Location: http://www.ida.liu.se/~TDTS04/labs/2011/ass2/error2.html");
            //send
            printf("\nSending: \n%s\n\n", buffer);
            if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
              perror("Send error on part 2");
            }
          }
        	close(sockfd); // will be reopened with the while loop

			// receive the data
			recv(newsockfd, buffer, MAXLINE, 0);
			// first line is formed by three strings, being:
			// part1 =  protocol,
			// part2 = address and
			// part3 = the version of HTTP
			sscanf(buffer,"%s %s %s",part1,part2,part3);

			// GET request, with either HTTP 1.0 or 1.1 and the protocol used is http (not https)
			if(((strncmp(part1,"GET",3)==0)) && ((strncmp(part3,"HTTP/1.1",8) == 0)
			|| (strncmp(part3, "HTTP/1.0",8) == 0)) && (strncmp(part2,"http://",7) == 0)) {

				/* =========================================================== *
				 * ============== CONNECTION PROXY -> WEB SERVER ============= *
				 * =========================================================== */
				strcpy(part1, part2); // dest, src // part1 = part2
				char* fullURL = strdup(part2);
				flag_before_slashes = 0;

				for (i=7; i < strlen(part2); i++) { // checks if there is a port in the address
				 	if (part2[i] == ':') {
				 		flag_before_slashes=1;
				 		break;
				 	}
   			}

    			token = strtok(part2, "//"); // returns http:

     			if (flag_before_slashes == 0) { // if no port, port is 80 (default port)
				 	port=80;
				 	token = strtok(NULL, "/"); // returns next token, up until /
			 	} else { // else, host goes until ":"
				 	token = strtok(NULL, ":");
				}

				sprintf(part2, "%s", token);
				host = gethostbyname(part2); // use host obtained earlier to obtain hostent host


				if (flag_before_slashes == 1) { // obtain port if there was a ":" sign
					token = strtok(NULL,"/"); // i.e. http://www.blabla.com:1240/
					port = atoi(token); // token = 1240 in above example
				}

				// char that can't be in normal HTTP requests to flag the end
				strcat(part1,"^]");
				token = strtok(part1, "//");
				token = strtok(NULL, "/");
				if (token != NULL) {
					token = strtok(NULL, "^]");
				}

				// preprocessor, block only works when URL_FILTERING is set to 1
				flag_filter_url = 0; // set to 0 so that next request can check too
				#if(URL_FILTERING == 1)
					ru = RESTRICTED_URL;
					while (*ru != "") {
						// create a writable copy of our char* []
						char* copy = strdup(*ru);

						// if addr is in the list, filtering goes to 1
						//printf("1: %s\n2: %s\n", copy, fullURL);
						if (strcmp(copy, fullURL) == 0) {
						 	flag_filter_url = 1;
							//printf("\nFilter URL flag is up\n");
						}
						free(copy);
						*ru++;
					}
				#endif /* URL_FILTERING == 1 */

				/* init the last part of the sockets (webserver -> proxy)*/

				bzero((char*)&host_addr,sizeof(host_addr));
				host_addr.sin_port = htons(port);
				host_addr.sin_family = AF_INET;
				bcopy((char*)host->h_addr,(char*)&host_addr.sin_addr.s_addr,host->h_length);

				//init new socket
				sockfd_part2 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

				// set socket reusable
				setsockopt(sockfd_part2, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
				// Connect
				if (newsockfd_part2 = connect(sockfd_part2,
					(struct sockaddr*)&host_addr,sizeof(struct sockaddr)) < 0) {
					perror("Connect error on part 2");
					_exit(-1);
				}

				bzero((char*)buffer,sizeof(buffer));
				if (token != NULL) { // get the location on the host if it exists
					sprintf(buffer,"GET /%s %s\r\nHost: %s\r\nConnection: close\r\n\r\n",
					token, part3, part2);
				}
				else {
					sprintf(buffer,"GET / %s\r\nHost: %s\r\nConnection: close\r\n\r\n",
					 part3, part2);
				}

				// Case where the url has been filtered
				if (flag_filter_url == 1) {
					bzero((char*) buffer, sizeof(buffer));
					sprintf(buffer, "%s\r\n%s\r\n\r\n", \
						"HTTP/1.1 302 Found", \
						"Location: http://www.ida.liu.se/~TDTS04/labs/2011/ass2/error2.html");
					// send
					//printf("\n2 - Request forwarded: %s\n", buffer);
					if (n = send(newsockfd, buffer, strlen(buffer), 0) < 0) {
						perror("Send error on part 2");
						_exit(-1);
					}
					shutdown(newsockfd_part2, 2);
					shutdown(sockfd_part2, 2);
					shutdown(newsockfd, 2);
					close(newsockfd_part2);
	     		close(sockfd_part2);
					close(newsockfd);

					_exit(0);
				}
				// If there is no url filtering to do, just send the request to the website
				// then get the response from the website
				// finally, send the info back to the client

				// case where content filtering is enabled
				// get response from web server by chunks of 3000 bytes
				// checks if word is inside
				// if it is, interupt connection and set a flag to 1
				// else, finish data transfer
				#if(CONTENT_FILTERING == 1)
				int end_of_zone = 0;
				int send_buffer2 = 0;
				int normal_behaviour = 0;
        filteredzone = 0;
					if (flag_filter_url == 0) {
						//printf("\n2 - Request forwarded: %s\n", buffer);
						n = send(sockfd_part2, buffer, strlen(buffer), 0);
						if (n < 0) {
							perror("Send error on part 2");
							_exit(-1);
						} else {
							do {
								bzero((char*)buffer, 3000);
								n = recv(sockfd_part2, buffer, 3000, 0);

								if(!(n <= 0)) {
									if ((filteredzone == 1) && (end_of_zone == 0)) {
										strcat(buffer2, buffer);
										printf("\nSize of buffer: %d\n", strlen(buffer2));
										// if new header is found, stop filtered zone
										// same if end of html document is found
										// flawed, could be done in a better way
										if((strstr(buffer, "HTTP/1.1") != NULL) || (strstr(buffer, "</html>") != NULL)) {
											end_of_zone = 1;
											send_buffer2 = 1; // raise flag for sending buffer2
											normal_behaviour = 1;
											printf("\nEND OF ZONE DETECTED\n\n");
										}
										printf("\nValue of buffer2: %s\n", buffer2);
									}
									if (strstr(buffer, "Content-Type: text/html")) {
										printf("\nFirst strstr: %s\n", buffer2);
										filteredzone = 1;
										strcpy(buffer2, buffer);
									}
									rw = RESTRICTED_WORD;
									while (*rw != "") {
										// create a writable copy of our char* []
										char* copy = strdup(*rw);
										// if text/html is detected, put buffer in new char[]
										if ((strstr(buffer, copy) != NULL) && (filteredzone == 1)) {
											printf("Blocked at: : %s\n\n", buffer);
											flag_filter_content = 1;
										}
										free(copy);
										*rw++;
									}
									if (flag_filter_content == 1) {
										// if bad word, filter page
										bzero((char*) buffer, sizeof(buffer));
									   sprintf(buffer, "%s\r\n%s\r\n\r\n", \
									     "HTTP/1.1 302 Found", \
									     "Location: http://www.ida.liu.se/~TDTS04/labs/2011/ass2/error2.html");
									   //send
									   //printf("\nSending: \n%s\n\n", buffer);
									   if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
									     perror("Send error on part 2");
									   }
									   shutdown(newsockfd_part2, 2);
									   shutdown(sockfd_part2, 2);
									   shutdown(newsockfd, 2);
									   shutdown(sockfd, 2);
									   close(newsockfd_part2);
									   close(sockfd_part2);
									   close(newsockfd);
									   close(sockfd);
									   _exit(0);
									}
									if ((flag_filter_content == 0) && (send_buffer2 == 1)) {
											// case triggered when the end of the content-type text/html
  										send(newsockfd, buffer2, strlen(buffer2), 0);
											send_buffer2 == 0;
									} else if ((flag_filter_content == 0) && (normal_behaviour == 1)
									 && (send_buffer2 == 0)) {
										// case triggered when content type data had been sent,
										// back to normal behaviour
										send(newsockfd, buffer, strlen(buffer), 0);
									} else if ((flag_filter_content == 0) && (filteredzone == 0)) {
										// case triggered when we are not in the filtered zone
										send(newsockfd, buffer, strlen(buffer), 0);
									}
								}
							} while(n > 0);
						}
					}

				// case where content filtering is disabled
				#else
					if (flag_filter_url == 0) {
						//printf("\n2 - Request forwarded: %s\n", buffer);
						n = send(sockfd_part2, buffer, strlen(buffer), 0);
						if (n < 0) {
							perror("Send error on part 2");
							_exit(-1);
						} else {
							do {
								bzero((char*)buffer, 3000);
								n = recv(sockfd_part2, buffer, 3000, 0);
								//printf("\nReceived content from web server: %s\n");
								if(!(n <= 0)) {
									//printf("\nSent content from web server: %s\n");
									send(newsockfd, buffer, n, 0);
								}
							} while (n > 0);
						}
					}
				#endif /* CONTENT_FILTERING == 1 */


    		} else {
				// if not the right format, redirect to random page
				bzero((char*) buffer, sizeof(buffer));
				sprintf(buffer, "%s\r\n%s\r\n\r\n%s", \
					"HTTP/1.1 418 I'm a teapot", \
					"Connection: close", \
					"Only HTTP GET requests are supported");
				// send
				//printf("\nSent: %s\n", buffer);
				if (n = send(newsockfd, buffer, strlen(buffer), 0) < 0) {
					perror("Send error on part 2");
					_exit(-1);
				}    		}
			shutdown(newsockfd_part2, 2);
			shutdown(sockfd_part2, 2);
			shutdown(newsockfd, 2);
			shutdown(sockfd, 2);
    	close(newsockfd_part2);
    	close(sockfd_part2);
			close(newsockfd);
			close(sockfd);
			_exit(0);
    	}
    }
    return(0);
}

// signals that take care of anoying cases such as child or term signal,
// so that the proxy is killed instantly
void sigchld_handler(int signal) {
    fflush(stdout);
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void sigterm_handler(int signal) {
    fflush(stdout);
    close(sockfd);
    close(newsockfd);
    close(sockfd_part2);
    close(newsockfd_part2);
    exit(0);
}

void sigint_handler(int signal) {
    close(sockfd);
    close(newsockfd);
    close(sockfd_part2);
    close(newsockfd_part2);
    exit(0);
}
