CC = gcc
CFLAGS = -ggdb -Wall
LDFLAGS = -lSDL2main -lSDL2 
INCLUDES = -Iinc/ -ISDL2

SRC_DIR = src
BUILD_DIR = build
BIN = $(BUILD_DIR)/gbcrapemu

# Find all .c files in src/
CFILES = $(wildcard $(SRC_DIR)/*.c)

# Replace src/*.c with build/*.o
OFILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(CFILES))

all: $(BIN)

# Link all object files into final binary
$(BIN): $(OFILES)
	$(CC) $(OFILES) -o $@ $(LDFLAGS) $(INCLUDES)

# Compile each .c file into a .o file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN)