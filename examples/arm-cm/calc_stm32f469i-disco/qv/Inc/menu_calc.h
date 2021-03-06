#ifndef __MENU_CALC_H
#define __MENU_CALC_H

#include "main.h"

#define LCD_PIXEL_X 800
#define LCD_PIXEL_Y 480

#define GRID_DIVISION_H 120
#define GRID_DIVISION_V 160

#define KEY_OPERAND_RADIUS 55
#define KEY_OPERAND_RADIUS_COS_45 39

#define KEY_OPERATOR_RADIUS 45
#define KEY_OPERATOR_RADIUS_COS_45 32

#define KEY_7_CENTER_X 60
#define KEY_8_CENTER_X (KEY_7_CENTER_X + GRID_DIVISION_H)
#define KEY_9_CENTER_X (KEY_7_CENTER_X + GRID_DIVISION_H * 2)
#define KEY_C_CENTER_X (KEY_7_CENTER_X + GRID_DIVISION_H * 3)

#define KEY_7_CENTER_Y (GRID_DIVISION_V + 80)
#define KEY_4_CENTER_Y (GRID_DIVISION_V * 2 + 80)
#define KEY_1_CENTER_Y (GRID_DIVISION_V * 3 + 80)
#define KEY_0_CENTER_Y (GRID_DIVISION_V * 4 + 80)

#define KEY_C_CENTER_Y (GRID_DIVISION_V + 53)
#define KEY_CE_CENTER_Y (GRID_DIVISION_V + 53 * 3)
#define KEY_DIV_CENTER_Y (GRID_DIVISION_V + 53 * 5)
#define KEY_MUL_CENTER_Y (GRID_DIVISION_V + 53 * 7)
#define KEY_SUB_CENTER_Y (GRID_DIVISION_V + 53 * 9)
#define KEY_ADD_CENTER_Y (GRID_DIVISION_V + 53 * 11)

#define KEY_7_X_MIN (KEY_7_CENTER_X - KEY_OPERAND_RADIUS_COS_45)
#define KEY_7_Y_MIN (KEY_7_CENTER_Y - KEY_OPERAND_RADIUS_COS_45)
#define KEY_7_X_MAX (KEY_7_CENTER_X + KEY_OPERAND_RADIUS_COS_45)
#define KEY_7_Y_MAX (KEY_7_CENTER_Y + KEY_OPERAND_RADIUS_COS_45)

#define KEY_8_X_MIN (KEY_8_CENTER_X - KEY_OPERAND_RADIUS_COS_45)
#define KEY_8_Y_MIN KEY_7_Y_MIN
#define KEY_8_X_MAX (KEY_8_CENTER_X + KEY_OPERAND_RADIUS_COS_45)
#define KEY_8_Y_MAX KEY_7_Y_MAX

#define KEY_9_X_MIN (KEY_9_CENTER_X - KEY_OPERAND_RADIUS_COS_45)
#define KEY_9_Y_MIN KEY_7_Y_MIN
#define KEY_9_X_MAX (KEY_9_CENTER_X + KEY_OPERAND_RADIUS_COS_45)
#define KEY_9_Y_MAX KEY_7_Y_MAX

#define KEY_4_X_MIN KEY_7_X_MIN
#define KEY_4_Y_MIN (KEY_4_CENTER_Y - KEY_OPERAND_RADIUS_COS_45)
#define KEY_4_X_MAX KEY_7_X_MAX
#define KEY_4_Y_MAX (KEY_4_CENTER_Y + KEY_OPERAND_RADIUS_COS_45)

#define KEY_5_X_MIN KEY_8_X_MIN
#define KEY_5_Y_MIN KEY_4_Y_MIN
#define KEY_5_X_MAX KEY_8_X_MAX
#define KEY_5_Y_MAX KEY_4_Y_MAX

#define KEY_6_X_MIN KEY_9_X_MIN
#define KEY_6_Y_MIN KEY_4_Y_MIN
#define KEY_6_X_MAX KEY_9_X_MAX
#define KEY_6_Y_MAX KEY_4_Y_MAX

#define KEY_1_X_MIN KEY_7_X_MIN
#define KEY_1_Y_MIN (KEY_1_CENTER_Y - KEY_OPERAND_RADIUS_COS_45)
#define KEY_1_X_MAX KEY_7_X_MAX
#define KEY_1_Y_MAX (KEY_1_CENTER_Y + KEY_OPERAND_RADIUS_COS_45)

