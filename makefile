# Compiler
CXX = g++
 
# Compiler flags
CXXFLAGS = -g -Og -Wall -Wextra -pedantic -std=c++11

# Linker flags
LDFLAGS = -ldwarf -lelf -ldl 

# Source files
SRCS = main.cpp Log.cpp ./backward-cpp/backward.cpp

# Build the target
build: 
	$(CXX) $(CXXFLAGS) $(SRCS) $(LDFLAGS) -o main.o

# Clean the build files
clean:
	rm -f $(TARGET)

# Doxygen documentation
doxygen:
	doxygen Doxyfile

# Phony targets
.PHONY: all clean doxygen
