#ifndef __MENU_QHSMTST_H
#define __MENU_QHSMTST_H

#include "main.h"

#define LCD_PIXEL_X 800
#define LCD_PIXEL_Y 480

#define GRID_DIVISION 160

#define BUTTON_RADIUS 65
#define BUTTON_RADIUS_COS_45 46

#define BUTTON_A_CENTER_X 80
#define BUTTON_B_CENTER_X (BUTTON_A_CENTER_X + GRID_DIVISION)
#define BUTTON_C_CENTER_X (BUTTON_A_CENTER_X + GRID_DIVISION * 2)

#define BUTTON_A_CENTER_Y (GRID_DIVISION * 2 + BUTTON_A_CENTER_X)
#define BUTTON_D_CENTER_Y (GRID_DIVISION * 3 + BUTTON_A_CENTER_X)
#define BUTTON_G_CENTER_Y (GRID_DIVISION * 4 + BUTTON_A_CENTER_X)

#define BUTTON_A_X_MIN (BUTTON_A_CENTER_X - BUTTON_RADIUS_COS_45)
#define BUTTON_A_Y_MIN (BUTTON_A_CENTER_Y - BUTTON_RADIUS_COS_45)
#define BUTTON_A_X_MAX (BUTTON_A_CENTER_X + BUTTON_RADIUS_COS_45)
#define BUTTON_A_Y_MAX (BUTTON_A_CENTER_Y + BUTTON_RADIUS_COS_45)

#define BUTTON_B_X_MIN (BUTTON_B_CENTER_X - BUTTON_RADIUS_COS_45)
#define BUTTON_B_Y_MIN BUTTON_A_Y_MIN
#define BUTTON_B_X_MAX (BUTTON_B_CENTER_X + BUTTON_RADIUS_COS_45)
#define BUTTON_B_Y_MAX BUTTON_A_Y_MAX

#define BUTTON_C_X_MIN (BUTTON_C_CENTER_X - BUTTON_RADIUS_COS_45)
#define BUTTON_C_Y_MIN BUTTON_A_Y_MIN
#define BUTTON_C_X_MAX (BUTTON_C_CENTER_X + BUTTON_RADIUS_COS_45)
#define BUTTON_C_Y_MAX BUTTON_A_Y_MAX

#define BUTTON_D_X_MIN BUTTON_A_X_MIN
#define BUTTON_D_Y_MIN (BUTTON_D_CENTER_Y - BUTTON_RADIUS_COS_45)
#define BUTTON_D_X_MAX BUTTON_A_X_MAX
#define BUTTON_D_Y_MAX (BUTTON_D_CENTER_Y + BUTTON_RADIUS_COS_45)

#define BUTTON_E_X_MIN BUTTON_B_X_MIN
#define BUTTON_E_Y_MIN BUTTON_D_Y_MIN
#define BUTTON_E_X_MAX BUTTON_B_X_MAX
#define BUTTON_E_Y_MAX BUTTON_D_Y_MAX

#define BUTTON_F_X_MIN BUTTON_C_X_MIN
#define BUTTON_F_Y_MIN BUTTON_D_Y_MIN
#define BUTTON_F_X_MAX BUTTON_C_X_MAX
#define BUTTON_F_Y_MAX BUTTON_D_Y_MAX

#define BUTTON_G_X_MIN BUTTON_A_X_MIN
#define BUTTON_G_Y_MIN (BUTTON_G_CENTER_Y - BUTTON_RADIUS_COS_45)
#define BUTTON_G_X_MAX BUTTON_A_X_MAX
#define BUTTON_G_Y_MAX (BUTTON_G_CENTER_Y + BUTTON_RADIUS_COS_45)

#define BUTTON_H_X_MIN BUTTON_B_X_MIN
#define BUTTON_H_Y_MIN BUTTON_G_Y_MIN
#define BUTTON_H_X_MAX BUTTON_B_X_MAX
#define BUTTON_H_Y_MAX BUTTON_G_Y_MAX

#define BUTTON_I_X_MIN BUTTON_C_X_MIN
#define BUTTON_I_Y_MIN BUTTON_G_Y_MIN
#define BUTTON_I_X_MAX BUTTON_C_X_MAX
#define BUTTON_I_Y_MAX BUTTON_G_Y_MAX

#define TEXT_OFFSET_X 8
#define TEXT_OFFSET_Y 12

void Draw_Menu(void);

#endif /* __MENU_QHSMTST_H */