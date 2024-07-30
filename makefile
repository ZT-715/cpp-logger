# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -g -Og -Wall -Wextra -pedantic

# Linker flags
LDFLAGS = -lbfd -ldl

# Source files
SRCS = main.cpp Log.cpp ./backward-cpp/backward.cpp

# Output executable
TARGET = main

# Default target
all: $(TARGET)

# Build the target
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) $(LDFLAGS) -o $(TARGET)

# Clean the build files
clean:
	rm -f $(TARGET)

# Doxygen documentation
doxygen:
	doxygen Doxyfile

# Phony targets
.PHONY: all clean doxygen
