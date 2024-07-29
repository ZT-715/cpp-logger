#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <csignal>

#define BACKWARD_HAS_BFD 1
#include "backward-cpp/backward.hpp"

#define logged_exception(msg, logger) fatal_logged_exception( std::string(__FILE__) +" - "+ std::to_string(__LINE__)+ ": " + msg, logger)


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

/** @function stacktrace()
 *  @brief Generates stacktrace with Backward.cpp
 *
 *  @todo
 *  Make it produce a string w/ the trace instead of accessign a global stream.
 */
std::string stacktrace();

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
class FileLogger: public Logger{

private:
    std::string output_filename;
    Severity logging_level;
    std::ofstream output_file;

public: 
    FileLogger(const std::string output_filename="./log.txt", Severity logging_level=DEBUG);
    void log(const std::string msg, Severity msg_severity=DEBUG);
    void set_logging_level(Severity new_logging_level);     
    ~FileLogger(); 
};

/** @class ConsoleLogger
 *  @brief Logger that manages console output streams.
 *  
 *  Ideally, output_stream is std::cerr, which is unbuffered.
 */
class ConsoleLogger: public Logger {

private:
    std::ostream& output_stream;
    Severity logging_level;

public:
    ConsoleLogger(std::ostream& output=std::cerr, Severity logging_level=DEBUG):
        output_stream(output), logging_level(logging_level) {};
    void log(const std::string msg, Severity msg_severity=DEBUG); 
    void set_logging_level(Severity new_logging_level);
    ~ConsoleLogger();
};
class fatal_logged_exception: public std::exception {
    const std::string msg;
    const Logger& logger;

    public:
        fatal_logged_exception(const std::string msg, Logger& logger);
        const char* what() const noexcept;

        ~fatal_logged_exception() {};
};   

void setup_signal_handling(Logger&); 
void restore_signal_handling();
/** @function signal_handler()
 *  @brief Configures log for system signals
 */
void signal_handler(int);


#endif

