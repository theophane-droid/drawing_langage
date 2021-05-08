#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "langage.h"
#include "variable.h"
#include "error_handling.h"
#include "globals.h"
#include "utils.h"

#define DEBUG

/* raise an error if the number of children is not btw min and max */
void dl_check_number_of_children(box* b, size_t min, size_t max){
    if(b->children_list->size < min || b->children_list->size > max ){
            char error_msg[200] ;
            sprintf(error_msg, "The number of children boxes is not correct for this instruction, this instruction is waiting between %d and %d children\n", (int)min, (int) max);
            print_error(error_msg, -3, b);
    }
}
variable* dl_calc_operand(char* operand){
    char type = u_return_type(operand);
    variable* v;
    if(type==U_VARNAME){
        v = v_find(operand, v_get_main_context());
        if(!v){
            char error_msg[200];
            sprintf(error_msg, "%s variable not found in context\n", operand);
            print_error(error_msg, -3, NULL);
        }
    }
    if(type==U_INT){
        v = v_init("an", INT_TYPE, v_get_anonymous_context());
        int i=atoi(operand);
        v_set(v, &i);
    }
    if(type==U_FLOAT){
        v = v_init("an", FLOAT_TYPE, v_get_anonymous_context());
        float f=atof(operand);
        v_set(v, &f);
    }
    if(type==U_STRING){
        operand++;
        operand[strlen(operand)-1] = 0;
        v = v_init("an", STRING_TYPE, v_get_anonymous_context());
        v_set(v, operand);
    }
    return v;
}
variable dl_eval_expression(char* expression){
    variable res;
    char operator = u_parse_expression(expression);
    char* op1=expression;
    char* op2=expression;
    while(op2[0]!=0)
        op2++;
    op2++;
    variable* op1_v = dl_calc_operand(op1);
    variable* op2_v = dl_calc_operand(op2);
    if(strcmp(op1_v->type_name, op2_v->type_name)!=0){
        char error_msg[200];
        sprintf(error_msg, "Invalid operation between %s and %s\n", op1_v->type_name, op2_v->type_name);
        print_error(error_msg, -3, NULL);
    }
    if(operator == ADD_OPERATOR)
        res = op1_v->add(*op1_v, *op2_v);
    if(operator == SUB_OPERATOR){
        int i = -(int)(*op1_v->data);
        v_set(op2_v, &i); // * TO check
        res = op1_v->add(*op1_v, *op2_v);
    }
    if(operator == MUL_OPERATOR)
        res = op1_v->mul(*op1_v, *op2_v);
    if(operator == DIV_OPERATOR)
        res = op1_v->div(*op1_v, *op2_v);
    if(operator == INF_OPERATOR)
        res = op1_v->lesser(*op1_v, *op2_v);
    if(operator == SUP_OPERATOR)
        res = op1_v->greater(*op1_v, *op2_v);
    return res;
}

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
    char* arg;
    for(size_t i=0; i<args->size; i++){
        arg = &l_get(args, i)->data;
    }
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
    if(strcmp(instruction, IF_INSTR)==0){
        dl_check_number_of_children(box_to_execute, 2, 2);
        following_box = dl_instr_if(list_box, box_to_execute, context);
    }
    else if(strcmp(instruction, END_INSTR)==0){
        dl_check_number_of_children(box_to_execute, 0, 0);
        following_box = dl_instr_end(list_box, box_to_execute, context);
    }
    else // * if instruction != IF, it's need only one child
        dl_check_number_of_children(box_to_execute, 1, 1);

    if(strcmp(instruction, FIRST_BOX_CONTENT)==0){
        following_box = dl_instr_first(list_box, box_to_execute, context);
    }
    if(strcmp(instruction, INSTANCE_INSTR)==0){
        following_box = dl_instr_instance(list_box, box_to_execute, context);
    }
    
    if(strcmp(instruction, STORE_INSTR)==0){
        following_box = dl_instr_store(list_box, box_to_execute, context);
    }
    if(strcmp(instruction, CALC_INSTR)==0){
        following_box = dl_instr_calc(list_box, box_to_execute, context);
    }
    if(strcmp(instruction, PRINT_INSTR)==0){
        following_box = dl_instr_print(list_box, box_to_execute, context);
    }
    if(strcmp(instruction, INPUT_INSTR)==0){
        following_box = dl_instr_input(list_box, box_to_execute, context);
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
    list* args = dl_check_number_of_args(INSTANCE_INSTR, box_to_execute, 3, 3);
    char *variable_name = &l_get(args, 1)->data;
    char *operand = &l_get(args, 2)->data;
    variable* var = v_find(variable_name, v_get_main_context());
    if(!var){ // * no variable found
        char error_msg[200];
        sprintf(error_msg, "%s doesn't exist in context\n", variable_name);
        print_error(error_msg, -3, box_to_execute);
    }
    variable* operand_var = dl_calc_operand(operand);
    if(strcmp(operand_var->type_name, var->type_name)!=0){
        char error_msg[200];
        sprintf(error_msg, "You can not store a %s in a %s\n", operand_var->type_name, var->type_name);
        print_error(error_msg, -3, box_to_execute);
    }
    v_set(var, operand_var->data);
    box* following = l_get(box_to_execute->children_list, 0)->data;
    return following;
}
box* dl_instr_calc(list* listbox, box* box_to_execute, execution_context* context){
    list* args = dl_check_number_of_args(INSTANCE_INSTR, box_to_execute, 3, 3);
    char *variable_name = &l_get(args, 1)->data;
    char *expression = &l_get(args, 2)->data;
    variable* var = v_find(variable_name, v_get_main_context());
    if(!var){
        char error_msg[200];
        sprintf(error_msg, "%s doesn't exist in context\n", variable_name);
        print_error(error_msg, -3, box_to_execute);
    }
    variable result = dl_eval_expression(expression);
    if(strcmp(result.type_name, var->type_name)!=0){
        char error_msg[200];
        sprintf(error_msg, "Impossible to store %s in %s\n", result.type_name, var->type_name);
        print_error(error_msg, -3, NULL);
    }
    v_set(var, result.data);
    box* following = l_get(box_to_execute->children_list, 0)->data;
    return following;
}
box* dl_instr_if(list* listbox, box* box_to_execute, execution_context* context){
    list* args = dl_check_number_of_args(INSTANCE_INSTR, box_to_execute, 2, 2);
    char *expression = &l_get(args, 1)->data;
    variable result = dl_eval_expression(expression);
    box* following;
    if((char)*result.data)
        following = l_get(box_to_execute->children_list, 1)->data;
    else
        following = l_get(box_to_execute->children_list, 0)->data;
    return following;
}
box* dl_instr_print(list* listbox, box* box_to_execute, execution_context* context){
    list* args = dl_check_number_of_args(INSTANCE_INSTR, box_to_execute, 2, 2);
    char *content = &l_get(args, 1)->data;
    variable* content_var = dl_calc_operand(content);
    if(strcmp(content_var->type_name, STRING_TYPE)==0)
        u_print_string(content_var->data);
    else if(strcmp(content_var->type_name, FLOAT_TYPE)==0)
        printf("%f", *content_var->data);
    else
        printf("%d", *content_var->data);

    box* following = l_get(box_to_execute->children_list, 0)->data;
    return following;
}
box* dl_instr_input(list* listbox, box* box_to_execute, execution_context* context){
    list* args = dl_check_number_of_args(INSTANCE_INSTR, box_to_execute, 2, 2);
    char *variable_name = &l_get(args, 1)->data;
    variable* var = v_find(variable_name, v_get_main_context());
    if(!var){
        char error_msg[200];
        sprintf(error_msg, "%s doesn't exist in context\n", variable_name);
        print_error(error_msg, -3, box_to_execute);
    }
    if (strcmp(var->type_name, STRING_TYPE)==0)
        scanf("%s", var->data);
    else if (strcmp(var->type_name, CHAR_TYPE)==0)
        scanf("%c", var->data);
    else if (strcmp(var->type_name, FLOAT_TYPE)==0)
        scanf("%f", var->data);
    else if (strcmp(var->type_name, BOOL_TYPE)==0)
        scanf("%d", var->data);
    else if (strcmp(var->type_name, INT_TYPE)==0){
        scanf("%d", var->data);
    }
    box* following = l_get(box_to_execute->children_list, 0)->data;
    return following;
}
box* dl_instr_end(list* listbox, box* box_to_execute, execution_context* context){
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
