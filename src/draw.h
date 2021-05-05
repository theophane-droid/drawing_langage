/* Header of drawing langage utils functions */

#pragma once
#include <stdlib.h>

#include "liste.h"

#define CORNER_DELIMITER '+'
#define ROW_DELIMITER '-'
#define COL_DELIMITER '|'

#define MATRICE

/* return != 0 if the array is containing a box */
char d_isItABox(char** array, size_t nb_row, size_t nb_col);

/* return a list of area delimited by corner (a corner is delimited by CORNER_DELIMITER ) */
list* d_returnCorner(char* file_name);

/* return a file in a list of list */
list* d_read_file(char* file_name);

/* return !=0 if the referenced cell contain a box corner*/
char d_its_left_corner(list* tab, size_t row, size_t col);

/* get the value of a char in the tab, return 0 if nothing found */
char d_matrice_get(list* tab, size_t row, size_t col);