#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 80

int main(int argc, char const *argv[]) 
{

	int server_fd = atoi(argv[0]);
	int new_socket, valread;
	struct sockaddr_in address; 
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 

	if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    printf("Already listen............\n");
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 


    valread = read( new_socket , buffer, 1024); 
    printf("%s\n",buffer ); 
    send(new_socket , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n"); 

    // printf("Starting exec..");

    char str_server_fd[10];
	sprintf(str_server_fd, "%d", server_fd);
    char *args[] = {"./child_process", str_server_fd, NULL};
    printf("This is args[1]: %s", args[1]);
    execl(args[0], args[1], NULL);

    return 0;
}
