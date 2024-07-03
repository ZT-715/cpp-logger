#ifndef log_h
#define log_h


#include <ostream>
#include <fstream>
#include <string>

/* @Brief
 * Possible logging levels, DEBUG is default.
 *
 * @todo
 * Remove Severity to another header.
 */
enum Severity {OFF, DEGUB, INFO, WARNING, ERROR};


/* @Brief
 * Converts enum to text on output streams.
 */
std::ostream& operator<<(std::ostream& os, const Severity& level) {
    switch(level) {
        case OFF: 
            os << "OFF";
            break;
        case DEBUG: 
            os << "DEBUG";
            break;
        case INFO:
            os << "INFO";
            break;
        case WARNING:
            os << "WARNING";
            break;
        case ERROR:
            os << "ERROR";
            break;
        default: 
            os << " ";
            break;
    }
    return os;
}

/* @Brief
 * Abstract logger interface.
 */
class Logger {
public:
    virtual void log(std::string& msg) = 0;
    virtual void set_logging_level(Severity) = 0;
};

/* @Brief
 * Logger that manages file creation and uptdate.
 */
class FileLogger: Logger {};

/* @Brief
 * Logger that manages console output streams.
 */
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

/* @Brief
 * Stream interface to aggregate multiple Loggers.
 *
 * @todo
 * Implement stream to multiple Loggers
 * Add __LINE__ and __FILE__ inline
 * Add time to output
 */
class Log: ostream {
    private:
        Logger& file;
        Logger& console;

    public:
        Log (Logger& console_logger, Logger& file_logger): 
            console(console_logger), file(file_logger) {}

        void set_logging_level(Severity);
};
