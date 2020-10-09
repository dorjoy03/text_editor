#include "key_functions.h"

/* Move the cursor one character back and change CURRENT accordingly */
void move_one_char_back()
{
    if (CURRENT == NULL)
	CURRENT = END;
    else if (CURRENT->left != NULL)
	CURRENT = CURRENT->left;
    if (CURRENT != NULL && CURRENT->row < WIN_START->row) 
	WIN_START = prev_line_start(WIN_START);
    
    screen_update(WIN_START);
    move(CUR_ROW, CUR_COL);		
}

/* Move the cursor one character forward and change CURRENT accordingly */
void move_one_char_front()
{
    if (CURRENT != NULL)
	CURRENT = CURRENT->right;
    screen_update(WIN_START);
    if (CUR_ROW == MAX_ROW - 1) 
	WIN_START = next_line_start(WIN_START);

    screen_update(WIN_START);
    move(CUR_ROW, CUR_COL);
}

/* Move cursor to the end of current line and change CURRENT accordingly */
void move_to_line_end()
{
    if (CURRENT == NULL)
	return;
    struct text_data *tmp = CURRENT;
    while (!(tmp->data == '\n' || tmp->col == MAX_COL - 1)) {
	tmp = tmp->right;
	if (tmp == NULL)
	    break;
    }
    
    CURRENT = tmp;
    screen_update(WIN_START);
    move(CUR_ROW, CUR_COL);
}

/* Move cursor to the start of current line and change CURRENT accordingly */
void move_to_line_front()
{
    struct text_data *tmp = CURRENT;
    if (CURRENT == NULL)
	tmp = END;
    if (tmp == NULL)
	return;
    while (tmp->col != 0) {
	if (tmp->left == NULL)
	    break;
	tmp = tmp->left;
    }
    
    CURRENT = tmp;
    screen_update(WIN_START);
    move(CUR_ROW, CUR_COL);
}

/* Move cursor to the beginning of previous line and change CURRENT accordingly */
void move_one_line_back()
{
    if (CURRENT == NULL) {
	if (END != NULL) {
	    struct text_data *tmp = END;
	    while (!(tmp->row == END->row && tmp->col == 0)) {
		if (tmp->left == NULL)
		    break;
		tmp = tmp->left;
	    }
	    CURRENT = tmp;
	}
    }
    else
	CURRENT = prev_line_start(CURRENT);
    if (CURRENT != NULL && CURRENT->row < WIN_START->row)
	WIN_START = prev_line_start(WIN_START);
    
    screen_update(WIN_START);
    move(CUR_ROW, CUR_COL);
}

/* Move cursor to the end of next line and change CURRENT accordingly
   This is the same as doing C-e C-f */
void move_one_line_front()
{
    move_to_line_end();
    move_one_char_front();
}

/* Many other key bindings can be added here if needed */
