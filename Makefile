TARGET = my_project
TEST = test

OBJ_DIR = obj

SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c) # list all the source files
SRC_OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC)) # list all the src object files

LIB_DIR = lib
LIB = $(wildcard $(LIB_DIR)/*.c) # list all the library files
LIB_OBJ = $(patsubst $(LIB_DIR)/%.c, $(OBJ_DIR)/%.o, $(LIB)) # list all the lib object files

TEST_DIR = tests
TEST_SRC = $(wildcard $(TEST_DIR)/*.c) # list all the test files
TEST_OBJ = $(patsubst $(TEST_DIR)/%.c, $(OBJ_DIR)/%.o, $(TEST_SRC)) # list all the test object files

INC_DIR = include

# CFLAGS = -Wall -Wextra -g3 -Werror
CFLAGS = -Wall -Wextra -g3 -Werror -fsanitize=address
# CFLAGS =

all: $(TARGET)

# Make the main executable
$(TARGET): $(SRC_OBJ) $(LIB_OBJ)
	gcc $(CFLAGS) $(SRC_OBJ) $(LIB_OBJ) -o $(TARGET)

# Make the test executable
$(TEST): $(TEST_OBJ) $(LIB_OBJ)
	gcc $(CFLAGS) $(TEST_OBJ) $(LIB_OBJ) -o $(TEST)

# Make the object files from library files
$(OBJ_DIR)/%.o: $(LIB_DIR)/%.c
	gcc $(CFLAGS) -c $< -o $@

# Make the object files from source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	gcc $(CFLAGS) -c $< -o $@

# Make the object files from test files
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o *.o

fclean: clean
	rm -f $(TARGET) $(TEST)

re: fclean all