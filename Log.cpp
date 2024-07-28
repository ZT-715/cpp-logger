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

int FileLogger::log_c(char c, Severity msg_severity) {
    if(logging_level != OFF && msg_severity >= logging_level)
        output_file << c;
    return 0;
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
int ConsoleLogger::log_c(char c, Severity msg_severity) {
    if(logging_level != OFF && msg_severity >= logging_level)
        output_stream << c;
    return 0;
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

void Log::setup_signal_handling() {
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    // Save the old handlers and set the new ones                                                                                          
    sigaction(SIGSEGV, &sa, &old_sigsegv_handler);
    sigaction(SIGABRT, &sa, &old_sigabrt_handler);
    sigaction(SIGFPE, &sa, &old_sigfpe_handler);
}

void Log::restore_signal_handling() {
    // Restore the old handler
    sigaction(SIGSEGV, &old_sigsegv_handler, nullptr);
    sigaction(SIGABRT, &old_sigabrt_handler, nullptr);
    sigaction(SIGFPE, &old_sigfpe_handler, nullptr);
}

int Log::overflow(int c) {                                                                                              
    file.log_c(c);
    console.log_c(c);
    return 0;
}

Log::~Log() {
    this->restore_signal_handling();
    std::cerr.rdbuf(original_cerr);
    // std::cout.rdbuff(original_cout);
}
