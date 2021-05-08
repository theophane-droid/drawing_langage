/* 
* here all the functions about syntax and code interpretation
*/
#pragma once

#include "liste.h"
#include "draw.h"
#include "variable.h"

#define PRINT_INSTR "print"

variable* dl_calc_operand(char* operand);

variable dl_eval_expression(char* expression);

/* check if the number of args is between min and max, return args parsed */
list* dl_check_number_of_args(char* instruction_name, box* b, size_t min, size_t max);

/* return a list containing box args splitted by ' '. Escaping works. Return NULL if parse error */
list* dl_split_func_args(char* string_to_parse);

// this function return a pointer to the box which contains FIRST_BOX_CONTENT
box* dl_find_first_box(list* list_box);

// this function execute a box, return the next box to run
box* dl_exec_box(list* list_box, box* box_to_execute);

// this is the main function which execute the loop
void dl_exec_loop(list* list_box);

// all the following function execute a specified instruction
box* dl_instr_first(list* listbox, box* box_to_execute, execution_context* context);
box* dl_instr_instance(list* listbox, box* box_to_execute, execution_context* context);
box* dl_instr_store(list* listbox, box* box_to_execute, execution_context* context);
box* dl_instr_calc(list* listbox, box* box_to_execute, execution_context* context);
box* dl_instr_if(list* listbox, box* box_to_execute, execution_context* context);
box* dl_instr_print(list* listbox, box* box_to_execute, execution_context* context);
box* dl_instr_input(list* listbox, box* box_to_execute, execution_context* context);
box* dl_instr_end(list* listbox, box* box_to_execute, execution_context* context);