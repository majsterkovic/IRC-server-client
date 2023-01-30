//
// Created by Mariusz on 21.11.2022.
//

#ifndef UNTITLED_CLIENT_H
#define UNTITLED_CLIENT_H

#include <string>
#include <sys/socket.h>
#include <unistd.h>

class Client
{
public:
    int descriptor;
    char action{};
    int room{};
    std::string name;

    explicit Client(int server_descriptor);

    ~Client();
};

#endif //UNTITLED_CLIENT_H
