/* 
* Header of the drawing part of the drawing language, 
* there is every functions aimed to parse the file 
*/

#ifndef DRAW_H
#define DRAW_H
#include <stdlib.h>

#include "liste.h"
#include "globals.h"

#define TEXT_S_MAX 200

typedef struct {
    size_t row;
    size_t col;
} coord;

typedef struct{
    size_t right_up[2];
    size_t left_up[2];
    size_t right_down[2];
    size_t left_down[2];
    char text[TEXT_S_MAX];
    char error; // 1 if present
    list* children_list;
} box;

typedef struct{
    char sign;
    int current_coord[2];
    int last_coord[2];
} connection;


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

/*extract the text contained in a box*/
char* d_extract_text_from_box(list* tab, box* box_);

/* return a list of the beginning connection around the box */
void d_find_beg_connection_from_box(list* tab, box box_, list* beg_connection);

/* find all the beginning connection around the boxes listed in list_box */
void d_find_all_beg_connection(list* tab, list* list_box);

/* find the box that the connection relied to */
box* d_follow_connection(list* tab, list* list_box, connection conn);

/* return != 0 if (row; col) is in the perimeter of box */ 
char d_is_in_the_perimeter(box b, size_t row, size_t col);

/* find the box in which (row; col) belongs to the perimeter */
box* d_find_the_box_in_the_perimeter(list* list_box, size_t row, size_t col);


/* 
* Note about logic:
* d_find_all_box call d_its_left_corner for every cell in the matrix
* if a cell is a left corner the func call d_find_box the find out the others corners
*/
#endif