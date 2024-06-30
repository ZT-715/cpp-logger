#include <iostream>
#define BACKWARD_HAS_BFD 1
#include "backward-cpp/backward.hpp"

void test_backward(){
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
        int result = a / b; // This will cause a division by zero error
                            // which is caught by a sig handler
        std::cout << "Result: " << result << std::endl;
        
        throw std::invalid_argument("TEST backtrace"); // this is handled 
                                                       // by the catch

    } catch (std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
         
        st.load_here(32); // capture stack trace
                          // Problem: load_here() includes itself
                          // and the catch handle.
                          //
                          // Must use macro to generate trace on 
                          // throw statement.
    
        backward::TraceResolver tr;
        tr.load_stacktrace(st);
    
        std::cout << "st: ";
        p.print(st,stderr); // print stack trace
    }
}


int main(void) {
    
    test_backward();

    return 1;

}
