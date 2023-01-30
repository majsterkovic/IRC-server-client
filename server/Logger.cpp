//
// Created by Mariusz on 28.12.2022.
//

#include <ctime>
#include <iostream>
#include "Logger.h"

/**
 * Logs the given message to the console and to the file.
 *
 * @param type The type of the message.
 * @param message The message to log.
 */
void Logger::log(char type, const std::string &message)
{
    static std::ofstream file;
    file.open("server.log", std::ios::out | std::ios::app);
    if (!file.is_open())
    {
        std::cout << "Nie udało się otworzyć pliku logów" << std::endl;
        exit(EXIT_FAILURE);
    }

    switch (type)
    {
        case 'I':
            file << "INFO\t" << currentDateTime() << "\t\t" << message << std::endl;
            std::cout << "INFO\t" << currentDateTime() << "\t\t" << message << std::endl;
            break;
        case 'W':
            file << "WARNING]\t " << currentDateTime() << "\t\t" << message << std::endl;
            std::cout << "WARNING]\t " << currentDateTime() << "\t\t" << message << std::endl;
            break;
        case 'E':
            file << "ERROR\t" << currentDateTime() << "\t\t" << message << std::endl;
            std::cout << "ERROR\t" << currentDateTime() << "\t\t" << message << std::endl;
            break;
        default:
            file << "UNKNOWN\t " << currentDateTime() << "\t\t" << message << std::endl;
            std::cout << "UNKNOWN\t " << currentDateTime() << "\t\t" << message << std::endl;
            break;
    }
    file.close();
}

/**
 * Returns the current date and time in the format YYYY-MM-DD.HH:MM:SS.
 *
 * @return The current date and time.
 */
std::string Logger::currentDateTime()
{
    time_t now = time(nullptr);
    struct tm tstruct{};
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}

