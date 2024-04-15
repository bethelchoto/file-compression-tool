#include "error_handling.h"

// Signal handler functions
void handle_sigsegv(int signum) {
    report_error(ERROR_SIGSEGV);
    graceful_termination();
}

void handle_sigill(int signum) {
    report_error(ERROR_SIGILL);
    graceful_termination();
}

void handle_sigfpe(int signum) {
    report_error(ERROR_SIGFPE);
    graceful_termination();
}


void graceful_termination() {
    fprintf(stderr, "Program exited due to errors.\n");
    exit(EXIT_FAILURE);
}

// Signal handling setup function
void setup_signal_handlers() {

    signal(SIGSEGV, handle_sigsegv);
    signal(SIGILL, handle_sigill);
    signal(SIGFPE, handle_sigfpe);

}

// Function to print descriptive error messages
void report_error(int error_code) {
    switch (error_code) {
        case ERROR_INVALID_CMD_ARG:
            fprintf(stderr, "Error: Invalid command line argument. Usage: ./a.out <input file>\n");
            break;
        case ERROR_OPEN_INPUT_FILE:
            fprintf(stderr, "Error: Failed to open input file.\n");
            break;
        case ERROR_MEMORY_ALLOCATION:
            fprintf(stderr, "Error: Memory allocation failed.\n");
            break;
        case ERROR_INVALID_FILENAME:
            fprintf(stderr, "Error: Input file name does not contain \"_compressed\".\n");
            break;
        case ERROR_READ_FILE:
            fprintf(stderr, "Error: Failed to read from file.\n");
            break;
        case ERROR_CREATE_OUTPUT_FILE:
            fprintf(stderr, "Error: Failed to create output file.\n");
            break;
        case ERROR_UNEXPECTED_EOF:
            fprintf(stderr, "Error: Unexpected end of file.\n");
            break;
        case ERROR_INVALID_INPUT_FORMAT:
            fprintf(stderr, "Error: Invalid input format.\n");
            break;
        case ERROR_UNKNOWN_SIGNAL:
            fprintf(stderr, "Error: Received unknown signal.\n");
            break;
        case ERROR_SIGFPE:
            fprintf(stderr, "Floating point exception (SIGFPE) occurred. Program terminated.\n");
            break;
        case ERROR_SIGILL:
            fprintf(stderr, "Illegal instruction (SIGILL) occurred. Program terminated.\n");
            break;
        case ERROR_SIGSEGV:
            fprintf(stderr, "Segmentation fault (SIGSEGV) occurred. Program terminated.\n");
            break;
        default:
            fprintf(stderr, "Unknown error occurred.\n");
    }
}
