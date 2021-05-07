#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "variable.h"
#include "error_handling.h"

/* * current context */
execution_context main_context;

/* * anonymous context */
execution_context anonymous_context;


variable v_operation_not_allowed(variable v1, variable v2){
    char error_string[200];
    sprintf(error_string, "operation not allowed with %s type\n", v1.type_name);
    print_error(error_string, -3, NULL);
}

variable v_add_number(variable v1, variable v2){
    if(v1.size==1)
        *v1.data = (int8_t)*(v1.data) + (int8_t)*(v2.data);
    else if(v1.size==2)
        *v1.data = (int16_t)*(v1.data) + (int16_t)*(v2.data);
    else if(v1.size==4)
        *v1.data = (int32_t)*(v1.data) + (int32_t)*(v2.data);
    else if(v1.size==8)
        *v1.data = (int64_t)*(v1.data) + (int64_t)*(v2.data);
    else 
        return v_operation_not_allowed(v1, v2);
    return v1;
}

variable v_mul_number(variable v1, variable v2){
     if(v1.size==1)
        *v1.data = (int8_t)*(v1.data) * (int8_t)*(v2.data);
    else if(v1.size==2)
        *v1.data = (int16_t)*(v1.data) * (int16_t)*(v2.data);
    else if(v1.size==4)
        *v1.data = (int32_t)*(v1.data) * (int32_t)*(v2.data);
    else if(v1.size==8)
        *v1.data = (int64_t)*(v1.data) * (int64_t)*(v2.data);
    else 
        return v_operation_not_allowed(v1, v2);
    return v1;
}

variable v_div_number(variable v1, variable v2){
    if(v1.size==1)
        *v1.data = (int8_t)*(v1.data) / (int8_t)*(v2.data);
    else if(v1.size==2)
        *v1.data = (int16_t)*(v1.data) / (int16_t)*(v2.data);
    else if(v1.size==4)
        *v1.data = (int32_t)*(v1.data) / (int32_t)*(v2.data);
    else if(v1.size==8)
        *v1.data = (int64_t)*(v1.data) / (int64_t)*(v2.data);
    else 
        return v_operation_not_allowed(v1, v2);
    return v1;
}

variable v_equal_number(variable v1, variable v2){
    variable bool_var = *v_init("temp_bool", BOOL_TYPE, &anonymous_context);
    if(v1.size==1)
        *bool_var.data = (int8_t)*(v1.data) == (int8_t)*(v2.data);
    else if(v1.size==2)
        *bool_var.data = (int16_t)*(v1.data) == (int16_t)*(v2.data);
    else if(v1.size==4)
        *bool_var.data = (int32_t)*(v1.data) == (int32_t)*(v2.data);
    else if(v1.size==8)
        *bool_var.data = (int64_t)*(v1.data) == (int64_t)*(v2.data);
    else 
        return v_operation_not_allowed(v1, v2);
    return bool_var;
}

variable v_lesser_number(variable v1, variable v2){
    variable bool_var = *v_init("temp_bool", BOOL_TYPE, &anonymous_context);
    if(v1.size==1)
        *bool_var.data = (int8_t)*(v1.data) < (int8_t)*(v2.data);
    else if(v1.size==2)
        *bool_var.data = (int16_t)*(v1.data) < (int16_t)*(v2.data);
    else if(v1.size==4)
        *bool_var.data = (int32_t)*(v1.data) < (int32_t)*(v2.data);
    else if(v1.size==8)
        *bool_var.data = (int64_t)*(v1.data) < (int64_t)*(v2.data);
    else 
        return v_operation_not_allowed(v1, v2);
    return bool_var;
}

variable v_greater_number(variable v1, variable v2){
    variable bool_var = *v_init("temp_bool", BOOL_TYPE, &anonymous_context);
    if(v1.size==1)
        *bool_var.data = (int8_t)*(v1.data) > (int8_t)*(v2.data);
    else if(v1.size==2)
        *bool_var.data = (int16_t)*(v1.data) > (int16_t)*(v2.data);
    else if(v1.size==4)
        *bool_var.data = (int32_t)*(v1.data) > (int32_t)*(v2.data);
    else if(v1.size==8)
        *bool_var.data = (int64_t)*(v1.data) > (int64_t)*(v2.data);
    else 
        return v_operation_not_allowed(v1, v2);
    return bool_var;
}

void v_set(variable* v, void* data){
    memcpy(v->data, data, v->size);
}

