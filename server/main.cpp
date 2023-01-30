#include <cstdlib>

#include "Server.h"

/**
 * Main function for the server. Starts the server and handles connections.
 *
 *
 * @param argc The number of arguments in `argv`.
 * @param argv The arguments passed to the program - the first is the port number.
 * @return 0 if the program exits successfully.
 */
int main([[maybe_unused]] int argc, char **argv)
{

    int port = 1234;
    if (argc > 1)
    {
        port = (int) strtol(argv[1], nullptr, 10);
    }

    Server server(port);

    while (true)
    {
        server.handle_connection();
    }

    return 0;
}