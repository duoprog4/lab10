# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra

# Target executable
TARGET = double_add

# Source files
SRC = main.cpp

# Build rule
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Clean rule
clean:
	rm -f $(TARGET)