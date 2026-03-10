CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
BIN_DIR = bin
OBJ_DIR = obj

SHELL_SRC = $(wildcard src/*.c)
SHELL_OBJ = $(SHELL_SRC:src/%.c=$(OBJ_DIR)/%.o)

CMD_SRCS = $(wildcard mojeKomande/*.c)
CMD_BINS = $(CMD_SRCS:mojeKomande/%.c=$(BIN_DIR)/%)

all: setup minishell $(CMD_BINS)

setup:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR) logs

minishell: $(SHELL_OBJ)
	$(CC) $(SHELL_OBJ) -o $@

$(BIN_DIR)/%: mojeKomande/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) minishell
	rm -f logs/*.log

.PHONY: all clean setup