# Mini Shell – Unix-like Command Line Interpreter (C, Linux, POSIX)
[![Build and Test](https://github.com/Strahinja147/MiniShell-C/actions/workflows/ci.yml/badge.svg)](https://github.com/Strahinja147/MiniShell-C/actions/workflows/ci.yml)
## Overview

Custom Unix-like shell implemented in C using POSIX system calls.  
The project demonstrates low-level process control, inter-process communication, file descriptor manipulation, and terminal management on Linux systems.

The shell supports command execution, pipelines, redirection, background processes, and includes custom implementations of common Unix utilities.

---

## Features

### Process Management
- Full **fork–exec** model implementation  
- Proper child process synchronization using `waitpid`  
- Support for foreground and background execution (`&`)  

### Inter-Process Communication (IPC)
- Pipeline support (`|`) using `pipe`  
- I/O redirection:
  - `>` (overwrite)
  - `>>` (append)
  - `2>` (stderr redirection)
  - `&>` (stdout + stderr)
- File descriptor manipulation via `dup2` and `open`

### Background Execution
- Commands can be executed in background using `&`  
- Automatic creation of log files inside `miniShell/logs/`  
- Log naming format: `pid_<ID>.log`  

### Terminal Control & History
- Raw terminal mode using `termios`  
- Command history navigation via arrow keys  
- Manual input handling and character-level control  

### Custom System Utilities
Independent implementations of common Unix commands (without relying on system binaries):

- `ls`
- `cat`
- `ps` (parses `/proc` directly)
- `head`
- `tail`
- `touch`
- `rm`
- `mkdir`
- `rmdir`
- `cp`
- `pwd`
- `whoami`
- `echo`
- `date`

The `ps` command retrieves process metadata by directly parsing the `/proc` filesystem.

---

## Architecture Overview

The project is modular and organized into multiple components:

- `shell.c` – Main loop and command parsing  
- `utils.c` – Helper functions  
- `history.c` – Command history management  
- `builtin.c` – Built-in command handling  
- `external.c` – External command execution  
- `pipe.c` – Pipe creation and handling  
- `redirection.c` – I/O redirection logic  
- `backgroundProcess.c` – Background execution management  

**Execution flow:**

User Input → Parsing → Execution Engine → Fork/Exec → Pipe/Redirection Handling → Synchronization

---

## System Calls Used

- `fork`
- `execvp`
- `waitpid`
- `pipe`
- `dup2`
- `open`
- `read`
- `write`
- `tcgetattr`
- `tcsetattr`

---

## Memory Safety

The project was tested using **Valgrind** to ensure proper memory management.

Result:

```
0 bytes definitely lost
```

All dynamically allocated memory used for input reading, argument parsing, and history handling is properly freed.

---

## Build & Run

### Requirements
- Linux OS
- GCC
- Make

### Build

From the project root directory:

```
make
```

This will:
- Create the required `logs` directory  
- Compile the main shell  
- Compile all custom external utilities  

### Run

```
cd miniShell
./shell
```

### Clean

```
make clean
```

---

## Example Usage

Basic command execution:

```
ls
```

Pipelines and redirection:

```
ps | head 5
ps > processes.txt
```

Background execution:

```
ls &
```

Directory navigation:

```
cd ..
pwd
```

---

## Purpose

This project focuses on understanding Unix internals and system-level programming concepts through a fully functional shell implementation written in C.
