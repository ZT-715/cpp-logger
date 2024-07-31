#include <iostream>
#define BACKWARD_HAS_BFD 1
#include "backward-cpp/backward.hpp"

#include"Log.hpp"

void test_log(){
    // Constroi um ConsoleLogger default
    ConsoleLogger log_console{std::cout, DEBUG};
    
    log_console.log("Iniciando ConsoleLogger.");

    // Constroi um FileLogger default
    FileLogger log_file{"./test.txt", DEBUG};

    log_file.log("Iniciando FileLogger default.");

    Logger& log_ref{log_console};

    log_ref.set_logging_level(Severity::OFF);

    log_ref.log("Mensagem que não aparece.", Severity::ERROR);
    log_ref.log("Mensagem que aparece.", Severity::OFF);

    log_ref.set_logging_level(Severity::DEBUG);
    try{
        throw logged_exception("TEST", log_ref);
    }
    catch(const std::exception& e){
        std::cout << "catch: " << e.what() << std::endl;
    }

    Log log{log_console, log_file};    
    log << "ostream" << std::endl;
}

void test_backward(std::ostream& out){
    backward::StackTrace st; // initialize stack trace
    backward::Printer p;

    p.object = true;
    p.color_mode = backward::ColorMode::always;
    p.address = true;
    p.snippet = true;

   
    
    try {
        // Simulate a core dump (division by zero)
        volatile int a = 1;
        int b = --a;
        int result = a + b; // This will cause a division by zero error
                            // which is caught by a sig handler
        out << "Result: " << result << std::endl;
        
        throw std::invalid_argument("TEST backtrace"); // this is handled 
                                                       // by the catch

    } catch (std::exception& e) {
        out << "Exception caught: " << e.what() << std::endl;
         
        st.load_here(32); // capture stack trace
                          // Problem: load_here() includes itself
                          // and the catch handle.
                          //
                          // Must use macro to generate trace on 
                          // throw statement.
    
        backward::TraceResolver tr;
        tr.load_stacktrace(st);
    
        out << "st: ";
        p.print(st,out); // print stack trace
    }
}


int main(void) {
    
//    test_log();

        // Constroi um ConsoleLogger default
    ConsoleLogger log_console{std::cout, DEBUG};
    
    log_console.log("Iniciando ConsoleLogger.");

    // Constroi um FileLogger default
    FileLogger log_file{"./test.txt", DEBUG};

    log_file.log("Iniciando FileLogger default.");


    Log log{log_console, log_file};    
    log << "ostream" << std::endl;

    test_backward(log);

    return 1;

}
