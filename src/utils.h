/*
* here unclassifiable functions
*/

#pragma once

#define U_INT 1
#define U_FLOAT 2
#define U_STRING 3
#define U_VARNAME 4

/* split expression with operator, return !=0 if operator found */
char u_parse_expression(char* expression);

// return != 0 if string is a string
char u_is_a_string(char* string);

// return != 0 if string is an integer
char u_is_an_int(char* string);

// return != 0 if string is a float
char u_is_a_float(char* string);

// return != 0 if string is a variable_name
char u_is_a_variable_name(char* string);

// return the type
char u_return_type(char* string);

// print string with \n
void u_print_string(char* string);