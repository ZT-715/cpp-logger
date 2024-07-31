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
void FileLogger::log(const std::string msg, Severity msg_severity) {
    if(msg_severity <= logging_level)
        output_file << msg << std::endl;
}

void FileLogger::set_logging_level(Severity new_logging_level){
    logging_level = new_logging_level;
}

FileLogger::~FileLogger() {
    output_file.close(); 
}
void ConsoleLogger::log(const std::string msg, Severity msg_severity)  {
    if(msg_severity <= logging_level)
        output_stream << msg << std::endl;
}

void ConsoleLogger::set_logging_level(Severity new_logging_level) {
    logging_level = new_logging_level;

}




