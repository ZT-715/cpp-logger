#include"Log.hpp"

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

void stacktrace() {
    backward::StackTrace st;
    backward::Printer p;

    p.object = true;
    p.color_mode = backward::ColorMode::always;
    p.address = true;
    p.snippet = true;

    std::cerr << "Stack trace: \n";

    // Todo:
    // load_here() includes itself on the trace,
    // must be removed

    st.load_here(32);
    backward::TraceResolver tr;
    tr.load_stacktrace(st);
    p.print(st, std::cerr);
}

void signal_handler(int signal) {
    std::cerr << "Signal handler: " << strsignal(signal) << ":\n";

    stacktrace();

    std::exit(signal);
}

void FileLogger::log(const std::string msg, Severity msg_severity) {
    if(logging_level != OFF && msg_severity >= logging_level)
        output_file << msg << std::endl;
}

void FileLogger::set_logging_level(Severity new_logging_level){
    logging_level = new_logging_level;
}

FileLogger::~FileLogger() {
    output_file.close(); 
}
void ConsoleLogger::log(const std::string msg, Severity msg_severity)  {
    if(logging_level != OFF && msg_severity >= logging_level)
        output_stream << msg << std::endl;
}

void ConsoleLogger::set_logging_level(Severity new_logging_level) {
    logging_level = new_logging_level;

}

fatal_logged_exception::fatal_logged_exception(const std::string msg, Logger& logger): msg{"fatal_logged_exception: " + msg}, logger{logger} {
    logger.log(this->msg, ERROR);
}

const char* fatal_logged_exception::what() const noexcept {
    return msg.c_str();
}

Log::Log(Logger& console_logger, Logger& file_logger):
    std::ostream(this), console(console_logger),
    file(file_logger) {
        setup_signal_handling();
        // original_cout = std::cout.rdbuf(this);
        original_cerr = std::cerr.rdbuf(this);
}

void Log::log(const std::string msg, Severity logging_level) {
    console.log(msg, logging_level);
    file.log(msg, logging_level);
    
    return;
}

Log::~Log() {
    this->restore_signal_handling();
//    std::cerr.rdbuf(original_cerr);
    // std::cout.rdbuff(original_cout);
}
