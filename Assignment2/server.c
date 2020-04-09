// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 80
int main(int argc, char const *argv[]) 
{ 


    //assuming here is executing as a root or admin
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server1"; 
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    printf("This is the correct server_fd: %d \n", server_fd);
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 



    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 

    pid_t pid = fork();
    if(pid < 0)
    {
    	perror("Fork Failed \n");
    }

    if(pid == 0)
    {
	    if (listen(server_fd, 3) < 0) 
	    { 
	        perror("listen \n"); 
	        exit(EXIT_FAILURE); 
	    }
	    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
	                       (socklen_t*)&addrlen))<0) 
	    { 
	        perror("accept"); 
	        exit(EXIT_FAILURE); 
	    }
    	setuid(-1); // set to nobody
	    printf("The user is set to nobody(99) \n");

	    valread = read( new_socket , buffer, 1024); 
	    printf("%s\n",buffer ); 
	    send(new_socket , hello , strlen(hello) , 0 ); 
	    printf("Hello message sent\n"); 

	    printf("Starting exec..\n");
  		char str_server_fd[10];
  		sprintf(str_server_fd, "%d", server_fd);
	    char *args[] = {"./child_process", str_server_fd, NULL};
	    printf("This is args[1]: %s", args[1]);
	    execl(args[0], args[1], NULL);

	    printf("Child is existing...\n");
	    exit(0);
    }
    wait(NULL);
    printf("Parent is ending...\n");
    
    return 0; 
} 
