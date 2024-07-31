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
std::ostream& operator<<(std::ostream& os, const Severity& level);

/** @class Logger
 *  @brief Abstract logger interface.
 */
class Logger {
public:
    virtual void log(const std::string  msg, Severity msg_severity=DEBUG) = 0;
    virtual void set_logging_level(Severity new_logging_level) = 0;
    virtual ~Logger() {};
};

/** @class FileLogger
 *  @brief Logger that manages file creation and uptdate.
 */
class FileLogger: public Logger {
private:
    std::string output_filename;
    Severity logging_level;
    std::ofstream output_file;

public: 
    FileLogger(const std::string output_filename="./log.txt", Severity logging_level=DEBUG):
        output_filename(output_filename), logging_level(logging_level) {
        output_file.open(output_filename, std::ios::app);
        if(!output_file)
           throw std::ios_base::failure("Cannot open file '" + output_filename + "'.");
    } 
    void log(const std::string msg, Severity msg_severity=DEBUG);
    void set_logging_level(Severity new_logging_level);     
    ~FileLogger(); 
};

/** @class ConsoleLogger
 *  @brief Logger that manages console output streams.
 *  
 *  Ideally, \p output_stream is cerr which is unbuffered.
 */
class ConsoleLogger: public Logger {
    std::ostream& output_stream;
    Severity logging_level;
   
public:
    ConsoleLogger(std::ostream& output=std::cerr, Severity logging_level=DEBUG):
        output_stream(output), logging_level(logging_level) {};
    
    void log(const std::string msg, Severity msg_severity=DEBUG); 

    void set_logging_level(Severity new_logging_level);

    ~ConsoleLogger() {}
};

/** @class Log
 *  @brief Stream interface to aggregate multiple Loggers.
 *
 *  @todo
 *  Implement stream to multiple Loggers
 *  Add __LINE__ and __FILE__ inline
 *  Add time to output
 */
class Log: public std::ostream {
    private:
        Logger& console;
        Logger& file;

    public:
        Log (Logger& console_logger, Logger& file_logger): 
            console(console_logger), file(file_logger) {} 

        void set_logging_level(Severity);
};

#endif

