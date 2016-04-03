#include "menu_qhsmtst.h"

void Draw_Menu(void)
{  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillRect((uint16_t)0, (uint16_t)(GRID_DIVISION * 2), (uint16_t)BSP_LCD_GetXSize(), (uint16_t)(BSP_LCD_GetYSize() - GRID_DIVISION * 2));
  
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
  
  BSP_LCD_DisplayChar((uint16_t)(BUTTON_A_CENTER_X - TEXT_OFFSET_X), (uint16_t)(BUTTON_A_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'A');
  BSP_LCD_DisplayChar((uint16_t)(BUTTON_A_CENTER_X - TEXT_OFFSET_X), (uint16_t)(BUTTON_D_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'D');
  BSP_LCD_DisplayChar((uint16_t)(BUTTON_A_CENTER_X - TEXT_OFFSET_X), (uint16_t)(BUTTON_G_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'G');
  
  BSP_LCD_DisplayChar((uint16_t)(BUTTON_B_CENTER_X - TEXT_OFFSET_X), (uint16_t)(BUTTON_A_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'B');
  BSP_LCD_DisplayChar((uint16_t)(BUTTON_B_CENTER_X - TEXT_OFFSET_X), (uint16_t)(BUTTON_D_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'E');
  BSP_LCD_DisplayChar((uint16_t)(BUTTON_B_CENTER_X - TEXT_OFFSET_X), (uint16_t)(BUTTON_G_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'H');
  
  BSP_LCD_DisplayChar((uint16_t)(BUTTON_C_CENTER_X - TEXT_OFFSET_X), (uint16_t)(BUTTON_A_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'C');
  BSP_LCD_DisplayChar((uint16_t)(BUTTON_C_CENTER_X - TEXT_OFFSET_X), (uint16_t)(BUTTON_D_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'F');
  BSP_LCD_DisplayChar((uint16_t)(BUTTON_C_CENTER_X - TEXT_OFFSET_X), (uint16_t)(BUTTON_G_CENTER_Y - TEXT_OFFSET_Y), (uint8_t)'I');
  
  BSP_LCD_SetFont(&LCD_LOG_TEXT_FONT);
  BSP_LCD_SetTextColor(LCD_LOG_TEXT_COLOR);
  BSP_LCD_SetBackColor(LCD_LOG_BACKGROUND_COLOR);
}