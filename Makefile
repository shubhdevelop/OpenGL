# Compiler
CC = g++

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Files
SRC = $(wildcard $(SRC_DIR)/*.cpp)
SRC += $(wildcard $(SRC_DIR)/tests/*.cpp)
SRC += $(wildcard $(INCLUDE_DIR)/imgui/*.cpp)
OBJ = $(SRC:%.cpp=$(BUILD_DIR)/%.o)

# Output
TARGET = $(BUILD_DIR)/app

# Detect OS
UNAME_S := $(shell uname -s)
UNAME_P := $(shell uname -p)

# Flags
CFLAGS = -Wall -Wextra -std=c++11 -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/imgui
CFLAGS += -I/usr/include -I/usr/local/include
CFLAGS += -Wno-unused-parameter

# Platform-specific setup
ifeq ($(UNAME_S),Darwin)
    # macOS
    CFLAGS += -I/opt/homebrew/include
    LIBS = -L/opt/homebrew/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework Foundation
    RUN_CMD = ./$(TARGET)
else ifeq ($(UNAME_S),Linux)
    # Linux
    LIBS = -lglfw -lGL -lm -ldl
    RUN_CMD = __NV_PRIME_RENDER_OFFLOAD=1 __GLX_VENDOR_LIBRARY_NAME=nvidia ./$(TARGET)
else ifeq ($(OS),Windows_NT)
    # Windows (MINGW/MSYS)
    LIBS = -lglfw -lopengl32 -lgdi32
    RUN_CMD = ./$(TARGET).exe
else
    # Fallback
    LIBS = -lglfw -lGL -lm -ldl
    RUN_CMD = ./$(TARGET)
endif

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
	$(RUN_CMD)

.PHONY: all clean run
