#include <stdio.h>
#include <string.h>

#include "langage.h"
#include "variable.h"
#include "error_handling.h"

#define DEBUG


list* dl_split_func_args(char* string_to_parse){
    list* l = l_init(100);
    char last_char = 0;
    char is_in_string = 0;
    char actual_arg[100] = "\0";
    char only_space = 1;
    size_t actual_index = 0;
    for(size_t i=0; string_to_parse[i]!='\0' && i<TEXT_S_MAX; i++){
        char c = string_to_parse[i];
        if(c=='"' && last_char!='\\'){
            if(!is_in_string)
                is_in_string=1;
            else
                is_in_string=0;
        }
        if(c==' ' && last_char!=' ' && !is_in_string){ //* means we get to the next arg
            actual_arg[actual_index] = '\0';
            l_add(l, actual_arg);
            actual_index = 0;
            char only_space = 1;
        }
        else if(c!=' ' || is_in_string){
            actual_arg[actual_index] = c;
            actual_index++;
            char only_space = 0;
        }
        last_char = c;
    }
    if(!only_space){
        actual_arg[actual_index] = '\0';
        l_add(l, actual_arg);
    }
    if(is_in_string){
        l_free(l);
        print_error("parse error\n", -3, NULL);
    }
    return l;
}

list* dl_check_number_of_args(char* instruction_name, box* b, size_t min, size_t max){
    list* args = dl_split_func_args(b->text);
    // printf("args size = %d \n", args->size); ! TODO: remove that when debug is finished
    // char* arg;
    // for(size_t i=0; i<args->size; i++){
    //     arg = &l_get(args, i)->data;
    //     printf(" arg %d = %s", i, arg);
    // }
    if(args->size > max || args->size < min){
        char error_msg[200];
        sprintf(error_msg, "Wrong number of argument for instruction %s\n", instruction_name);
        print_error(error_msg, -3, b);
    }
    return args;
}

box* dl_find_first_box(list* list_box){
    for(size_t i=0; i<list_box->size; i++){
        box* b = &l_get(list_box, (int)i)->data;
        if(strcmp(b->text, FIRST_BOX_CONTENT)==0){
            return b;
        }
    }
    print_error("No first box\n", -2, NULL);
}

box* dl_exec_box(list* list_box, box* box_to_execute){
    char instruction_temp[TEXT_S_MAX];
    strcpy(instruction_temp,box_to_execute->text);
    char* instruction = strtok(instruction_temp, " "); // * because strtok parse inplace
    box* following_box;
    execution_context* context = v_get_main_context(); 
    if(strcmp(instruction, FIRST_BOX_CONTENT)==0){
        following_box = dl_instr_first(list_box, box_to_execute, context);
    }
    if(strcmp(instruction, INSTANCE_INSTR)==0){
        following_box = dl_instr_instance(list_box, box_to_execute, context);
    }
    if(strcmp(instruction, IF_INSTR)==0){
        following_box = dl_instr_if(list_box, box_to_execute, context);
    }
    if(strcmp(instruction, STORE_INSTR)==0){
        following_box = dl_instr_store(list_box, box_to_execute, context);
    }
    if(strcmp(instruction, CALC_INSTR)==0){
        following_box = dl_instr_calc(list_box, box_to_execute, context);
    }
    if(strcmp(instruction, END_INSTR)==0){
        following_box = dl_instr_end(list_box, box_to_execute, context);
    }
    if(strcmp(instruction, PRINT_INSTR)==0){
        following_box = dl_instr_print(list_box, box_to_execute, context);
    }
    return following_box;
}

void dl_exec_loop(list* list_box){
    box* current_box = dl_find_first_box(list_box);
    while (current_box){
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
    list* args = dl_check_number_of_args(INSTANCE_INSTR, box_to_execute, 3, 3);
    char *variable_type = &l_get(args, 1)->data;
    char *variable_name = &l_get(args, 2)->data;
    v_init(variable_name, variable_type, v_get_main_context());
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
    box* following = l_get(box_to_execute->children_list, 0)->data;
    return following;
}
box* dl_instr_print(list* listbox, box* box_to_execute, execution_context* context){
    printf("box content = %s\n", box_to_execute->text);
    box* following = l_get(box_to_execute->children_list, 0)->data;
    return following;
}
box* dl_instr_end(list* listbox, box* box_to_execute, execution_context* context){
    printf("end box\n");
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
