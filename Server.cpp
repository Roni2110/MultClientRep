#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Knn.h"
#include "Server.h"


using namespace std;


Server::Server(int port){
    sockNum = socket(AF_INET, SOCK_STREAM, 0);
    if (sockNum < 0) {
        cout << "error creating socket" << endl;
        exit(1);
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    if (bind(sockNum, (struct sockaddr *) &server, sizeof(server)) < 0) {
        cout << "error binding socket" << endl;
        exit(1);
    }
    if (listen(sockNum, 5) < 0) {
        cout << "error listening to a socket" << endl;
        exit(1);
    }
}

void* handle(CLI* cli){
    cli->start();
    pthread_exit(nullptr);
}

void Server:: start() {
    socklen_t addr_len = sizeof(client_sin);
    int client_sock = accept(sockNum, (struct sockaddr *) &client_sin, &addr_len);
    if (client_sock < 0) {
        cout << "error accepting client" << endl;
        exit(0);
    }

    pthread_t pthread;
    SocketIO sio(client_sock);
    CLI cli(&sio);
    pthread_create(&pthread, NULL, reinterpret_cast<void *(*)(void *)>(handle), &cli);

//    pthread_attr_t attr;
//    pthread_attr_init(&attr);
//    pthread_create(&pthread, &attr, handle, &cli);
    //pthread_join(pthread, NULL);
   // pthread_detach(pthread);
//    std::thread t(&ClientHandler::handle,clientHandler,client_sock);
}


/**
 * checking arguments.
 * @param port - port.
 * @param fileName - should be .csv.
 */
void checkingArgv(int port) {
   if(port < 1024 || port > 65535) {
       cout<< "invalid port number!"<< endl;
       exit(1);
   }
}

int main (int argc, char *argv[]) {
    const int server_port = atoi(argv[1]);
    checkingArgv(server_port);
    Server server(server_port);
    while (true) {
        server.start();
    }
}