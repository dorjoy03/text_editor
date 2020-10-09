#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ncurses.h>
#include <string.h>
#include "struct_data.h"
#include "char_check.h"
#include "key_functions.h"

#define BUF_SIZE 1024

struct text_data *START = NULL;	/* Start of text buffer */
struct text_data *END = NULL;	/* End of text buffer */
struct text_data *CURRENT = NULL; /* 'CURRENT' maps with current cursor position */
struct text_data *WIN_START = NULL; /* current window start pointer in buffer */


int CHANGED_START = 0;		/* If START changes this is set to 1 */
int MAX_ROW = 0;
int MAX_COL = 0;
int CUR_ROW = 0;
int CUR_COL = 0;

void help()
{
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "      ./main filename\n");
    fprintf(stderr, "help: ./main -help\n");
    fprintf(stderr, "Key Commands:\n");
    fprintf(stderr, "             C-f --> Move forward one character\n");
    fprintf(stderr, "             C-b --> Move backward one character\n");
    fprintf(stderr, "             C-a --> Move to the start of current line\n");
    fprintf(stderr, "             C-e --> Move to the end of current line\n");
    fprintf(stderr, "             C-p --> Move to previous line start\n");
    fprintf(stderr, "             C-n --> Move to next line start\n");
    fprintf(stderr, "             C-s --> Save to file\n");
    fprintf(stderr, "             C-k --> Exit\n");
    exit(EXIT_SUCCESS);
}

void error(char *msg)
{
    fprintf(stderr, msg);
    exit(EXIT_SUCCESS);
}

/* This function updates the screen according to the data structure
   starting from foo (which is actually WIN_START) */

void screen_update(struct text_data *foo)
{
    struct text_data *tmp = foo;
    int cur_row, cur_col;
    move(0, 0);
    
    while (tmp != NULL) {
	/* Get current cursor position on screen */
	getyx(stdscr, cur_row, cur_col);	
	if (tmp == CURRENT) {
	    CUR_ROW = cur_row;
	    CUR_COL = cur_col;
	}
	if (cur_row == MAX_ROW - 1) break;
	/* Draw the character on the screen */
	addch(tmp->data);
	tmp = tmp->right;
    }

    /* When CURRENT is NULL cursor is at the end of all text */
    if (CURRENT == NULL) {
	getyx(stdscr, cur_row, cur_col);
	CUR_ROW = cur_row;
	CUR_COL = cur_col;
    }

    int i, j;

    /* The last line of screen is not used */
    for (i = CUR_ROW; i < MAX_ROW; ++i) {
	for (j = CUR_COL; j < MAX_COL; ++j) {
	    addch(' ');
	}
    }
    
}

/* Update the data structure values according to data added or removed */
void buffer_update()
{
    struct text_data *tmp = START;

    int i = 0;
    int j = 0;
    int k = 0;

    while (tmp != NULL) {
	tmp->row = i;
	tmp->col = j;
	
	/* In case of TAB */
	if ((tmp->data ^ 64) == 'I') {
	    int div = j / 8 + 1;
	    int pos = div * 8;
	    if (pos >= MAX_COL - 1) {
		++i;
		j = 0;
	    }
	    else 
		j = pos;
	    k = 1;
	}
	
	/* In case of NEWLINE  */
	if (tmp->data == '\n') {
	    ++i;
	    j = 0;
	    k = 1;
	}

	/* In case of current character position exceeds screen width */
	if (j == MAX_COL - 1) {
	    ++i;
	    j = 0;
	    k = 1;
	}
	tmp = tmp->right;
	if (k == 1)
	    k = 0;
	else
	    ++j;
    }

    return;
}

/* Returns the previous line start character pointer for scrolling effect */
struct text_data *prev_line_start(struct text_data *foo)
{   
    if (foo == NULL) return NULL;
    
    if (foo->row == 0) return START;

    struct text_data *tmp = foo;

    while (!(tmp->row == foo->row - 1 && tmp->col == 0)) {
	tmp = tmp->left;
	if (tmp == NULL)
	    return foo;
    }

    return tmp;
}

/* Returns the next line start character pointer for scrolling effect */
struct text_data *next_line_start(struct text_data *foo)
{
    if (foo == NULL) return NULL;
    struct text_data *tmp = foo;

