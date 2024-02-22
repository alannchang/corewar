TARGET = vm
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

SUB_MOD_DIR = submodules
GTEST_DIR = $(SUB_MOD_DIR)/googletest/googletest

INC_DIR = inc

# CFLAGS = -Wall -Wextra -g3 -Werror
CFLAGS = -Wall -Wextra -g3 -Werror -fsanitize=address
# CFLAGS =

all: $(TARGET)

# Make the main executable
$(TARGET): $(SRC_OBJ) $(LIB_OBJ)
	gcc $(CFLAGS) $(SRC_OBJ) $(LIB_OBJ) -o $(TARGET)

# Make the test executable
$(TEST): $(TEST_OBJ) $(LIB_OBJ) $(LIB_DIR)/gtest_main.a
	g++ -isystem $(GTEST_DIR)/include/ $(LIB_DIR)/gtest_main.a $(OBJ_DIR)/test.o -o test

$(TEST_OBJ): $(TEST_SRC)
	g++ -isystem $(GTEST_DIR)/include/ -c $(TEST_DIR)/test.c -o $(OBJ_DIR)/test.o

$(LIB_DIR)/gtest_main.a: $(OBJ_DIR)/gtest-all.o $(OBJ_DIR)/gtest_main.o
	ar rs $(LIB_DIR)/gtest_main.a $(OBJ_DIR)/gtest-all.o $(OBJ_DIR)/gtest_main.o

$(OBJ_DIR)/gtest-all.o:
	g++ -isystem $(GTEST_DIR)/include/ -I $(GTEST_DIR) -c $(GTEST_DIR)/src/gtest-all.cc -o $(OBJ_DIR)/gtest-all.o

$(OBJ_DIR)/gtest_main.o:
	g++ -isystem $(GTEST_DIR)/include/ -I $(GTEST_DIR) -c $(GTEST_DIR)/src/gtest_main.cc -o $(OBJ_DIR)/gtest_main.o


# Make the object files from library files
$(OBJ_DIR)/%.o: $(LIB_DIR)/%.c
	gcc $(CFLAGS) -c $< -o $@

# Make the object files from source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	gcc $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ_DIR)/*.o *.o *.a

fclean: clean
	rm -f $(TARGET) $(TEST) $(LIB_DIR)/*.a

re: fclean all