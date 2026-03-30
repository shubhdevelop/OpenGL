# Compiler
CC = g++

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Files
SRC = $(wildcard $(SRC_DIR)/*.cpp)
SRC += $(wildcard $(INCLUDE_DIR)/imgui/*.cpp)
OBJ = $(SRC:%.cpp=$(BUILD_DIR)/%.o)

# Output
TARGET = $(BUILD_DIR)/app

# Flags
CFLAGS = -Wall -Wextra -std=c++11 -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/imgui
CFLAGS += -I/usr/include
CFLAGS += -I/usr/local/include

# Libraries
LIBS = -lglfw -lGL -lm -ldl
# macOS alternative:
# LIBS = -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

# GLAD (if included manually)
# ensure glad.c is compiled as part of SRC

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJ) -o $@ $(LIBS)

# Compile
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

# Run
run: all
	__NV_PRIME_RENDER_OFFLOAD=1 __GLX_VENDOR_LIBRARY_NAME=nvidia ./$(TARGET)

.PHONY: all clean run
