#ifndef STRUCT_DATA_H
#define STRUCT_DATA_H		/* So that there is no double inclusion */

#include <sys/types.h>
#include <stdlib.h>
#include <ncurses.h>

extern struct text_data *START;
extern struct text_data *END;
extern struct text_data *CURRENT;
extern struct text_data *WIN_START;
extern int CHANGED_START;

/* Data structure for doubly linked list */
struct text_data {
    char data;		       	/* Character value */
    int row;			/* Line number */
    int col; 			/* Column number */
    struct text_data *right;	/* Right pointer */
    struct text_data *left;	/* Left pointer */
};


void add_data(char ch);
void remove_data();

#endif
