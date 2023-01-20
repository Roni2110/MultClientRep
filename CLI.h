//
// Created by omern on 19/01/2023.
//

#ifndef MULTCLIENTREP_CLI_H
#define MULTCLIENTREP_CLI_H

#include "commands.h"

using namespace std;

class CLI {
    DeafultIO* dio;
    vector<Command*> commands;

public:
    CLI(DeafultIO* dio);
    void start();
    virtual ~CLI();
};


#endif //MULTCLIENTREP_CLI_H
