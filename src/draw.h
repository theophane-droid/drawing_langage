/* Header of drawing langage utils functions */

#pragma once
#include <stdlib.h>

#include "liste.h"

#define CORNER_DELIMITER '+'
#define ROW_DELIMITER '-'
#define COL_DELIMITER '|'

#define MATRICE


#define TEXT_S_MAX 200

typedef struct{
    size_t right_up[2];
    size_t left_up[2];
    size_t right_down[2];
    size_t left_down[2];
    char text[TEXT_S_MAX];
    char error; // 1 if present  
} box;

/* return a list box (a box is delimited by CORNER_DELIMITER ) */
list* d_find_all_box(char* file_name);

/* return a file in a list of list */
list* d_read_file(char* file_name);

/* find the box from his left corner (in (row; col) coordinates) */
box d_find_box(list* tab, size_t row, size_t col);

/* return !=0 if the referenced cell contain a box corner*/
char d_its_left_corner(list* tab, size_t row, size_t col);

/* get the value of a char in the tab, return 0 if nothing found */
char d_matrice_get(list* tab, size_t row, size_t col);

/* for debugging purpose only: this func print the content of a box */
void d_print_box(box b);

/* 
* Note about logic:
* d_find_all_box call d_its_left_corner for every cell in the matrix
* if a cell is a left corner the func call d_find_box the find out the others corners
*/