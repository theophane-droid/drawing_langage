#include <stdlib.h>
#include <stdio.h>

#include "draw.h"

#define DEBUG

list* d_returnCorner(char* file_name){
    FILE* f = fopen(file_name, "r");
    list* file_rpz = d_read_file(file_name);
    if(file_rpz){

    }
}

list* d_read_file(char* file_name){
    FILE* f = fopen(file_name, "r");
    list* file_rpz;
    if(f){
        file_rpz = l_init(sizeof(list*));
        list* new_line = l_init(1);
        l_add(file_rpz, &new_line);
        char c = getc(f);
        while(c!=EOF){
            if(c=='\n'){
                new_line = l_init(1);
                l_add(file_rpz, &new_line);
            }
            else{
                l_add(new_line, &c);
            }
            c = getc(f);
        }
    }
    return file_rpz;
}
#ifdef DEBUG

int main(int argc, char** argv){
    if(argc!=2){
        printf("Usage : ./bin filename\n");
        exit(-1);
    }
    list* file_rpz = d_read_file(argv[1]);
    if(file_rpz){
        for(int i=0; i<file_rpz->size; i++){
            list* current = (list*)l_get(file_rpz, i)->data;
            for(int j=0; j<current->size; j++){
                printf("%c", l_get(current, j)->data);
            }
            printf("\n");
        }
        char c = d_matrice_get(file_rpz, 0, 0);
        printf("d get %c \n", c);
        c = d_matrice_get(file_rpz, 10, 10);
        printf("d get %c \n", c);
        c = d_matrice_get(file_rpz, 1, 10);
        printf("d get %c \n", c);
        c = d_its_left_corner(file_rpz, 0, 0);
        if(c)
            printf("its left corner !\n");
        l_free(file_rpz);
    }
    return 0;
}
#endif

char d_its_left_corner(list* tab, size_t row, size_t col){
    char c1 = d_matrice_get(tab, row, col);
    char c2 = d_matrice_get(tab, row, col+1);
    char c3 = d_matrice_get(tab, row+1, col);
    return c1==CORNER_DELIMITER && c2==ROW_DELIMITER && c3==COL_DELIMITER;
}
char d_matrice_get(list* tab, size_t row, size_t col){
    char c = 0;
    cell* row_cell = (list*)l_get(tab, row);
    if(row_cell!=NULL){
        list* row_list = (list*)l_get(tab, row)->data;
        cell* col_cell = l_get(row_list, col);
        if(col_cell)
            c = col_cell->data;
    }
    return c;
}
