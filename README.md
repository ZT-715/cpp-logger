### README for C++ Exception Logger with Stacktrace

#### Overview
This project implements a **C++ logging library** designed to handle exceptions, stack traces, and message filtering. The logger is flexible, supporting multiple output options such as the console and files, while filtering messages based on severity levels.

#### Features
1. **Modular Logger Interface**:
   - Abstract `Logger` class with implementations like `FileLogger` and `ConsoleLogger`.
   - Supports RAII for resource management.
2. **Log Filtering**:
   - Five severity levels: OFF, DEBUG, INFO, WARNING, ERROR.
   - Customizable with `set_logging_level()` for dynamic filtering.
3. **Exception Logging**:
   - `fatal_logged_exception` class, derived from `std::exception`, logs detailed errors including file name and line number.
   - Example log: `ERROR: fatal_logged_exception: main.cpp - 27: TEST`.
4. **Stacktrace Support**:
   - Integrated with **Backward-Cpp** to provide detailed stack trace information, including function names, code lines, and file paths.
5. **Signal Handling**:
   - Custom `signal_handler` captures unexpected program signals (e.g., segmentation faults) and logs details along with the stack trace.

#### How to Run

To run successfully, the libdwarf and libelf libraries are needed, be sure to have them installed and on the path CPLUS_INCLUDE_PATH and LIBRARY_PATH.

```
git clone https://github.com/ZT-715/cpp-logger

cd cpp-logger/

git submodule init

git submodule update

make build

./main.o
```

#### Next Steps
Potential improvements include:
- Resolving memory allocation issues in signal handling.
- Reducing global variable dependency to align with RAII principles.

#### References
- [Backward-Cpp GitHub](https://github.com/bombela/backward-cpp)
- [Custom Exceptions in C++](https://stackoverflow.com/questions/16945787/how-is-it-possible-to-overload-the-throw-function-while-writing-a-custom-excepti)
