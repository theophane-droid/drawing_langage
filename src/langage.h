/* 
* here all the functions about syntax and code interpretation
*/
#pragma once

#include "liste.h"
#include "draw.h"

#define FIRST_BOX_CONTENT "first"
#define INSTANCE_INSTR "instance"
#define IF_INSTR "if"
#define STORE_INSTR "store"
#define CALC_INSTR "calc"
#define END_INSTR "end"
#define PRINT_INSTR "print"

typedef struct {
    char content; // ! TODO fill that
} execution_context;


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
box* dl_instr_end(list* listbox, box* box_to_execute, execution_context* context);