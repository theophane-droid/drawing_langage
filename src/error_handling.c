#include <stdio.h>
#include <stdlib.h>

#include "error_handling.h"
#include "draw.h"

void print_error(char* error_message, int exit_code, box* actual_box){
    fprintf(stderr, error_message);
    if(actual_box){
        fprintf(stderr, "Note : This error hapenned in the box containing \"%s\"\n", actual_box->text);
    }
    exit(exit_code);
}