    while (!(tmp->row == foo->row + 1 && tmp->col == 0)) {
	tmp = tmp->right;
	if (tmp == NULL)
	    return foo;
    }

    return tmp;
}
    
int main(int argc, char *argv[])
{
    int fd;
    char buf[BUF_SIZE];

    if (argc != 2 || (strcmp(argv[1], "-help") == 0))
	help();
    else {
	/* Open or create file accordingly and build the data structure */
	fd = open(argv[1], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
	if (fd == -1) 
	    error("Couldn't open or create file\n");
	else {
	    int cnt = 0;
	    while ((cnt = read(fd, buf, BUF_SIZE)) > 0) {		
		int i;
		for (i = 0; i < cnt; ++i) 
		    add_data(buf[i]);
	    }
	    if (cnt < 0)
		error("Error while reading file\n");
	}
    }

    initscr();			/* Initialize screen */
    raw();			/* Set terminal to raw mode to manipulate all keyboard input directly */
    noecho();			/* Don't echo keyboard inputs */
    keypad(stdscr, TRUE);	/* So that function and arrow keys can be read if needed */
    getmaxyx(stdscr, MAX_ROW, MAX_COL); /* Get screen length and width */
    WIN_START = START;			/* For drawing the initial screen */
    buffer_update();			/* Update the data structure */
    screen_update(WIN_START);		/* Draw to the screen */
    move(0, 0);				/* Move cursor to the beginning of screen */
    CURRENT = START;			/* Set CURRENT synchronous to cursor pos */

    int ch;

    for (;;) {
	ch = getch();
	int v = char_type(ch);	/* Determine input type */
	
	/* This adds data to the data structure and screen */
	if (v == PRINT || v == NEWLINE || v == TAB) {
	    add_data(ch);
	    if (WIN_START == NULL)
		WIN_START = START;
	    buffer_update();
	    /* In case new START is added */
	    if (CHANGED_START) {
		CHANGED_START = 0;
		WIN_START = START;
	    }
	    screen_update(WIN_START);
	    if (CUR_ROW == MAX_ROW - 1) {
		WIN_START = next_line_start(WIN_START);
		screen_update(WIN_START);
	    }
	    move(CUR_ROW, CUR_COL);
	}
	
	/* This deletes data from data structure and screen */
	else if (v == ERASE) {
	    struct text_data *removing;
	    if (CURRENT == NULL)
		removing = END;
	    else
		removing = CURRENT->left;
	    if (removing == NULL) continue;
	    else if (removing->data == '\n')
		WIN_START = prev_line_start(WIN_START);
	    if (removing == WIN_START) {
		if (removing->right == NULL)
		    WIN_START = prev_line_start(WIN_START);
		else 
		    WIN_START = removing->right;
	    }
	    
	    remove_data();	/* Remove from data structure */
	    buffer_update();
	    
	    /* In case START is removed */
	    if (CHANGED_START) {
		CHANGED_START = 0;
		WIN_START = START;
	    }
	    screen_update(WIN_START);	    
	    move(CUR_ROW, CUR_COL);
	}

	/* This writes the current data to the file */
	else if (v == CTRL_S) {
	    struct text_data *tmp = START;
	    ftruncate(fd, 0);
	    lseek(fd, 0, SEEK_SET);
	    int i = 0;
	    while (tmp != NULL) {
		buf[i++] = tmp->data;
		tmp = tmp->right;
		if (i == BUF_SIZE || tmp == NULL) {
		    int s = write(fd, buf, i);
		    if (s < i)
			error("Couldn't write fully\n");
		    i = 0;
		}
	    }
	}
	
	else if (v == CTRL_B) 
	    move_one_char_back();		
	    
	else if (v == CTRL_F) 
	    move_one_char_front();
	
	else if (v == CTRL_P) 
	    move_one_line_back();
	
	else if (v == CTRL_N) 
	    move_one_line_front();
		
	else if (v == CTRL_E) 
	    move_to_line_end();

	else if (v == CTRL_A) 
	    move_to_line_front();
	
	else if (v == CTRL_K) 
	    break;	
    }

    /* Set terminal attributes like before */
    refresh();
    endwin();	     
  
    exit(EXIT_SUCCESS);
}
