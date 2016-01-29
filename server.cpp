#include <iostream>
#include <cstring>      // Needed for memset
#include <sys/socket.h> // Needed for the socket functions
#include <netdb.h>      // Needed for the socket functions
#include "pthread.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <spawn.h>
#include <sys/wait.h>

using namespace std;

void *start_posix_spawn(void* threadid);
pid_t pid, pid_happy, pid_sad, pid_open;

int main()
{

    pthread_t thread;
    int rc;

    int status;
    struct addrinfo host_info;       // The struct that getaddrinfo() fills up with data.
    struct addrinfo *host_info_list; // Pointer to the to the linked list of host_info's.

    // The MAN page of getaddrinfo() states "All  the other fields in the structure pointed
    // to by hints must contain either 0 or a null pointer, as appropriate." When a struct
    // is created in c++, it will be given a block of memory. This memory is not nessesary
    // empty. Therefor we use the memset function to make sure all fields are NULL.
    memset(&host_info, 0, sizeof host_info);

    //std::cout << "Setting up the structs..."  << std::endl;

    host_info.ai_family = AF_UNSPEC;     // IP version not specified. Can be both.
    host_info.ai_socktype = SOCK_STREAM; // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.
    host_info.ai_flags = AI_PASSIVE;     // IP Wildcard

    // Now fill up the linked list of host_info structs with google's address information.
    status = getaddrinfo("127.0.0.1", "8880", &host_info, &host_info_list);
    // getaddrinfo returns 0 on succes, or some other value when an error occured.
    // (translated into human readable text by the gai_gai_strerror function).
    if (status != 0)  std::cout << "getaddrinfo error" << gai_strerror(status) ;


    //std::cout << "Creating a socket..."  << std::endl;
    int socketfd ; // The socket descripter
    socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype,
                      host_info_list->ai_protocol);
		std::cout << "socketfd "  << socketfd <<std::endl;
    if (socketfd == -1)  std::cout << "socket error " ;

    //std::cout << "Binding socket..."  << std::endl;
    // we use to make the setsockopt() function to make sure the port is not in use
    // by a previous execution of our code. (see man page for more information)
    int yes = 1;
    status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    status = bind(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1)  std::cout << "bind error" << std::endl ;

    std::cout << "Listening..."  << std::endl;
    status =  listen(socketfd, 5);
    if (status == -1)  std::cout << "listen error" << std::endl ;


    int new_sd;
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(their_addr);
do{
    new_sd = accept(socketfd, (struct sockaddr *)&their_addr, &addr_size);
    if (new_sd == -1)
    {
        std::cout << "listen error" << std::endl ;
    }
    else
    {
        std::cout << "Connection accepted. Using new socketfd : "  <<  new_sd << std::endl;
    }


    //std::cout << "Waiting to recieve data..."  << std::endl;
    ssize_t bytes_recieved;
    char message[100];
   	
    bytes_recieved = read(new_sd, message,sizeof(message));
	//std::cout<<bytes_recieved<<std::endl;

    // If no data arrives, the program will just wait here until some data arrives.
    if (bytes_recieved == 0) std::cout << "host shut down." << std::endl ;
    if (bytes_recieved == -1)std::cout << "recieve error!" << std::endl ;
    std::cout << bytes_recieved << " bytes recieved :" << std::endl ;
    message[bytes_recieved] = '\0';
    std::cout << message << std::endl;
  
    if (message!=0) {
        cout<<"eimai ston elegxo message!=0"<<endl;
        cout<<"pid "<<pid<<endl;
        
        if(pid!=0){
    	int s=kill(pid,9);}
        
    rc=pthread_create(&thread,NULL,start_posix_spawn,(void*) message);
    }
   
    
 
   
}while(true);

    //std::cout << "Stopping server..." << std::endl;
    //freeaddrinfo(host_info_list);
    //close(new_sd);
    //close(socketfd);

return 0 ;
}

void *start_posix_spawn(void* threadid) {
  std::cout<<"Process starts"<<std::endl;
  
 char * exe=(char *)"mpg_main";
 char * orisma=(char *)threadid;

  char *argv[] = {exe, orisma, (char *) 0};
  int status;
  puts("Testing start_posix_spawn");
  fflush(NULL);
  status = posix_spawn(&pid, "/home/mixa/mpg_main", NULL, NULL, argv, environ);
  
   if (status == 0) {
      /*if (orisma=="1"){
         pid_happy=pid;}
      else if (orisma=="2"){
         pid_sad=pid;}
      else if (orisma=="3"){
         pid_open=pid;}
    printf("Happy id: %i\n", pid_happy);
    printf("Sad id: %i\n", pid_sad);
    printf("Open id: %i\n", pid_sad);*/
    printf("Child id: %i\n", pid);
    fflush(NULL);
    if (waitpid(pid, &status, 0) != -1) {
      printf("Child exited with status %i\n", status);
    } else {
      perror("waitpid");
    }
  } else {
    printf("posix_spawn: %s\n", strerror(status));
  }
}

