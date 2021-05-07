/*
* here every declaration about variable for the drawingLanguage
*/
#pragma once

#include "liste.h"

#define MAX_TYPE_NAME 50
#define MAX_VAR_NAME 50

#define CHAR_TYPE "char\0"
#define CHAR_SIZE 1
#define BOOL_TYPE "bool\0"
#define BOOL_SIZE 1
#define FLOAT_TYPE "float\0"
#define FLOAT_SIZE 4
#define INT_TYPE "int\0"
#define INT_SIZE 8
#define STRING_TYPE "string\0"
#define STRING_SIZE 100

typedef struct {
    list* list_variable;
} execution_context;

typedef struct variable {
    char* data;
    size_t size;
    char variable_name[MAX_VAR_NAME];
    char type_name[MAX_TYPE_NAME];
    struct variable (*add) (struct variable v1, struct variable v2);
    struct variable (*mul) (struct variable v1, struct variable v2);
    struct variable (*div) (struct variable v1, struct variable v2);
    struct variable (*equal) (struct variable v1, struct variable v2);
    struct variable (*lesser) (struct variable v1, struct variable v2);
    struct variable (*greater) (struct variable v1, struct variable v2);
} variable ;

variable v_operation_not_allowed(variable v1, variable v2);

/* number operations */
variable v_add_number(variable v1, variable v2);
variable v_mul_number(variable v1, variable v2);
variable v_div_number(variable v1, variable v2);
variable v_div_number(variable v1, variable v2);
variable v_equal_number(variable v1, variable v2);
variable v_greater_number(variable v1, variable v2);
variable v_lesser_number(variable v1, variable v2);

void v_set(variable* v, void* data);

/* if variable_context == NULL, no garbage collector but variable can be used out of a var context*/
variable* v_init(char* variable_name, char* data_type, execution_context* variable_context);

/* return the specifed context */
execution_context* v_get_anonymous_context();
execution_context* v_get_main_context();


/* return != 0 if variable already defined in context */
char v_check_if_already_defined(char* variable_name, execution_context* context);