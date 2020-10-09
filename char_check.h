#ifndef CHAR_CHECK_H
#define CHAR_CHECK_H		/* So that there is no double inclusion */

#include <sys/types.h>

/* To differentiate different types of keyboard inputs */
enum RET { PRINT, ERASE, NEWLINE, TAB, CTRL_A, CTRL_E, CTRL_N, CTRL_P,
	   CTRL_F, CTRL_B, CTRL_S, CTRL_K, OTHER };

enum RET char_type(int ch);

#endif

