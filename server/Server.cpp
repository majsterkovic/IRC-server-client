#include "Server.h"
#include "Logger.h"

/**
 * Constructor for the server. Creates a socket and binds it to the given port.
 *
 * @param port The port number.
 */
Server::Server(int port)
{
    Logger::log('I', "Server started");

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(port);

    this->descriptor = socket(AF_INET, SOCK_STREAM, 0);

    if (descriptor < 0)
    {
        std::string message = "Nie udało się utworzyć gniazda serwera\n";
        Logger::log('E', message);
        perror(message.c_str());
        exit(EXIT_FAILURE);
    }
    setsockopt(descriptor, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse_addr_value, sizeof(reuse_addr_value));

    int bind_result = bind(descriptor, (struct sockaddr *) &saddr, sizeof(saddr));

    if (bind_result < 0)
    {
        std::string message = "Nie udało się powiązać gniazda serwera z adresem\n";
        Logger::log('E', message);
        perror(message.c_str());
        exit(EXIT_FAILURE);
    }

    int listen_result = listen(descriptor, QUEUE_SIZE);

    if (listen_result < 0)
    {
        std::string message = "Nie udało się ustawic gniazda serwera w tryb nasłuchiwania\n";
        Logger::log('E', message);
        perror(message.c_str());
        exit(EXIT_FAILURE);
    }
}

/**
 * Returns the current time in the format HH:MM:SS.
 *
 * @return The current time.
 */
std::string Server::get_current_time()
{
    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string my_time = std::ctime(&time);
    return my_time.substr(11, 8);
}


/**
 * Flushes the message from the client.
 *
 * @param client_descriptor The client descriptor.
 */
void Server::flush_message(int client_descriptor)
{
    int rd;
    char buf[4];
    rd = (int) read(client_descriptor, buf, 4);
    std::cout << rd << std::endl;
}


/**
 * Reads the message from the client described by descriptor and returns it.
 * 
 * @param client_descriptor The client descriptor.
 * @return The message from the client.
 */
std::string Server::get_message(int client_descriptor)
{
    int rd;
    char buffer[1024];

    int header_size = 4;
    char size[header_size];

    int n = 0;
    while (n < header_size)
    {
        rd = (int) read(client_descriptor, size + n, header_size - n);
        n = n + rd;
        std::cout << n << std::endl;
    }

    int message_size = (int) strtol(size, nullptr, 10);

    int m = 0;
    while (m < message_size)
    {
        rd = (int) read(client_descriptor, buffer + m, message_size - m);
        m = rd + m;
    }
    std::string my_string(buffer);

    return my_string.substr(0, message_size);
}


/**
 * Write message to all clients descriptors in the room.
 *
 * @param type The type of the message.
 * @param message The message.
 * @param room The room number.
 * @param name The name of the client.
 */
void Server::send_message_to_room(std::string &message, int room, std::string &name)
{

    message = '[' + get_current_time() + "] " + name + '/' + message;

    std::string message_size = std::to_string(message.size()) + '\n';

    room_mutex[room].lock();

    for (auto &c: clients)
    {
        if (c->room == room)
        {
            int write_result = (int) write(c->descriptor, message_size.c_str(), message_size.size());
            if (write_result < 0)
            {
                std::string error_message = "Nie udalo sie wyslac wiadomosci do pokoju\n";
                Logger::log('E', error_message);
                perror(message.c_str());
            }
            write_result = (int) write(c->descriptor, message.c_str(), message.size());
            if (write_result < 0)
            {
                std::string error_message = "Nie udalo sie wyslac wiadomosci do pokoju\n";
                Logger::log('E', error_message);
                perror(message.c_str());
            }
        }
    }
    room_mutex[room].unlock();
}


/**
 * Gets the nick and room number from the message from new user and sets them in the client object.
 *
 * @param c The client object.
 */
void Server::login_new_user(Client *c)
{
    std::string message = get_message(c->descriptor);

    std::string name = message.substr(0, message.find(' '));
    std::string room = message.substr(message.find(' ') + 1, message.size());

    int room_number = (int) strtol(room.c_str(), nullptr, 10);

    c->room = room_number;
    c->name = name;
}


/**
 * Sends the message about entering the room to all clients in the room.
 *
 * @param room_id The room number.
 * @param name The name of the client. 
 */
void Server::send_message_about_entering_room(int room_id, std::string &name)
{
    std::string message = "## " + name + " wchodzi do pomieszczenia ##";
    std::string system = "SYSTEM";
    send_message_to_room(message, room_id, system);
}


/**
 * Sends the message about leaving the room to all clients in the room.
 *
 * @param room_id The room number.
 * @param name The name of the client.
 */
void Server::send_message_about_leaving_room(int room_id, std::string &name)
{
    std::string message = "## " + name + " opuszcza pomieszczenie ##";
    std::string system = "SYSTEM";
    send_message_to_room(message, room_id, system);

}

/**
 * Moves the client to the new room and sends the message about leaving the old room and entering the new one.
 *
 * @param client The reference to the client object.
 *
 */
void Server::change_room(Client *client)
{
    std::string message = get_message(client->descriptor);
    int room_number = (int) strtol(message.c_str(), nullptr, 10);

    client->room = room_number;
}


/**
 * Sends the message about leaving the room to all clients in the room.
 *
 * @param client The reference to the client object.
 */
void Server::quit_room(Client *client)
{
    send_message_about_leaving_room(client->room, client->name);
}

/**
 * Removes the client from the clients vector and closes the descriptor.
 *
 * @param client The reference to the client object.
 */
void Server::remove_client(Client *client)
{
    client_mutex.lock();
    close(client->descriptor);
    for (auto it = clients.begin(); it != clients.end(); ++it)
    {
        if ((*it)->descriptor == client->descriptor)
        {
            delete *it;
            clients.erase(it);
            break;
        }
    }
    client_mutex.unlock();
}

/**
 * The thread function. It reads the action from the client and calls the appropriate method.
 *
 * @param client The reference to the client object.
 */
void *Server::my_thread(Client *c)
{
    std::string message;

    bool quit = false;
    c->action = 'n';

    while (!quit)
    {
        int rd;
        rd = (int) read(c->descriptor, &c->action, sizeof(char));

        if (rd == 0)
        {
            c->action = 'q';
        }

        switch (c->action)
        {
            case 'n':
                break;
            case 'l':
                login_new_user(c);
                send_message_about_entering_room(c->room, c->name);
                break;
            case 's':
                message = get_message(c->descriptor);
                send_message_to_room(message, c->room, c->name);
                break;
            case 'c':
                change_room(c);
                break;
            case 'q':
                quit_room(c);
                Logger::log('I', "Usunieto uzytkownika na deskryptorze: " + std::to_string(c->descriptor));
                remove_client(c);
                quit = true;
                break;
            default:
                break;
        }
    }
    return nullptr;
}

/**
 * The function that creates new Client object and starts the thread.
 *
 */
void Server::handle_connection()
{
    auto *client = new Client(descriptor);

    client_mutex.lock();
    clients.push_back(client);
    client_mutex.unlock();

    Logger::log('I', "Dodano nowego uzytkownika na deskryptorze: " + std::to_string(client->descriptor));

    std::thread(&Server::my_thread, this, client).detach();
}


