#include "struct_data.h"

/* Add data function for the doubly linked list */
void add_data(char ch)
{
    struct text_data *tmp;

    /* Allocate memory for data */
    tmp = (struct text_data *) malloc(sizeof(struct text_data));
    tmp->data = ch;

    /* If there is no data */
    if (START == NULL) {
	tmp->right = NULL;
	tmp->left = NULL;
	START = tmp;
	END = tmp;
    }
    /* If there is no data at the current cursor position
       add data to END of list */
    else if (CURRENT == NULL) {
	END->right = tmp;
	tmp->left = END;
	tmp->right = NULL;
	END = tmp;
    }
    /* Add data to the left of START and change START */
    else if (CURRENT == START) {
	CHANGED_START = 1;
	START = tmp;
	CURRENT->left = tmp;
	tmp->right = CURRENT;
	tmp->left = NULL;
    }
    /* If there are data on both sides of CURRENT */
    else {
	tmp->right = CURRENT;
	tmp->left = CURRENT->left;
	
	(CURRENT->left)->right = tmp;
	CURRENT->left = tmp;
    }

    return;
}

/* Remove data from the doubly linked list */
void remove_data()
{
    struct text_data *tofree;

    if (CURRENT == NULL)
	tofree = END;
    else
	tofree = CURRENT->left;

    if (tofree == NULL) return;

    if (tofree->left != NULL)
	(tofree->left)->right = tofree->right;
    else {
	START = tofree->right;
	CHANGED_START = 1;
	if (START != NULL) START->left = NULL;
    }
    if (tofree->right != NULL)
	(tofree->right)->left = tofree->left;
    else {
	END = tofree->left;
	if (END != NULL) END->right = NULL;
    }

    /* Free the memory allocated */
    free(tofree);
    return;
}
	
