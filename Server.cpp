#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>
#include <sstream>
#include "Knn.h"
#include "Server.h"


using namespace std;


Server::Server(int port) throw(const char*) {
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
/**
 * checking arguments.
 * @param k - should be a number > 0.
 * @param dis - should be one of the distance we got in knn.
 */
int checkingArg(int k, string dis, int &flag) {
    if((k<=0) ||
       ((dis != "AUC") && (dis != "MAN") && (dis != "CHB")
        && (dis != "CAN") && (dis != "MIN"))) {
       flag = -1;
        return flag;
    }
    return flag;
}

/**
 * checking buffer from client.
 * @param str - the string we got in buffer from client.
 * @param distance - MAN/AUC/CHB/MIN/CAN.
 * @param v1 - vector for calculating distance.
 * @param k - the k neighbors.
 */
int checkingStr(string str,string &distance, vector<double> &v1, int &k, int &flag) {
    k = -1;
    v1.clear();
    double num;
    int checkInt;
    stringstream ss;
    string checkStr;
    stringstream stringstream1(str);
    while (stringstream1 >> num) {
        v1.push_back(num);
    }
    if (v1.empty()) {
        flag = -1;
        return flag;
    }
    stringstream1.clear();
    if(stringstream1 >> checkStr) {
        ss << checkStr;
    }
    distance = ss.str();
    if (str.empty()) {
        flag = -1;
        return flag;
    }
    stringstream1.clear();
    if (stringstream1 >> checkInt) {
        k = checkInt;
    }
    if (k == -1) {
        flag = -1;
        return flag;
    }
    if(!stringstream1.eof()) {
        flag = -1;
        return flag;
    }
    int res = checkingArg(k, distance,flag);
    return res;
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
    string message;
    vector<double> vector;
    string distance;
    int neighbor;
    int res;
    string file = argv[1];
    const int server_port = atoi(argv[2]);
    checkingArgv(server_port);
    Server server(server_port);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cout << "error creating socket" << endl;
        exit(1);
    }
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(server_port);
    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        cout << "error binding socket" << endl;
        exit(1);
    }
    if (listen(sock, 5) < 0) {
        cout << "error listening to a socket" << endl;
        exit(1);
    }
    struct sockaddr client_sin;
    unsigned int addr_len = sizeof(client_sin);
    while (true) {
        int client_sock = accept(sock, (struct sockaddr *) &client_sin, &addr_len);
        if (client_sock < 0) {
            cout << "error accepting client" << endl;
            break;
        }
        //dana add
        clientHandler ch;
        ch.handle(client_sock);

        while (true) {
            int flag = 0;
            char buffer[4096] = {0};
            int expected_data_len = sizeof(buffer);
            int read_bytes = recv(client_sock, buffer, expected_data_len, 0);
            if (read_bytes == 0) {
                cout << "no message from client" << endl;
                break;
            }
            if (read_bytes < 0) {
                cout << "error getting a message from client" << endl;
                break;
            }
            string data = buffer;
            res = checkingStr(data, distance, vector, neighbor, flag);
            //in case we found invalid input
            if (res == -1) {
                message = "invalid input";
                int length = message.length();
                char invalid_message[length + 1];
                strcpy(invalid_message, message.c_str());
                int send_bytes = send(client_sock, invalid_message, length, 0);
                if (send_bytes < 0) {
                    cout << "error sending a message" << endl;
                }
            } else {
                //if the input is valid
                Knn *knn = new Knn(neighbor, distance, vector);
                knn->uploadFiles(file, flag);
                //check if flag is -1 - then something went wrong in knn class
                if (flag == -1) {
                    message = "invalid input";
                } else {
                    message = knn->getMessage();
                }
                int length = message.length();
                char message_to_send[length + 1];
                strcpy(message_to_send, message.c_str());
                int send_bytes = send(client_sock, message_to_send, length, 0);
                if (send_bytes < 0) {
                    cout << "error sending a message" << endl;
                }
                delete knn;
            }
        }
    }
}
