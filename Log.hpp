#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <csignal>

#define BACKWARD_HAS_BFD 1
#include "backward-cpp/backward.hpp"

/**
 * @def logged_exception(msg, logger)
 * @brief Creates a fatal_logged_exception with a formatted message and logs it.
 * 
 * This macro generates a formatted message that includes the file name and line number,
 * and then creates a fatal_logged_exception with this message and the provided logger.
 * 
 * @param msg The message to log, also shown in .what().
 * @param logger The logger to use for logging the message.
 */
#define logged_exception(msg, logger) fatal_logged_exception( std::string(__FILE__) +" - "+ std::to_string(__LINE__)+ ": " + msg, logger)


/** @enum Severity
 *  @brief Defines the various logging levels.
 *
 *  - OFF: No logging.
 *
 *  @todo Remove Severity to another header.
 */
enum Severity {OFF, DEBUG, INFO, WARNING, ERROR};

/** @brief Overloads the << operator to output the text representation of Severity.
 *  @param os The output stream.
 *  @param level The logging level.
 *  @return The output stream.
 */
std::ostream& operator<<(std::ostream& os, const Severity& level);

/** @brief Generates a stacktrace using Backward.cpp.
 *  @return A string containing the stacktrace.
 */
std::string stacktrace();

/** @class Logger
 *  @brief Abstract base class for logging.
 *
 *  This class provides the interface for logging messages with different severity levels.
 */
class Logger {

public:
    /** @brief Logs a message with a given severity level.
     *  @param msg The message to log.
     *  @param msg_severity The severity level of the message (default is DEBUG).
     */
    virtual void log(const std::string msg, Severity msg_severity=DEBUG) = 0;

    /** @brief Sets the logging level.
     *  @param new_logging_level The new logging level.
     */
    virtual void set_logging_level(Severity new_logging_level) = 0;

    virtual ~Logger() {};
};

/** @class FileLogger
 *  @brief A logger that writes log messages to a file.
 *
 *  This class manages the creation and updating of a log file.
 */
class FileLogger: public Logger{

private:
    std::string output_filename;
    Severity logging_level;
    std::ofstream output_file;

public: 
    /** @brief Constructs a FileLogger.
     *  @param output_filename The name of the output file (default is "./log.txt").
     *  @param logging_level The logging level (default is DEBUG).
     */
    FileLogger(const std::string output_filename="./log.txt", Severity logging_level=DEBUG);

    /** @brief Logs a message to the file.
     *  @param msg The message to log.
     *  @param msg_severity The severity level of the message (default is DEBUG).
     */
    void log(const std::string msg, Severity msg_severity=DEBUG);

    /** @brief Sets the logging level.
     *  @param new_logging_level The new logging level.
     */
    void set_logging_level(Severity new_logging_level);     

    ~FileLogger(); 
};

/** @class ConsoleLogger
 *  @brief A logger that writes log messages to a console output stream.
 */
class ConsoleLogger: public Logger {

private:
    std::ostream& output_stream;
    Severity logging_level;

public:
    /** @brief Constructs a ConsoleLogger.
     *  @param output The output stream (default is std::cerr).
     *  @param logging_level The logging level (default is DEBUG).
     */
    ConsoleLogger(std::ostream& output=std::cerr, Severity logging_level=DEBUG):
        output_stream(output), logging_level(logging_level) {};

    /** @brief Logs a message to the console.
     *  @param msg The message to log.
     *  @param msg_severity The severity level of the message (default is DEBUG).
     */
    void log(const std::string msg, Severity msg_severity=DEBUG); 

    /** @brief Sets the logging level.
     *  @param new_logging_level The new logging level.
     */
    void set_logging_level(Severity new_logging_level);

    ~ConsoleLogger();
};

/** @class fatal_logged_exception
 *  @brief Exception class that logs a fatal message upon being thrown.
 *  @brief Use logged_exception macro to include __LINE__ and __FILE__
 */
class fatal_logged_exception: public std::exception {
    const std::string msg;
    const Logger& logger;

public:
    /** @brief Constructs a fatal_logged_exception.
     *  @param msg The exception message.
     *  @param logger The logger to use for logging the message.
     */
    fatal_logged_exception(const std::string msg, Logger& logger);

    /** @brief Returns the exception message.
     *  @return The exception message.
     */
    const char* what() const noexcept;

    ~fatal_logged_exception() {};
};

/** @brief Sets up signal handling with a given logger.
 *  @param logger The logger to use for logging signals.
 */
void setup_signal_handling(Logger& logger);

/** @brief Restores the default signal handling.
 */
void restore_signal_handling();

/** @brief Signal handler that logs system signals.
 *  @param signal The signal number.
 */
void signal_handler(int signal);

#endif
