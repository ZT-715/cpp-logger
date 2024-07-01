#ifndef log_h
#define log_h


#include <ostream>
#include <fstream>
#include <string>

enum Priority {OFF, DEGUB, INFO, WARNING, ERROR};

// Logger interface
class Logger {
public:
    virtual void log(std::string& msg) = 0;
    virtual void set_priority(Priority) = 0;
    virtual ~Logger() = default; 
};

// Log dynamically on multiple files
class FileLogger: Logger {};

// Pretty log on console
class ConsoleLogger: Logger {};

// Stream interface to aggregate multiple Loggers
class Log: ostream {
    private:
        Logger& file_logger;
        Logger& console_logger;

    public:
        Log (Logger& console_logger, Logger& file_logger): 
            console_logger(console_logger), file_logger(file_logger) {}

        void set_priority(Priority);

        ~Log();
};
