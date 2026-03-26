PROGRAM_NAME = mdfix
SOURCE_FILE = $(PROGRAM_NAME).c
OUT_DIR = out
BUILD_DIR = build
CFLAGS = -save-temps -time

CC = gcc $(CFLAGS)

all: program

program: $(SOURCE_FILE) $(BUILD_DIR)/${PROGRAM_NAME}.o
	$(CC) $< $()

$(BUILD_DIR)/$(PROGRAM_NAME).o: $(SOURCE_FILE)
	$(CC) $(SOURCE_FILE) -o $(BUILD)/$(PROGRAM_NAME).o

build:
	@mkdir -p $(BUILD_DIR)

.PHONY: build
