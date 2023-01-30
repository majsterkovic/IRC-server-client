#include "Client.h"
#include "Logger.h"

/**
 * Construct a new Client:: Client object. Accepts a connection from a client.
 * 
 * @param server_descriptor The server descriptor.
 */
Client::Client(int server_descriptor)
{
    this->descriptor = accept(server_descriptor, nullptr, nullptr);
    if (descriptor < 0)
    {
        std::string message = "Nie udało się zaakceptować połączenia\n";
        perror(message.c_str());
        Logger::log('E', message);
        exit(EXIT_FAILURE);
    }
}

/**
 * Destroy the Client:: Client object. Closes the connection with the client.
 * 
 */
Client::~Client()
{
    close(descriptor);
}

