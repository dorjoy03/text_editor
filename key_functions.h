#ifndef KEY_FUNCTIONS_H
#define KEY_FUNCTIONS_H		/* So that there is no double inclusion */

#include <ncurses.h>
#include <sys/types.h>
#include "struct_data.h"

extern struct text_data *START;
extern struct text_data *END;
extern struct text_data *CURRENT;
extern struct text_data *WIN_START;

extern int CUR_ROW;
extern int CUR_COL;
extern int MAX_ROW;
extern int MAX_COL;

void screen_update(struct text_data *foo);
struct text_data *prev_line_start(struct text_data *foo);
struct text_data *next_line_start(struct text_data *foo);

void move_one_char_front();
void move_one_char_back();
void move_one_line_front();
void move_one_line_back();
void move_to_line_front();
void move_to_line_end();

#endif
