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

extern void stacktrace();
extern void signal_handler(int);

/** @class Logger
 *  @brief Abstract logger interface.
 */
class Logger {
public:
    virtual void log(const std::string  msg, Severity msg_severity=DEBUG) = 0;
    virtual void set_logging_level(Severity new_logging_level) = 0;
    virtual int log_c(char c) = 0;
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
protected:
    int log_c(char c){
        output_file.put(c);
        return 0;
    }
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
private:
    std::ostream& output_stream;
    Severity logging_level;
protected:
   int log_c(char c){
        output_stream.put(c);
        return 0;
    }
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
 *  Add time to output
 */
class Log: public std::ostream,
           private std::streambuf  {
    private:
        Logger& console;
        Logger& file;
        std::streambuf* original_cout;
        std::streambuf* original_cerr;
        struct sigaction old_sigsegv_handler;
        struct sigaction old_sigabrt_handler;
        struct sigaction old_sigfpe_handler;


    protected:

        void setup_signal_handling() {
            struct sigaction sa;
            sa.sa_handler = signal_handler;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;

            // Save the old handlers and set the new ones
            sigaction(SIGSEGV, &sa, &old_sigsegv_handler);
            sigaction(SIGABRT, &sa, &old_sigabrt_handler);
            sigaction(SIGFPE, &sa, &old_sigfpe_handler);
        }

        void restore_signal_handling() {
            // Restore the old handlers
            sigaction(SIGSEGV, &old_sigsegv_handler, nullptr);
            sigaction(SIGABRT, &old_sigabrt_handler, nullptr);
            sigaction(SIGFPE, &old_sigfpe_handler, nullptr);
        }

         int overflow(int c) override {
                //file.log_c(c);
                console.log_c(c);
                return 0;
            }

    public:
        Log (Logger& console_logger, Logger& file_logger);       
        void log(const std::string msg, Severity logging_level=DEBUG); 


        void set_logging_level(Severity);

        ~Log();

};

class fatal_logged_exception: public std::exception {
    const std::string msg;
    const Logger& logger;
    public:
        fatal_logged_exception(const std::string msg, Logger& logger);
        const char* what() const noexcept;

        ~fatal_logged_exception() {};
};   

#endif

