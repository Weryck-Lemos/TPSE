CC = gcc
CFLAGS = -Wall -I./src

OBJ_DIR =obj
SRC_DIR =src
BIN_DIR = bin

OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/funcao01.o $(OBJ_DIR)/funcao02.o $(OBJ_DIR)/funcao03.o $(OBJ_DIR)/imprime.o

TARGET = $(BIN_DIR)/app

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)
