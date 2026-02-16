# Compiler and flags
CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra
CXXFLAGS = -std=c++23 -Wall -Wextra -O2
LDFLAGS = -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework QuartzCore

# Directories
SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = build/obj
GL3W_SRC_DIR = external/gl3w/src
GL3W_INCLUDE_DIR = external/gl3w/include
INCLUDE_DIRS = -I $(GL3W_INCLUDE_DIR) -I external/glfw/include -I external/glm
LIB_DIRS = -L external/glfw/build/src

# Source files
C_SOURCES = $(wildcard $(GL3W_SRC_DIR)/*.c)
CXX_SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
SOURCES = $(C_SOURCES) $(CXX_SOURCES)

# Object files
C_OBJECTS = $(patsubst $(GL3W_SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SOURCES))
CXX_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CXX_SOURCES))
OBJECTS = $(C_OBJECTS) $(CXX_OBJECTS)

# Phony targets
.PHONY: all clean rebuild burst

# Default target
all: burst

# Rule to build the executable
burst: $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(BUILD_DIR)/$@ $(LIB_DIRS)
	cp build/burst .

# Rule to compile C++ source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE_DIRS)

# Rule to compile C source files
$(OBJ_DIR)/%.o: $(GL3W_SRC_DIR)/%.c $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(GL3W_INCLUDE_DIR)

# Create directories if they don't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(OBJ_DIR)

# Clean target
clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)

# Rebuild target
rebuild: clean all
