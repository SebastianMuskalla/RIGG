//
// Created by Sebastian on 6 Sep 2020.
//

#ifndef RIGG_LOGGER_H
#define RIGG_LOGGER_H


#include <string>
#include <iostream>

class Logger {
private:

    unsigned int logLevel = 0;

    constexpr static unsigned int maxLogLevel = 1;

protected:
    Logger() {}

    static Logger* instance;


public:

    Logger(Logger &other) = delete;

    void operator=(const Logger &) = delete;

    static Logger *getInstance();



    Logger& operator<<(const std::string & str) {
        if (logLevel <= maxLogLevel)
        {
            if (logLevel == 0) {
                std::cout << str;
            }
            else
            {
                std::cout << "DEBUG(" << logLevel << "): " << str;
            }
        }
        else
        {
            std::cout << "SUPPRESSED(" << logLevel << "): " << str;
        }

        return *this;
    }

};


Logger* Logger::instance= nullptr;;


Logger *Logger::getInstance()
{
    if(instance==nullptr){
        instance = new Logger();
    }
    return instance;
}


class Singleton
{

};



#endif //RIGG_LOGGER_H
