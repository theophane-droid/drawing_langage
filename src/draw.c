#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "draw.h"

#define DEBUG

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
list* d_find_all_box(char* file_name){
    list* liste_box = l_init(sizeof(box));
    list* file_rpz = d_read_file(file_name);
    for(size_t i=0; i<file_rpz->size; i++){
        list* row = l_get(file_rpz, i)->data;
        for(size_t j=0; j<row->size; j++){
            if(d_its_left_corner(file_rpz, i, j)){
                box b = d_find_box(file_rpz, i, j);
                if(!b.error)
                    l_add(liste_box, &b);
                
            }
        }
    }
    return liste_box;
}
box d_find_box(list* tab, size_t row, size_t col){
    box actual_box;
    actual_box.error = 0; //* no error for now
    actual_box.left_up[0] = row;
    actual_box.left_up[1] = col;
    size_t col_ = col+1, row_ = row+1;
    // * first find the right up corner
    char c=ROW_DELIMITER;
    for(; c==ROW_DELIMITER; col_+=1){
        c = d_matrice_get(tab, row, col_);
    }
    if(c!=CORNER_DELIMITER)
        actual_box.error = 1;
    col_-=1; // ! quick fix 
    actual_box.right_up[0] = row;
    actual_box.right_up[1] = col_;

    // * then find the left low corner
    c=COL_DELIMITER;
    for(; c==COL_DELIMITER; row_+=1){
        c = d_matrice_get(tab, row_, col);
    }
    if(c!=CORNER_DELIMITER)
        actual_box.error = 2;
    row_-=1; // ! quick fix
    actual_box.left_down[0] = row_;
    actual_box.left_down[1] = col;

    // * let's calcultate the right low corner
    actual_box.right_down[0] = actual_box.left_down[0];
    actual_box.right_down[1] = actual_box.right_up[1];
    c = d_matrice_get(tab, actual_box.right_down[0], actual_box.right_down[1]);
    if(c!=CORNER_DELIMITER)
        actual_box.error = 3;

    //* to finish we extract the text from the box
    d_extract_text_from_box(tab, &actual_box);
    return actual_box;
}
void d_print_box(box b){
    printf("left up corner (%d,%d)\n", b.left_up[0], b.left_up[1]);
    printf("right up corner (%d,%d)\n", b.right_up[0], b.right_up[1]);
    printf("left down corner (%d,%d)\n", b.left_down[0], b.left_down[1]);
    printf("right down corner (%d,%d)\n", b.right_down[0], b.right_down[1]);
    printf("text in the box : %s\n", b.text);
    printf("error (%d)\n", (int)b.error);
}
char* d_extract_text_from_box(list* tab, box* box_){
    printf("extract text\n");
    char text[TEXT_S_MAX];
    size_t char_counter = 0;
    for(size_t row=box_->left_up[0]+1; row<box_->left_down[0]; row++){
        for(size_t col=box_->left_up[1]+1; col<box_->right_up[1]; col++){
            char c = d_matrice_get(tab, row, col);
            printf("char extracted = %c\n", c);
            if(c!=' ' && char_counter<TEXT_S_MAX-1){
                text[char_counter] = c;
                char_counter++;
            }
        }
    }
    text[char_counter] = 0;
    strncpy(box_->text, text, TEXT_S_MAX);
}
void d_find_beg_connection_from_box(list* tab, box box_, list* beg_connection){
    // * first line above the box
    char c;
    connection conn;
    int col_=box_.left_up[1]-1;
    for(int col=box_.left_up[1]-1; col<=(int)box_.right_up[1]+1; col++){
        c = d_matrice_get(tab, box_.left_up[0]-1, col);
        if(c==COL_DELIMITER){
            conn.current_coord[0] = box_.left_up[0]-1;
            conn.current_coord[1] = col;
            conn.last_coord[0] = box_.left_up[0];
            conn.last_coord[1] = col;
            conn.sign = COL_DELIMITER;
            l_add(beg_connection, &conn);
        }
    }

    // * second line to the left to the box
    for(int row=box_.left_up[0]-1; row<=box_.left_down[0]+1; row++){
        c = d_matrice_get(tab, row, box_.left_up[1]-1);
        if(c==ROW_DELIMITER){
            conn.current_coord[0] = row;
            conn.current_coord[1] = box_.left_up[1]-1;
            conn.last_coord[0] = row;
            conn.last_coord[1] = box_.left_up[1];
            conn.sign = ROW_DELIMITER;
            l_add(beg_connection, &conn);
        }
    }

    // * third line to the right to the box
    for(int row=box_.right_up[0]-1; row<=box_.left_down[0]+1; row++){
        c = d_matrice_get(tab, row, box_.right_up[1]+1);
        if(c==ROW_DELIMITER){
            conn.current_coord[0] = row;
            conn.current_coord[1] = box_.right_up[1]+1;
            conn.last_coord[0] = row;
            conn.last_coord[1] = box_.right_up[1];
            conn.sign = ROW_DELIMITER;
            l_add(beg_connection, &conn);
        }
    }

    // * fourth line to the below the box
    for(int col=box_.left_down[1]-1; col<=(int)box_.right_down[1]+1; col++){
        c = d_matrice_get(tab, box_.left_down[0]+1, col);
        if(c==COL_DELIMITER){
            conn.current_coord[0] = box_.left_down[0]+1;
            conn.current_coord[1] = col;
            conn.last_coord[0] = box_.left_down[0];
            conn.last_coord[1] = col;
            conn.sign = COL_DELIMITER;
            l_add(beg_connection, &conn);
        }
    }

}
list* d_find_all_beg_connection(list* tab, list* list_box){
    list* list_all_connection = l_init(sizeof(connection));
    for(size_t i = 0; i<list_box->size; i++){
        box* b = (box*)&l_get(list_box, i)->data;
        d_find_beg_connection_from_box(tab, *b, list_all_connection);
    }
    return list_all_connection;
}
box* d_follow_connection(list* tab, list* list_box, connection conn){ // TODO: test this func
    char current_char = d_matrice_get(tab, conn.current_coord[0], conn.current_coord[1]);
    if(current_char!=ROW_DELIMITER && current_char!=COL_DELIMITER){
        if(current_char==ARROW_DOWN) {
            return d_find_the_box_in_the_perimiter(list_box, conn.current_coord[0]+1, conn.current_coord[1]);
        }
        if(current_char==ARROW_UP){
            return d_find_the_box_in_the_perimiter(list_box, conn.current_coord[0]-1, conn.current_coord[1]);
        } 
        if(current_char==ARROW_RIGHT){
            return d_find_the_box_in_the_perimiter(list_box, conn.current_coord[0], conn.current_coord[1]+1);
        } 
        if(current_char==ARROW_LEFT){
            return d_find_the_box_in_the_perimiter(list_box, conn.current_coord[0], conn.current_coord[1]-1);
        }
        else{
            return NULL;
        }
    }
    char save_row = conn.last_coord[0];
    char save_col = conn.last_coord[1];
    conn.last_coord[0] = conn.current_coord[0];
    conn.last_coord[1] = conn.current_coord[1];
    if(conn.sign == COL_DELIMITER){
        if(save_row<conn.current_coord[0])
            conn.current_coord[0]+=1;
        else
            conn.current_coord[0]-=1;
    }
    else{
        if(save_col<conn.current_coord[1])
            conn.current_coord[1]+=1;
        else
            conn.current_coord[1]-=1;
    }
    return d_follow_connection(tab, list_box, conn);
}
char d_is_in_the_perimeter(box b, size_t row, size_t col){
    // * check the lower line
    if(b.left_down[0] == row && col>=b.left_down[1] && col<=b.right_down[1])
        return 1;
    // * check the upper line
    if(b.left_up[0] == row && col>=b.left_up[1] && col<=b.right_up[1])
        return 2;
    // * check the right line
    if(b.right_up[1] == col && row>=b.right_up[0] && row<=b.right_down[0])
        return 3;
    // * check the left line
    if(b.left_up[1] == col && row>=b.left_up[0] && row<=b.left_down[0])
        return 4;
    return 0;
}
box* d_find_the_box_in_the_perimeter(list* list_box, size_t row, size_t col){
    printf("func 1\n");
    for(size_t i=0; i<list_box->size; i++){
        box* b = (box*)&l_get(list_box, i)->data;
        if(d_is_in_the_perimeter(*b, row, col))
            return b;
    }
    return NULL;
};
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

        box b = d_find_box(file_rpz, 0, 0);
        d_print_box(b);

        list* list_box = d_find_all_box(argv[1]);
        for(size_t i; i<list_box->size; i++){
            printf("\n****box number %d****\n", i);
            box* b = (box*)&l_get(list_box, i)->data;
            d_print_box(*b);
        }

        list* connection_liste = d_find_all_beg_connection(file_rpz, list_box);
        for(int i=0; i<connection_liste->size; i++){
            ;
            connection* conn = &l_get(connection_liste, i)->data;
            printf("connection %d : (%d; %d)\n", i, conn->current_coord[0], conn->current_coord[1]);
        }
        printf("the box in the perimeter : \n");
        box* b2 = d_find_the_box_in_the_perimeter(list_box, 4, 8);
        printf("b2 = %p\n", b2);
        d_print_box(*b2);
    }
    return 0;
}
#endif