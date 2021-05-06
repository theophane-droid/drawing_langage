#include <stdio.h>
#include <stdlib.h>

#include "error_handling.h"

void print_error(char* error_message, int exit_code){
    fprintf(stderr, error_message);
    exit(exit_code);
}
