#include <iostream>
#define BACKWARD_HAS_BFD 1
#include "backward-cpp/backward.hpp"

#include"Log.hpp"

void test_log(){
    ConsoleLogger log_console{std::cout, DEBUG};
    
    log_console.log("Iniciando ConsoleLogger.");

    FileLogger log_file{"./test.txt", DEBUG};

    log_file.log("Iniciando FileLogger default.");

    Logger& log_ref{log_console};

    log_ref.set_logging_level(Severity::OFF);

    log_ref.log("Mensagem que não aparece.", Severity::ERROR);
    log_ref.log("Mensagem que não aparece.", Severity::OFF);

    log_ref.set_logging_level(Severity::DEBUG);

    log_ref.log("Mensagem que aparece.", Severity::ERROR);
    try{
        throw logged_exception("TEST", log_ref);
    }
    catch(const std::exception& e){
        std::cout << "catch: " << e.what() << std::endl;
    }
 
    return;
}

void test_backward(){
    backward::StackTrace st; // initialize stack trace
    backward::Printer p;

    p.object = true;
    p.color_mode = backward::ColorMode::always;
    p.address = true;
    p.snippet = true;

     
    st.load_here(32); // capture stack trace
                      // Problem: load_here() includes itself
                      // and the catch handle.

    backward::TraceResolver tr;
    tr.load_stacktrace(st);

    std::cerr << "st: ";
    p.print(st,std::cerr); // print stack trace
}


int main(int argc, char* argv[]) {
    
    test_log();

    ConsoleLogger log_console{std::cout, DEBUG};
    log_console.log("Iniciando ConsoleLogger.");
    FileLogger log_file{"./test_singal.txt", DEBUG};
    log_file.log("Iniciando FileLogger default.");
 
    if (argc == 2) {
                setup_signal_handling(log_console);
    }
    else {
       setup_signal_handling(log_file);
    }

    // Simulate a core dump (division by zero)
    volatile int a = 1;
    int b = --a;
    int result = a / b; // This will cause a division by zero error
                        // which is caught by a sig handler
    std::cerr << "Result: " << result << std::endl;

// test_backward();

    std::cerr << "FI\n";
    return 1;

}
