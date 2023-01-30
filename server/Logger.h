#ifndef IRC_SERVER_LOGGER_H
#define IRC_SERVER_LOGGER_H

#include <fstream>
#include <string>

/**
 * Logger class
 * 
 */

class Logger
{

public:
    static void log(char type, const std::string &message);

private:
    static std::string currentDateTime();
};

#endif //IRC_SERVER_LOGGER_H
