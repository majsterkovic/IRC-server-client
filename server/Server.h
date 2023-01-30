// TODO: Nazewnictwo funkcji https://google.github.io/styleguide/cppguide.html#Function_Names
// TODO: Nazewnictwo zmiennych (podkreślenie) https://google.github.io/styleguide/cppguide.html#Variable_Names


#ifndef UNTITLED_SERVER_H
#define UNTITLED_SERVER_H

#include "Client.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <netinet/in.h>
#include <vector>
#include <iostream>
#include <thread>
#include <pthread.h>
#include <cstdlib>
#include <cstdio>
#include <mutex>
#include <chrono>
#include <stdio.h>


#define QUEUE_SIZE 8

/**
 * Server class
 * 
 */
class Server
{

public:
    void handle_connection();

    explicit Server(int port);

private:
    int descriptor;
    std::vector<Client *> clients;

    int reuse_addr_value = 1;
    sockaddr_in saddr{};

    std::mutex room_mutex[9];
    std::mutex client_mutex;

    static std::string get_current_time();

    [[maybe_unused]] static void flush_message(int client_descriptor);

    static std::string get_message(int client_descriptor);

    void send_message_to_room(std::string &message, int room, std::string &name);

    static void login_new_user(Client *c);

    void send_message_about_entering_room(int room_id, std::string &name);

    void send_message_about_leaving_room(int room_id, std::string &name);

    void change_room(Client *client);

    void quit_room(Client *client);

    void remove_client(Client *client);

    void *my_thread(Client *c);
};

#endif //UNTITLED_SERVER_H