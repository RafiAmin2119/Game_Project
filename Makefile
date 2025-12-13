# Compiler
CXX = g++
CXXFLAGS = -I C:/SFML-2.6.2/include -std=c++17
LDFLAGS = -L C:/SFML-2.6.2/lib -lsfml-graphics -lsfml-window -lsfml-system

# Files
SRC = main.cpp
OUT = test.exe

# Default build target
all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(SRC) -o $(OUT) $(CXXFLAGS) $(LDFLAGS)

# Run the program
run: all
	./$(OUT)

# Clean
clean:
	del $(OUT)