#define KEY_2_X_MIN KEY_8_X_MIN
#define KEY_2_Y_MIN KEY_1_Y_MIN
#define KEY_2_X_MAX KEY_8_X_MAX
#define KEY_2_Y_MAX KEY_1_Y_MAX

#define KEY_3_X_MIN KEY_9_X_MIN
#define KEY_3_Y_MIN KEY_1_Y_MIN
#define KEY_3_X_MAX KEY_9_X_MAX
#define KEY_3_Y_MAX KEY_1_Y_MAX

#define KEY_0_X_MIN KEY_7_X_MIN
#define KEY_0_Y_MIN (KEY_0_CENTER_Y - KEY_OPERAND_RADIUS_COS_45)
#define KEY_0_X_MAX KEY_7_X_MAX
#define KEY_0_Y_MAX (KEY_0_CENTER_Y + KEY_OPERAND_RADIUS_COS_45)

#define KEY_POINT_X_MIN KEY_8_X_MIN
#define KEY_POINT_Y_MIN KEY_0_Y_MIN
#define KEY_POINT_X_MAX KEY_8_X_MAX
#define KEY_POINT_Y_MAX KEY_0_Y_MAX

#define KEY_EQUAL_X_MIN KEY_9_X_MIN
#define KEY_EQUAL_Y_MIN KEY_0_Y_MIN
#define KEY_EQUAL_X_MAX KEY_9_X_MAX
#define KEY_EQUAL_Y_MAX KEY_0_Y_MAX

#define KEY_C_X_MIN (KEY_C_CENTER_X - KEY_OPERATOR_RADIUS_COS_45)
#define KEY_C_Y_MIN (KEY_C_CENTER_Y - KEY_OPERATOR_RADIUS_COS_45)
#define KEY_C_X_MAX (KEY_C_CENTER_X + KEY_OPERATOR_RADIUS_COS_45)
#define KEY_C_Y_MAX (KEY_C_CENTER_Y + KEY_OPERATOR_RADIUS_COS_45)

#define KEY_CE_X_MIN KEY_C_X_MIN
#define KEY_CE_Y_MIN (KEY_CE_CENTER_Y - KEY_OPERATOR_RADIUS_COS_45)
#define KEY_CE_X_MAX KEY_C_X_MAX
#define KEY_CE_Y_MAX (KEY_CE_CENTER_Y + KEY_OPERATOR_RADIUS_COS_45)

#define KEY_DIV_X_MIN KEY_C_X_MIN
#define KEY_DIV_Y_MIN (KEY_DIV_CENTER_Y - KEY_OPERATOR_RADIUS_COS_45)
#define KEY_DIV_X_MAX KEY_C_X_MAX
#define KEY_DIV_Y_MAX (KEY_DIV_CENTER_Y + KEY_OPERATOR_RADIUS_COS_45)

#define KEY_MUL_X_MIN KEY_C_X_MIN
#define KEY_MUL_Y_MIN (KEY_MUL_CENTER_Y - KEY_OPERATOR_RADIUS_COS_45)
#define KEY_MUL_X_MAX KEY_C_X_MAX
#define KEY_MUL_Y_MAX (KEY_MUL_CENTER_Y + KEY_OPERATOR_RADIUS_COS_45)

#define KEY_SUB_X_MIN KEY_C_X_MIN
#define KEY_SUB_Y_MIN (KEY_SUB_CENTER_Y - KEY_OPERATOR_RADIUS_COS_45)
#define KEY_SUB_X_MAX KEY_C_X_MAX
#define KEY_SUB_Y_MAX (KEY_SUB_CENTER_Y + KEY_OPERATOR_RADIUS_COS_45)

#define KEY_ADD_X_MIN KEY_C_X_MIN
#define KEY_ADD_Y_MIN (KEY_ADD_CENTER_Y - KEY_OPERATOR_RADIUS_COS_45)
#define KEY_ADD_X_MAX KEY_C_X_MAX
#define KEY_ADD_Y_MAX (KEY_ADD_CENTER_Y + KEY_OPERATOR_RADIUS_COS_45)

#define TEXT_OFFSET_X 8
#define TEXT_OFFSET_Y 12

void Draw_Menu(void);

#endif /* __MENU_CALC_H */