# Compiler
CC = gcc

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Files
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Output
TARGET = $(BUILD_DIR)/app

# Flags
CFLAGS = -Wall -Wextra -std=c11 -I$(INCLUDE_DIR)
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
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

# Run
run: all
	./$(TARGET)

.PHONY: all clean run
