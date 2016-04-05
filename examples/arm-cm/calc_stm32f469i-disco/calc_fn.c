#include "calc_fn.h"

#define DISP_WIDTH  9

static char l_display[DISP_WIDTH + 1]; /* the calculator display */
static int  l_len;  /* number of displayed characters */

/*..........................................................................*/
void BSP_clear(void) {
    memset(l_display, ' ', DISP_WIDTH - 1);
    l_display[DISP_WIDTH - 1] = '0';
    l_display[DISP_WIDTH] = '\0';
    l_len = 0;
}
/*..........................................................................*/
void BSP_insert(int keyId) {
    if (l_len == 0) {
        l_display[DISP_WIDTH - 1] = (char)keyId;
        ++l_len;
    }
    else if (l_len < (DISP_WIDTH - 1)) {
        memmove(&l_display[0], &l_display[1], DISP_WIDTH - 1);
        l_display[DISP_WIDTH - 1] = (char)keyId;
        ++l_len;
    }
}
/*..........................................................................*/
void BSP_negate(void) {
    BSP_clear();
    l_display[DISP_WIDTH - 2] = '-';
}
/*..........................................................................*/
void BSP_display(void) {
    BSP_LCD_DisplayStringAt((uint16_t)0, (uint16_t)120, (uint8_t *)l_display, RIGHT_MODE);
}
/*..........................................................................*/
void BSP_exit(void) {
    printf("\nBye! Bye!");
    fflush(stdout);
    exit(0);
}
/*..........................................................................*/
float BSP_get_value(void) {
    return strtod(l_display, (char **)0);
}
/*..........................................................................*/
int BSP_eval(float operand1, int oper, float operand2) {
    int ok = 1;
    float result = 0.0;
    switch (oper) {
        case KEY_ADD: {
            result = operand1 + operand2;
            break;
        }
        case KEY_SUB: {
            result = operand1 - operand2;
            break;
        }
        case KEY_MUL: {
            result = operand1 * operand2;
            break;
        }
        case KEY_DIV: {
            if ((operand2 < -1e-30) || (1e-30 < operand2)) {
                result = operand1 / operand2;
            }
            else {
                strcpy(l_display, " Error 0 "); /* error: divide by zero */
                ok = 0;
            }
            break;
        }
    }
    if (ok) {
        if ((-0.000001 < result) && (result < 0.000001)) {
            result = 0.0;
        }
        if ((-99999999.0 < result) && (result < 99999999.0)) {
            /* To prevent a negative result from exceeding the printable width because of the sign */
            if (result < 0) {
                sprintf(l_display, "%9.3g", result);
            }
            else {
                sprintf(l_display, "%9.4g", result); //Default: %9.6g
            }
        }
        else {
            strcpy(l_display, " Error 1 ");     /* error: out of range */
            ok = 0;
        }
    }
    return ok;
}
/*..........................................................................*/
void BSP_message(char const *msg) {
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillRect((uint16_t)0, (uint16_t)(24 * 3), (uint16_t)(17 * 16), (uint16_t)24);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DisplayStringAt((uint16_t)0, (uint16_t)(24 * 3), (uint8_t *)msg, LEFT_MODE);
}
/*..........................................................................*/