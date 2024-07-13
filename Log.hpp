#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <string>

/** @enum Severity
 *  @brief Possible logging levels, DEBUG is default.
 *
 *  @todo
 *  Remove Severity to another header.
 */
enum Severity {OFF, DEBUG, INFO, WARNING, ERROR};


/** @brief Converts enum to text on output streams.
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

/** @class Logger
 *  @brief Abstract logger interface.
 */
class Logger {
public:
    virtual inline void log(std::string& msg) = 0;
    virtual void set_logging_level(Severity) = 0;
};

/** @class FileLogger
 *  @brief Logger that manages file creation and uptdate.
 */
class FileLogger: Logger {
private:
    Severity logging_level;
    std::string output_filename;
    std::ofstream output_file;

public: 
    FileLogger(std::string output_filename="./log.txt", Severity logging_level=DEBUG): 
        output_filename{output_filename}, logging_level(logging_level) {
            output_file.open(output_filename, std::ios::app);
        if(!output_file)
            throw std::ios_base::failure("Cannot open file '" + output_filename + "'.");
    }
    
    inline void log(std::string& msg, Severity msg_severity) {
        if(msg_severity <= logging_level)
            return;
        output_file << msg << std::endl;
    }

    void set_logging_level(Severity new_logging_level) {
        logging_level = new_logging_level;
    }
    
    ~FileLogger() {
        output_file.close(); 
    }
};

/** @class ConsoleLogger
 *  @brief Logger that manages console output streams.
 *  
 *  Ideally, \p output_stream is cerr which is unbuffered.
 */
class ConsoleLogger: Logger {
    Severity logging_level;
    std::ostream& output_stream;
   
public:
    ConsoleLogger(std::ostream& output=std::cerr, Severity logging_level=DEBUG):
        output_stream(output), logging_level(logging_level) {
    }
    
    inline void log(std::string& msg, Severity msg_severity)  {
        if(msg_severity <= logging_level)
            return;
        output_stream << msg << std::endl;
    }

    void set_logging_level(Severity new_logging_level) {
        logging_level = new_logging_level;
    }
};

/** @class Log
 *  @brief Stream interface to aggregate multiple Loggers.
 *
 *  @todo
 *  Implement stream to multiple Loggers
 *  Add __LINE__ and __FILE__ inline
 *  Add time to output
 */
class Log: std::ostream {
    private:
        Logger& file;
        Logger& console;

    public:
        Log (Logger& console_logger, Logger& file_logger): 
            console(console_logger), file(file_logger) {}

        void set_logging_level(Severity);
};

#endif

