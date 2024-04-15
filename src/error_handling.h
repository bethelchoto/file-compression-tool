#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// Error codes
#define ERROR_INVALID_CMD_ARG 1
#define ERROR_OPEN_INPUT_FILE 2
#define ERROR_MEMORY_ALLOCATION 3
#define ERROR_INVALID_FILENAME 4
#define ERROR_READ_FILE 5
#define ERROR_CREATE_OUTPUT_FILE 6
#define ERROR_UNEXPECTED_EOF 7
#define ERROR_INVALID_INPUT_FORMAT 8
#define ERROR_UNKNOWN_SIGNAL 9
#define ERROR_SIGFPE 10
#define ERROR_SIGILL 11
#define ERROR_SIGSEGV 12

// Signal codes
#define SIGINFO_SIGNAL 29

// Signal handler functions
void handle_sigsegv(int signum);
void handle_sigill(int signum);
void handle_sigfpe(int signum);

// Error reporting functions
void report_error(int error_code);
void graceful_termination();

// Signal handling setup function
void setup_signal_handlers();

#endif
