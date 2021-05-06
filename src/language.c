#include <stdio.h>
#include <string.h>

#include "langage.h"
#include "error_handling.h"

#define DEBUG

execution_context context;

box* dl_find_first_box(list* list_box){
    for(size_t i=0; i<list_box->size; i++){
        box* b = &l_get(list_box, (int)i)->data;
        if(strcmp(b->text, FIRST_BOX_CONTENT)==0){
            return b;
        }
    }
    print_error("No first box\n", -2);
}

box* dl_exec_box(list* list_box, box* box_to_execute){
    char instruction_temp[TEXT_S_MAX];
    strcpy(instruction_temp,box_to_execute->text);
    char* instruction = strtok(instruction_temp, " "); // * because strtok parse inplace
    box* following_box;
    if(strcmp(instruction, FIRST_BOX_CONTENT)==0){
        following_box = dl_instr_first(list_box, box_to_execute, &context);
    }
    if(strcmp(instruction, INSTANCE_INSTR)==0){
        following_box = dl_instr_instance(list_box, box_to_execute, &context);
    }
    if(strcmp(instruction, IF_INSTR)==0){
        following_box = dl_instr_if(list_box, box_to_execute, &context);
    }
    if(strcmp(instruction, STORE_INSTR)==0){
        following_box = dl_instr_store(list_box, box_to_execute, &context);
    }
    if(strcmp(instruction, CALC_INSTR)==0){
        following_box = dl_instr_calc(list_box, box_to_execute, &context);
    }
    if(strcmp(instruction, END_INSTR)==0){
        following_box = dl_instr_end(list_box, box_to_execute, &context);
    }
    if(strcmp(instruction, PRINT_INSTR)==0){
        following_box = dl_instr_print(list_box, box_to_execute, &context);
    }
    return following_box;
}

void dl_exec_loop(list* list_box){
    box* current_box = dl_find_first_box(list_box);
    while (current_box!=NULL){
        current_box = dl_exec_box(list_box, current_box);
    }
}

box* dl_instr_first(list* listbox, box* box_to_execute, execution_context* context){
    printf("box content = ");
    puts(box_to_execute->text);
    printf("\n");
    box* following = l_get(box_to_execute->children_list, 0)->data;
    return following;
}
box* dl_instr_instance(list* listbox, box* box_to_execute, execution_context* context){
    printf("box content = %s\n", box_to_execute->text);
    box* following = l_get(box_to_execute->children_list, 0)->data;
    return following;
}
box* dl_instr_store(list* listbox, box* box_to_execute, execution_context* context){
    printf("box content = %s\n", box_to_execute->text);
    box* following = l_get(box_to_execute->children_list, 0)->data;
    return following;
}
box* dl_instr_calc(list* listbox, box* box_to_execute, execution_context* context){
    printf("box content = %s\n", box_to_execute->text);
    box* following = l_get(box_to_execute->children_list, 0)->data;
    return following;
}
box* dl_instr_if(list* listbox, box* box_to_execute, execution_context* context){
    printf("box content = %s\n", box_to_execute->text);
    d_print_box(*box_to_execute);
    box* following = l_get(box_to_execute->children_list, 0)->data;
    return following;
}
box* dl_instr_print(list* listbox, box* box_to_execute, execution_context* context){
    printf("box content = %s\n", box_to_execute->text);
    box* following = l_get(box_to_execute->children_list, 0)->data;
    return following;
}
box* dl_instr_end(list* listbox, box* box_to_execute, execution_context* context){
    printf("box content = %s\n", box_to_execute->text);
    return NULL;
}

#ifdef DEBUG
int main(int argc, char** argv){
    if(argc!=2){
        printf("Usage : %s filename\n", argv[0]);
        return -1;
    }
    list* file_rpz = d_read_file(argv[1]);
    list* list_box = d_find_all_box(argv[1]);
    d_find_all_beg_connection(file_rpz, list_box);
    dl_exec_loop(list_box);
}
#endif
