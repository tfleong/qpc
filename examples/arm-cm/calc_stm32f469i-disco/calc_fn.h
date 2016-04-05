#ifndef calc_fn_h
#define calc_fn_h

#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_ADD '+'
#define KEY_SUB '-'
#define KEY_MUL '*'
#define KEY_DIV '/'

void BSP_clear (void);
void BSP_negate(void);
void BSP_insert(int keyId);
float BSP_get_value(void);
int  BSP_eval(float operand1, int oper, float operand2);
void BSP_exit(void);

void BSP_display(void);
void BSP_message(char const *state);

#endif /* calc_fn_h */