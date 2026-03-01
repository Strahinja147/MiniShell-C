CC = gcc
CFLAGS = -Wall -g

SHELL_DIR = miniShell
CMDS_DIR = mojeKomande
LOGS_DIR = $(SHELL_DIR)/logs

SHELL_SRCS = $(SHELL_DIR)/shell.c $(SHELL_DIR)/utils.c $(SHELL_DIR)/history.c \
             $(SHELL_DIR)/builtin.c $(SHELL_DIR)/external.c $(SHELL_DIR)/pipe.c \
             $(SHELL_DIR)/redirection.c $(SHELL_DIR)/backgroundProcess.c
SHELL_OUT = $(SHELL_DIR)/shell

CMD_SRCS = $(wildcard $(CMDS_DIR)/*.c)
CMD_OUTS = $(CMD_SRCS:.c=)

all: setup shell commands

setup:
	mkdir -p $(LOGS_DIR)

shell:
	$(CC) $(CFLAGS) $(SHELL_SRCS) -o $(SHELL_OUT)

commands: $(CMD_OUTS)

$(CMDS_DIR)/%: $(CMDS_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(SHELL_OUT)
	rm -f $(CMD_OUTS)
	rm -rf $(LOGS_DIR)/*.log