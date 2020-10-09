#include <ctype.h>
#include "char_check.h"		/* Declares functions in this file */

/* Return character type */
enum RET char_type(int ch)
{
    enum RET val = OTHER;
    
    if (isprint(ch))
	val = PRINT;

    if (ch == 263)
	val = ERASE;

    if (iscntrl(ch)) {
	int check;
	check = ch ^ 64;

	switch (check) {
	case 'J':
	    val = NEWLINE;
	    break;	    
	case 'I':
	    val = TAB;
	    break;
	case 'A':
	    val = CTRL_A;
	    break;
	case 'B':
	    val = CTRL_B;
	    break;
	case 'E':
	    val = CTRL_E;
	    break;
	case 'F':
	    val = CTRL_F;
	    break;
	case 'K':
	    val = CTRL_K;
	    break;
	case 'N':
	    val = CTRL_N;
	    break;
	case 'P':
	    val = CTRL_P;	    
	    break;
	case 'S':
	    val = CTRL_S;
	    break;
	default:
	    break;
	}
    }

    return val;
}
