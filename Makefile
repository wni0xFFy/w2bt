
TARGET = w2bt

CC = clang

CFLAGS = -Wall -Wextra -O2 -I./include/ -g
BUILD_DIR = build
BIN_DIR = bin

SRCS := $(shell find . -name "*.c" )
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
EXEC = $(BIN_DIR)/$(TARGET)

#
all: $(EXEC)
$(EXEC): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

run: $(EXEC)
	@./$(EXEC)

.PHONY: all clean run
