#include "menu_calc.h"

void Draw_Menu(void)
{
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillRect((uint16_t)0, (uint16_t)GRID_DIVISION_V, (uint16_t)BSP_LCD_GetXSize(), (uint16_t)(BSP_LCD_GetYSize() - GRID_DIVISION_V));
  
  BSP_LCD_SetTextColor(LCD_COLOR_DARKGRAY);
  BSP_LCD_FillRect((uint16_t)(GRID_DIVISION_H * 3), (uint16_t)GRID_DIVISION_V, (uint16_t)(BSP_LCD_GetXSize() - GRID_DIVISION_H * 3), (uint16_t)(BSP_LCD_GetYSize() - GRID_DIVISION_V));
  
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
  BSP_LCD_FillRect((uint16_t)0, (uint16_t)0, (uint16_t)BSP_LCD_GetXSize(), (uint16_t)(24 * 3));
  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  
  BSP_LCD_SetBackColor(LCD_COLOR_DARKBLUE);  
  BSP_LCD_DisplayStringAt((uint16_t)0, (uint16_t)24, (uint8_t *)"calc_stm32f469i-disco", CENTER_MODE);
  
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);  
  BSP_LCD_DisplayChar((uint16_t)(KEY_7_CENTER_X - TEXT_OFFSET_X), (uint16_t)(KEY_7_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'7');
  BSP_LCD_DisplayChar((uint16_t)(KEY_7_CENTER_X - TEXT_OFFSET_X), (uint16_t)(KEY_4_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'4');
  BSP_LCD_DisplayChar((uint16_t)(KEY_7_CENTER_X - TEXT_OFFSET_X), (uint16_t)(KEY_1_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'1');
  BSP_LCD_DisplayChar((uint16_t)(KEY_7_CENTER_X - TEXT_OFFSET_X), (uint16_t)(KEY_0_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'0');  
  BSP_LCD_DisplayChar((uint16_t)(KEY_8_CENTER_X - TEXT_OFFSET_X), (uint16_t)(KEY_7_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'8');
  BSP_LCD_DisplayChar((uint16_t)(KEY_8_CENTER_X - TEXT_OFFSET_X), (uint16_t)(KEY_4_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'5');
  BSP_LCD_DisplayChar((uint16_t)(KEY_8_CENTER_X - TEXT_OFFSET_X), (uint16_t)(KEY_1_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'2');
  BSP_LCD_DisplayChar((uint16_t)(KEY_8_CENTER_X - TEXT_OFFSET_X), (uint16_t)(KEY_0_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'.');  
  BSP_LCD_DisplayChar((uint16_t)(KEY_9_CENTER_X - TEXT_OFFSET_X), (uint16_t)(KEY_7_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'9');
  BSP_LCD_DisplayChar((uint16_t)(KEY_9_CENTER_X - TEXT_OFFSET_X), (uint16_t)(KEY_4_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'6');
  BSP_LCD_DisplayChar((uint16_t)(KEY_9_CENTER_X - TEXT_OFFSET_X), (uint16_t)(KEY_1_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'3');
  BSP_LCD_DisplayChar((uint16_t)(KEY_9_CENTER_X - TEXT_OFFSET_X), (uint16_t)(KEY_0_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'=');
  
  BSP_LCD_SetBackColor(LCD_COLOR_DARKGRAY);  
  BSP_LCD_DisplayChar((uint16_t)(KEY_C_CENTER_X - TEXT_OFFSET_X), (uint16_t)(KEY_C_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'C');
  BSP_LCD_DisplayStringAt((uint16_t)(KEY_C_CENTER_X - 17), (uint16_t)(KEY_CE_CENTER_Y - TEXT_OFFSET_Y), (uint8_t *)"CE", LEFT_MODE);
  BSP_LCD_DisplayChar((uint16_t)(KEY_C_CENTER_X - TEXT_OFFSET_X), (uint16_t)(KEY_DIV_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'/');
  BSP_LCD_DisplayChar((uint16_t)(KEY_C_CENTER_X - TEXT_OFFSET_X), (uint16_t)(KEY_MUL_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'*');
  BSP_LCD_DisplayChar((uint16_t)(KEY_C_CENTER_X - TEXT_OFFSET_X), (uint16_t)(KEY_SUB_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'-');
  BSP_LCD_DisplayChar((uint16_t)(KEY_C_CENTER_X - TEXT_OFFSET_X), (uint16_t)(KEY_ADD_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'+');
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
}