variable* v_init(char* variable_name, char* data_type, execution_context* variable_context){
    if(!variable_context) // * if no context we set it at anonymous context
        variable_context = &anonymous_context;
    if(!variable_context->list_variable) // if list not defined we int the list
        variable_context->list_variable = l_init(sizeof(variable*));
    if(variable_context != &anonymous_context && v_check_if_already_defined(variable_name, variable_context)){
        char error_message[200];
        sprintf(error_message, "error : \"%s\" variable already defined in context\n", variable_name);
        print_error(error_message, -2, NULL);
    }
    variable* v = malloc(sizeof(variable));
    if(strcmp(data_type, CHAR_TYPE)==0){
        v->data = malloc(CHAR_SIZE);
        v->add = v_add_number;
        v->mul= v_mul_number;
        v->div = v_div_number;
        v->equal = v_equal_number;
        v->lesser = v_lesser_number;
        v->greater = v_greater_number;
        sprintf(v->type_name,"%s", CHAR_TYPE);
        v->size=CHAR_SIZE;
    }
    if(strcmp(data_type, BOOL_TYPE)==0){
        v->data = malloc(BOOL_SIZE);
        v->add = v_add_number;
        v->mul= v_mul_number;
        v->div = v_div_number;
        v->equal = v_equal_number;
        v->lesser = v_lesser_number;
        v->greater = v_greater_number;
        sprintf(v->type_name,"%s", BOOL_TYPE);
        v->size=BOOL_SIZE;
    }
    else if(strcmp(data_type, INT_TYPE)==0){
        v->data = malloc(INT_SIZE);
        v->add = v_add_number;
        v->mul= v_mul_number;
        v->div = v_div_number;
        v->equal = v_equal_number;
        v->lesser = v_lesser_number;
        v->greater = v_greater_number;
        sprintf(v->type_name,"%s", INT_TYPE);
        v->size=INT_SIZE;
    }
    else if(strcmp(data_type, FLOAT_TYPE)==0){
        v->data = malloc(FLOAT_SIZE);
        v->add = v_add_number;
        v->mul= v_mul_number;
        v->div = v_div_number;
        v->equal = v_equal_number;
        v->lesser = v_lesser_number;
        v->greater = v_greater_number;
        sprintf(v->type_name,"%s", FLOAT_TYPE);
        v->size=FLOAT_SIZE;
    }
    else if(strcmp(data_type, STRING_TYPE)==0){
        v->data = malloc(STRING_SIZE);
        v->add = v_operation_not_allowed;
        v->mul= v_operation_not_allowed;
        v->div = v_operation_not_allowed;
        v->equal = v_operation_not_allowed;
        v->lesser = v_operation_not_allowed;
        v->greater = v_operation_not_allowed;
       sprintf(v->type_name,"%s", STRING_TYPE);
        v->size=STRING_SIZE;
    }
    else{
        char error_msg [200];
        sprintf(error_msg, "%s type doesn't exist\n", data_type);
        print_error(error_msg, -2, NULL);
    }
    sprintf(v->variable_name,"%s", variable_name);
    l_add(variable_context->list_variable, &v);
    return v;
}

char v_check_if_already_defined(char* variable_name, execution_context* context){
   return v_find(variable_name, context)!=NULL;
}

variable* v_find(char* variable_name, execution_context* context){
    variable* v;
    if(context->list_variable){
        for(size_t i=0; i<context->list_variable->size; i++){
            v = l_get(context->list_variable, i)->data;
            if(strcmp(v->variable_name, variable_name)==0)
                return v;
        }
    }
    return NULL;
}

execution_context* v_get_anonymous_context(){
    return &anonymous_context;
}
execution_context* v_get_main_context(){
    return &main_context;
}

#ifdef DEBUG
int main(){
    variable* v1 = v_init("variable1", "int", &main_context);
    variable* v2 = v_init("variable2", "int", &main_context);
    variable* v3 = v_init("variable3", "int", &main_context);
    printf("after init \n");
    int i1 = 6, i2 = 2;
    v_set(v1, &i1);
    v_set(v2, &i2);
    v_set(v3, &i2);
    printf("after set\n");
    variable add = v1->add(*v1, *v2);
    variable mul = v1->mul(*v1, *v2);
    variable div = v1->div(*v1, *v2);
    variable equal_false = v1->equal(*v2, *v1);
    variable equal_true = v1->equal(*v2, *v3);
    variable inf = v1->lesser(*v1, *v2);
    variable greater = v1->greater(*v1, *v2);

    printf("v1 = %d\n", (int)(*(v1->data)));
    printf("v2 = %d\n", (int)(*(v2->data)));
    printf("sum = %d\n", (int)*(add.data));
    printf("mul = %d\n", (int)*(mul.data));
    printf("div = %d\n", (int)*(div.data));
    printf("equal false = %d\n", (char)*(equal_false.data));
    printf("equal true = %d\n", (char)*(equal_true.data));
    printf("inf = %d\n", (char)*(inf.data));
    printf("greater = %d\n", (char)*(greater.data));
    printf("greater type = %s\n", greater.type_name);
    printf("sum type = %s\n", add.type_name);
    printf("sum name = %s\n", add.variable_name);
}
#endif DEBUG