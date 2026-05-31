CC := gcc
DB := gdb

ASM := nasm
LD := ld

SRC_DIR := src
TEST_DIR := test
TEST_OUT_DIR := test_outputs
EXPECTEDS_DIR := test_expecteds
BUILD_DIR := build

DEBUG_FLAGS := -g -Wall -Wextra -Wpedantic #-fsanitize=address
LD_FLAGS := 

SRCS := $(wildcard $(SRC_DIR)/*.c)
TESTS := $(wildcard $(TEST_DIR)/*.bf)

TARGET := $(BUILD_DIR)/bf64
TARGET_RELEASE := $(BUILD_DIR)/bf64-release

.PHONY: build
build:
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(DEBUG_FLAGS) $(SRCS) -o $(TARGET) $(LD_FLAGS)

release:
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(SRCS) -o $(TARGET_RELEASE) $(LD_FLAGS)

debug: build
	@$(DB) $(TARGET)

run: build
	$(TARGET) $(ARGS)

define sizeof
    $$(du -sb \
    $(1) \
    | cut -f1 )
endef

test: build
# first compile all tests programs without careing if it failed or not
	@mkdir -p $(TEST_OUT_DIR)
	@rm -f $(TEST_DIR)/*.log
	@-$(foreach TEST, $(TESTS), $(TARGET) $(TEST) -o $(TEST_OUT_DIR)/$(notdir $(TEST)).s > $(TEST).log ; )
	@-$(foreach TEST, $(TESTS), $(ASM) -felf64 $(TEST_OUT_DIR)/$(notdir $(TEST)).s -o $(BUILD_DIR)/$(notdir $(TEST)).o ; )
	@-$(foreach TEST, $(TESTS), $(LD) $(BUILD_DIR)/$(notdir $(TEST)).o -o $(BUILD_DIR)/$(notdir $(TEST))_bin ; )