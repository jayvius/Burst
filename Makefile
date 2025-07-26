# Compiler and flags
CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra
CXXFLAGS = -std=c++23 -Wall -Wextra -O2
LDFLAGS = -lglfw

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin
DEPDIR = .deps
GL3W_SRCDIR = gl3w/src
GL3W_INCLUDE_DIR = gl3w/include

# Source files
C_SOURCES = $(wildcard $(GL3W_SRCDIR)/*.c)
CXX_SOURCES = $(wildcard $(SRCDIR)/*.cpp)
SOURCES = $(C_SOURCES) $(CXX_SOURCES)

# Object files
C_OBJECTS = $(patsubst $(GL3W_SRCDIR)/%.c,$(OBJDIR)/%.o,$(C_SOURCES))
CXX_OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(CXX_SOURCES))
OBJECTS = $(C_OBJECTS) $(CXX_OBJECTS)

# Executable name
TARGET = $(BINDIR)/burst

# Phony targets
.PHONY: all clean rebuild

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

# Rule to compile C++ source files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) $(DEPDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -MMD -MP -MF $(DEPDIR)/$*.d -I $(GL3W_INCLUDE_DIR)

# Rule to compile C source files
$(OBJDIR)/%.o: $(GL3W_SRCDIR)/%.c | $(OBJDIR) $(DEPDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MMD -MP -MF $(DEPDIR)/$*.d -I $(GL3W_INCLUDE_DIR)

# Create directories if they don't exist
$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(DEPDIR):
	mkdir -p $(DEPDIR)

# Include dependency files
-include $(wildcard $(DEPDIR)/*.d)

# Clean target
clean:
	rm -rf $(OBJDIR) $(BINDIR) $(DEPDIR)

# Rebuild target
rebuild: clean all
