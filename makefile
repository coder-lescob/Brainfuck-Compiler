CC := gcc
DB := gdb

SRC_DIR := src
BUILD_DIR := build

DEBUG_FLAGS := -g -Wall -Wextra -Wpedantic
LD_FLAGS :=

SRCS := $(wildcard $(SRC_DIR)/*.c)

TARGET := $(BUILD_DIR)/bf64

.PHONY: build
build:
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(C_FLAGS) $(SRCS) -o $(TARGET) $(LD_FLAGS)

debug: build
	@$(DB) $(TARGET)

run: build
	@$(TARGET)