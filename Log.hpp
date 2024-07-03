#ifndef log_h
#define log_h


#include <ostream>
#include <fstream>
#include <string>

enum Severity {OFF, DEGUB, INFO, WARNING, ERROR};


// Logger interface
class Logger {
public:
    virtual void log(std::string& msg) = 0;
    virtual void set_logging_level(Severity) = 0;
};

// Logs dynamically on multiple files
class FileLogger: Logger {};

// Pretty log on console
class ConsoleLogger: Logger {
    Severity logging_level;
    std::ostream& output_stream;
   
public:
    ConsoleLogger(std::ostream& output, Severity logging_level=DEBUG): output_stream(output),
                                                                  logging_level(logging_level) {
        if (logging_level != OFF)
            output_stream << "Start ConsoleLogger on " << logging_level <<  "." << std::endl;
    }
    
    void log(std::string msg, Severity msg_severity) override {
        if(msg_severity => logging_level)
            output_stream << msg << std::endl;
    }

    void set_logging_level(Severity new_logging_level) override {
        logging_level = new_logging_level;
     
        if (logging_level != OFF)
            output_stream << "Set logging level on " << logging_level << "." << std::endl;
    }
};

// Stream interface to aggregate multiple Loggers
class Log: ostream {
    private:
        Logger& file;
        Logger& console;

    public:
        Log (Logger& console_logger, Logger& file_logger): 
            console(console_logger), file(file_logger) {}

        void set_logging_level(Severity);
};
