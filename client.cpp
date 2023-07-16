#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <cmath>
#include <chrono>
#include <iostream>

using namespace std::chrono;

#define PORT 8081
#define XX 1000
//the bigger the number of the messages in the window the less
//effect of the ack has propotionaly to the total.

int main(int argc, char const* argv[]) {
    int status, client_fd;
    struct sockaddr_in serv_addr;
    char *msg= new char[1024*1024];
    char buffer[1024] = {0};
    double Throughputs[21];

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        std::cout<<"\n Socket creation error \n";
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    // Convert IPv4 and IPv6 addresses from text to binary
    if (inet_pton(AF_INET, "132.72.18.187", &serv_addr.sin_addr) <= 0) {
        std::cout<<"\nInvalid address/ Address not supported \n";
        return -1;
    }

    status = connect(client_fd, (struct sockaddr *) &serv_addr,sizeof(serv_addr));
    if (status  < 0) {
        std::cout<<"\nConnection Failed \n";
        return -1;
    }
    std::cout<<"connected\n";

    int count = 0 ;
    int size;
    while(count <= 20){
        size = pow(2, count);
        memset(msg, '*', size);
        auto start = high_resolution_clock::now();
        for(int i=0 ; i<XX ; i++){
            if (i == XX-1){
 		msg[size-1] =  'F'; //FINISH
	    }
            send(client_fd, msg, strlen(msg), 0);
        }
        read(client_fd, buffer, 1024);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        int temp = size * XX;
        Throughputs[count]= temp / (double)(duration.count());
        std::cout<<"Troughput["<<count<<"]= "<<Throughputs[count]<<"\n";
        count++;
    }
    std::cout<<"RESULTS:\n";
    for (int i = 0; i<count; i++){
        std::cout<<Throughputs[i]<<", ";
    }
    std::cout<<"\n";
    return 0;
}
