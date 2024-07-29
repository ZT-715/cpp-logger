#include"Log.hpp"

Logger* signal_logger;

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

std::string stacktrace() {
    backward::StackTrace st;
    backward::Printer p;

    std::stringstream ss;

    p.object = true;
    p.color_mode = backward::ColorMode::always;
    p.address = true;
    p.snippet = true;

    ss << "Stack trace: \n";

    // Todo:
    // load_here() includes itself on the trace,
    // must be removed

    st.load_here(32);
    backward::TraceResolver tr;
    tr.load_stacktrace(st);
    p.print(st, ss);

    return ss.str();
}

void signal_handler(int signal) {
    std::stringstream ss;
    ss <<  "Signal handler: " << strsignal(signal) << ":\n" << stacktrace();
    signal_logger->log(ss.str());

    std::exit(signal);
}

FileLogger::FileLogger(const std::string output_filename, Severity logging_level):
    output_filename(output_filename), logging_level(logging_level) {
   
    output_file.open(output_filename, std::ios::app);
    if(!output_file)
       throw std::ios_base::failure("Cannot open file '" + output_filename + "'.");
}

void FileLogger::log(const std::string msg, Severity msg_severity) {
    if(logging_level != OFF && msg_severity >= logging_level)
        output_file << msg << std::endl;
}


void FileLogger::set_logging_level(Severity new_logging_level){
    logging_level = new_logging_level;
}

FileLogger::~FileLogger() {
    this->log(stacktrace());
    output_file.close(); 
}
void ConsoleLogger::log(const std::string msg, Severity msg_severity)  {
    if(logging_level != OFF && msg_severity >= logging_level)
        output_stream << msg << std::endl;
}

void ConsoleLogger::set_logging_level(Severity new_logging_level) {
    logging_level = new_logging_level;

}
ConsoleLogger::~ConsoleLogger() {
}

fatal_logged_exception::fatal_logged_exception(const std::string msg, Logger& logger): msg{"fatal_logged_exception: " + msg}, logger{logger} {
    logger.log(this->msg, ERROR);
}

const char* fatal_logged_exception::what() const noexcept {
    return msg.c_str();
}

void setup_signal_handling(Logger& out) {

    signal_logger = &out;

    std::signal(SIGTERM,signal_handler );
    std::signal(SIGINT,signal_handler);
    std::signal(SIGFPE,signal_handler );
    std::signal(SIGABRT,signal_handler );
    std::signal(SIGSEGV,signal_handler );
}

void restore_signal_handling() {
    signal_logger = nullptr;
    // Restore the old handler
    std::signal(SIGTERM, SIG_DFL);
    std::signal(SIGINT, SIG_DFL);
    std::signal(SIGSEGV, SIG_DFL);
    std::signal(SIGABRT, SIG_DFL);
    std::signal(SIGFPE, SIG_DFL);
}


