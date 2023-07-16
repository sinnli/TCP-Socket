#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#define PORT 8081
#define MB 1024*1024

using namespace std;

int main(int argc, char *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[MB] = { 0 };
    const char* ack = "Ack";
    int message_count = 0;

    // Creating socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed\n");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt\n");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,sizeof(address)) < 0) {
        perror("bind failed\n");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    cout<<"after listen()"<<endl;

    if ((new_socket = accept(server_fd, (struct sockaddr*)&address,(socklen_t*)&addrlen)) < 0) {
        perror("accept\n");
        exit(EXIT_FAILURE);
    }

    cout<<"welcome Client :)"<<endl;
    for(int i = 0 ; i < 20 ; i++){
        int notFinish = 1;
        while (notFinish){
            recv(new_socket, buffer, MB, 0);
            if (strchr(buffer, 'F')!= NULL)
                notFinish = 0;
            memset(buffer,0, sizeof(buffer));
        }

        send(new_socket, ack, strlen(ack), 0);
        cout<<"sent ack :)"<<endl;
    }

    // closing the connected socket
    close(new_socket);

    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    cout<<"after shutdown bye :)"<<endl;

    return 0;
}
