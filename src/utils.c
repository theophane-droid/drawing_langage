#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

#include "utils.h"
#include "langage.h"
#include "globals.h"

char u_parse_expression(char* expression){
    char c = expression[0];
    for(size_t i=0; c!='\0'; i++){
        if(c==ADD_OPERATOR || c==SUB_OPERATOR || 
        c==MUL_OPERATOR || c==DIV_OPERATOR ||
        c==INF_OPERATOR || c==SUP_OPERATOR){
            expression[i-1]='\0';
            return c;
        }
        c = expression[i];
    }
    return 0;
}

char u_is_a_string(char* string){
    char first_char = string[0];
    char last_char;
    char c = first_char;
    for(size_t i=0; c!='\0'; i++){
        last_char = c;
        c = string[i];
    }
    return first_char=='"' && last_char=='"';
}

char u_is_an_int(char* string){
    char c = string[0];
    for(size_t i=0; c!='\0'; i++){
        if(c<'0' || c>'9')
            return 0;
        c = string[i];
    }
    return 1;
}

char u_is_a_float(char* string){
    char c = string[0];
    for(size_t i=0; c!='\0'; i++){
        if((c<'0' || c>'9') && c!='.')
            return 0;
        c = string[i];
    }
    return 1;
}

char u_return_type(char* string){
    if(u_is_a_string(string))
        return U_STRING;
    if(u_is_an_int(string))
        return U_INT;
    if(u_is_a_float(string))
        return U_FLOAT;
    return U_VARNAME;
}

void u_print_string(char* string){
    char last_char = 10;
    char c = string[0];
    for(size_t i=0; c!='\0'; i++){
        c = string[i];
        if(last_char=='\\' && c=='n')
            putc('\n', stdout);
        else if(last_char=='\\' && c=='t')
            putc('\t', stdout);
        else if(last_char=='\\' && c=='\\')
            putc('\t', stdout);
        else if(c!='\\')
            putc(c, stdout);
        
        last_char = c;
    }
}