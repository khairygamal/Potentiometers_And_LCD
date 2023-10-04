
#include <stdio.h>

#include "ecu_chr_lcd.h"

static std_ReturnType lcd_send_4bits(const chr_lcd_4bit_t *lcd ,uint8 _command);
static std_ReturnType lcd_4bits_send_enable_signal(const chr_lcd_4bit_t *lcd);
static std_ReturnType lcd_8bits_send_enable_signal(const chr_lcd_8bit_t *lcd);
static std_ReturnType lcd_8bit_set_cursor(const chr_lcd_8bit_t *lcd ,uint8 row ,uint8 column);
static std_ReturnType lcd_4bit_set_cursor(const chr_lcd_4bit_t *lcd ,uint8 row ,uint8 column);


std_ReturnType lcd_4bit_intialize(const chr_lcd_4bit_t *lcd)
{
 std_ReturnType ret =E_OK;
 uint8 l_data_pins =ZERO_INIT;
 if(NULL == lcd)
 {
     ret =E_NOT_OK;
 }
 else
 {
  ret =gpio_pin_direction_intialize(&(lcd->lcd_rs));  
  ret =gpio_pin_direction_intialize(&(lcd->lcd_en));
   for(l_data_pins =ZERO_INIT;l_data_pins<4;l_data_pins++)
         {
             ret =gpio_pin_intialize(&(lcd->lcd_data[l_data_pins]));
         }
  __delay_ms(20);
  ret =lcd_4bit_send_command(lcd ,_LCD_4BIT_MODE_2_LINE);
  __delay_ms(5);
  ret =lcd_4bit_send_command(lcd ,_LCD_4BIT_MODE_2_LINE);
  __delay_us(150);
  ret =lcd_4bit_send_command(lcd ,_LCD_4BIT_MODE_2_LINE);
  ret =lcd_4bit_send_command(lcd ,_LCD_CLEAR);
  ret =lcd_4bit_send_command(lcd ,_LCD_RETURN_HOME);
  ret =lcd_4bit_send_command(lcd ,_LCD_ENTERY_MODE);
  ret =lcd_4bit_send_command(lcd ,_LCD_CURSOR_OFF_DISPLAY_ON);
  ret =lcd_4bit_send_command(lcd ,_LCD_4BIT_MODE_2_LINE);
  ret =lcd_4bit_send_command(lcd ,0x80);
 }
 return ret;
}
std_ReturnType lcd_4bit_send_command(const chr_lcd_4bit_t *lcd, uint8 command)
{
  std_ReturnType ret =E_OK;
 if(NULL == lcd)
 {
     ret =E_NOT_OK;
 }
 else
 {
   ret =gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_LOW);  
   ret =lcd_send_4bits(lcd ,command >> 4);
   ret =lcd_4bits_send_enable_signal(lcd);
   ret =lcd_send_4bits(lcd ,command);
   ret =lcd_4bits_send_enable_signal(lcd);
 }
 return ret;
    
}
std_ReturnType lcd_4bit_send_char_data(const chr_lcd_4bit_t *lcd, uint8 data)
{
   std_ReturnType ret =E_OK;
 if(NULL == lcd)
 {
     ret =E_NOT_OK;
 }
 else
 {
   ret =gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_HIGH);  
   ret =lcd_send_4bits(lcd ,data >> 4);
   ret =lcd_4bits_send_enable_signal(lcd);
   ret =lcd_send_4bits(lcd ,data);
   ret =lcd_4bits_send_enable_signal(lcd);
 }
 return ret;  
}
std_ReturnType lcd_4bit_send_char_data_pos(const chr_lcd_4bit_t *lcd, uint8 row, uint8 column, uint8 data)
{
  std_ReturnType ret =E_OK;
 if(NULL == lcd)
 {
     ret =E_NOT_OK;
 }
 else
 {
  ret =lcd_4bit_set_cursor(lcd ,row ,column);
  ret =lcd_4bit_send_char_data(lcd,data);
 }
 return ret;   
    
}
std_ReturnType lcd_4bit_send_string(const chr_lcd_4bit_t *lcd, uint8 *str)
{
  std_ReturnType ret =E_OK;
 if(NULL == lcd)
 {
     ret =E_NOT_OK;
 }
 else 
 {
     while(*str)
     {
         ret =lcd_4bit_send_char_data(lcd,*str++);
     }
 }
 return ret;  
}

std_ReturnType lcd_4bit_send_string_pos(const chr_lcd_4bit_t *lcd, uint8 row, uint8 column, uint8 *str)
{
  std_ReturnType ret =E_OK;
 if(NULL == lcd)
 {
     ret =E_NOT_OK;
 }
 else 
 {
     ret = lcd_4bit_set_cursor(lcd ,row ,column);
     while(*str)
     {
         ret =lcd_4bit_send_char_data(lcd,*str++);
     }
 }
 return ret; 
}
std_ReturnType lcd_4bit_send_string_custom_char(const chr_lcd_4bit_t *lcd, uint8 row, uint8 column, const uint8 _chr[], uint8 mem_pos)
{
 std_ReturnType ret =E_OK;
 uint8 lcd_count=ZERO_INIT;
 if(NULL == lcd)
 {
     ret =E_NOT_OK;
 }
 else 
 {
      ret =lcd_4bit_send_command(lcd ,(_LCD_CGRAM_START+(mem_pos*8)));
     for(lcd_count=ZERO_INIT;lcd_count<=7;++lcd_count)
     {
        ret =lcd_4bit_send_char_data(lcd,_chr[lcd_count]);
     }
     ret =lcd_4bit_send_char_data_pos(lcd ,row ,column ,mem_pos);   
 }
 return ret;  
}

std_ReturnType lcd_8bit_intialize(const chr_lcd_8bit_t *lcd)
{
 std_ReturnType ret =E_OK;
 uint8 l_data_pins =ZERO_INIT;
 if(NULL == lcd)
 {
     ret =E_NOT_OK;
 }
 else
 {
  ret =gpio_pin_direction_intialize(&(lcd->lcd_rs));  
  ret =gpio_pin_direction_intialize(&(lcd->lcd_en));
   for(l_data_pins =ZERO_INIT; l_data_pins<8; l_data_pins++)
         {
             ret =gpio_pin_intialize(&(lcd->lcd_data[l_data_pins]));
         }
  __delay_ms(20);
  ret =lcd_8bit_send_command(lcd ,_LCD_8BIT_MODE_2_LINE);
  __delay_ms(5);
  ret =lcd_8bit_send_command(lcd ,_LCD_8BIT_MODE_2_LINE);
  __delay_us(150);
  ret =lcd_8bit_send_command(lcd ,_LCD_8BIT_MODE_2_LINE);
  ret =lcd_8bit_send_command(lcd ,_LCD_CLEAR);
  ret =lcd_8bit_send_command(lcd ,_LCD_RETURN_HOME);
  ret =lcd_8bit_send_command(lcd ,_LCD_ENTERY_MODE);
  ret =lcd_8bit_send_command(lcd ,_LCD_CURSOR_OFF_DISPLAY_ON);
  ret =lcd_8bit_send_command(lcd ,_LCD_8BIT_MODE_2_LINE);
  ret =lcd_8bit_send_command(lcd ,0x80);
  
  
 }
 return ret;
}
std_ReturnType lcd_8bit_send_command(const chr_lcd_8bit_t *lcd, uint8 command)
{
  std_ReturnType ret =E_OK;
  uint8 pin_counter =ZERO_INIT;
 if(NULL == lcd)
 {
     ret =E_NOT_OK;
 }
 else
 {
   ret =gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_LOW);
   for(pin_counter=0;pin_counter<8;pin_counter++)
   {
    ret =gpio_pin_write_logic(&(lcd->lcd_data[pin_counter]),(command >> pin_counter) & (uint8)0x01);
   }
   ret =lcd_8bits_send_enable_signal(lcd);
 }
 return ret;
       
}
std_ReturnType lcd_8bit_send_char_data(const chr_lcd_8bit_t *lcd, uint8 data)
{
   std_ReturnType ret =E_OK;
   uint8 pin_counter =ZERO_INIT;
 if(NULL == lcd)
 {
     ret =E_NOT_OK;
 }
 else
 {
   ret =gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_HIGH);
  for(pin_counter=0;pin_counter<8;pin_counter++)
   {
    ret =gpio_pin_write_logic(&(lcd->lcd_data[pin_counter]),(data >> pin_counter) & (uint8)0x01);
   }
   ret =lcd_8bits_send_enable_signal(lcd);  
 }
 return ret;   
    
    
}
std_ReturnType lcd_8bit_send_char_data_pos(const chr_lcd_8bit_t *lcd, uint8 row, uint8 column, uint8 data)
{
 std_ReturnType ret =E_OK;
 if(NULL == lcd)
 {
     ret =E_NOT_OK;
 }
 else
 {
   ret = lcd_8bit_set_cursor(lcd ,row ,column);
   ret =lcd_8bit_send_char_data(lcd ,data);
 }
 return ret;   
    
}
std_ReturnType lcd_8bit_send_string(const chr_lcd_8bit_t *lcd, uint8 *str)
{
  std_ReturnType ret =E_OK;
 if(NULL == lcd)
 {
     ret =E_NOT_OK;
 }
 else 
 {
     while(*str)
     {
         ret =lcd_8bit_send_char_data(lcd,*str++);
     }
 }
 return ret;    
}
std_ReturnType lcd_8bit_send_string_pos(const chr_lcd_8bit_t *lcd, uint8 row, uint8 column, uint8 *str)
{
 std_ReturnType ret =E_OK;
 if(NULL == lcd)
 {
     ret =E_NOT_OK;
 }
 else
 {
   ret = lcd_8bit_set_cursor(lcd ,row ,column);
        while(*str)
     {
         ret =lcd_8bit_send_char_data(lcd,*str++);
     }
   
 }
 return ret; 
}
std_ReturnType lcd_8bit_send_string_custom_char(const chr_lcd_8bit_t *lcd, uint8 row, uint8 column, const uint8 _chr[], uint8 mem_pos)
{
   std_ReturnType ret =E_OK;
   uint8 lcd_count=ZERO_INIT;   
 if(NULL == lcd)
 {
     ret =E_NOT_OK;
 }
 else 
 {
     ret =lcd_8bit_send_command(lcd ,(_LCD_CGRAM_START+(mem_pos*8)));
     for(lcd_count=ZERO_INIT;lcd_count<=7;++lcd_count)
     {
        ret =lcd_8bit_send_char_data(lcd,_chr[lcd_count]);
     }
     ret =lcd_8bit_send_char_data_pos(lcd ,row ,column ,mem_pos);
 }
 return ret;  
}

std_ReturnType Convert_byte_to_string(uint8 value ,uint8 *str)
{
    std_ReturnType ret =E_OK;
    if(NULL == str)
    {
        ret =E_NOT_OK;
    }
    else
    {
     memset(str,'\0',4);
     sprintf(str,"%i",value);
    }
    return ret;
}
std_ReturnType Convert_short_to_string(uint16 value ,uint8 *str)
{
     std_ReturnType ret =E_OK;
    if(NULL == str)
    {
        ret =E_NOT_OK;
    }
    else
    {
       memset(str,'\0',6);
      sprintf(str,"%i",value);  
    }
    return ret;   
}
std_ReturnType Convert_int_to_string(uint32 value ,uint8 *str)
{
    std_ReturnType ret =E_OK;
    if(NULL == str)
    {
        ret =E_NOT_OK;
    }
    else
    {
      memset(str,'\0',11);
      sprintf(str,"%i",value);
    }
    return ret;
}

static std_ReturnType lcd_send_4bits(const chr_lcd_4bit_t *led ,uint8 _command)
{
  std_ReturnType ret =E_OK;
   ret =gpio_pin_write_logic(&(led->lcd_data[0]),(_command >>0) & (uint8)0x01); 
   ret =gpio_pin_write_logic(&(led->lcd_data[1]),(_command >>1) & (uint8)0x01);
   ret =gpio_pin_write_logic(&(led->lcd_data[2]),(_command >>2) & (uint8)0x01);
   ret =gpio_pin_write_logic(&(led->lcd_data[3]),(_command >>3) & (uint8)0x01);
  return ret;
}

static std_ReturnType lcd_4bits_send_enable_signal(const chr_lcd_4bit_t *lcd)
{
  std_ReturnType ret =E_OK;
   ret =gpio_pin_write_logic(&(lcd->lcd_en),GPIO_HIGH); 
   __delay_us(5);
   ret =gpio_pin_write_logic(&(lcd->lcd_en),GPIO_LOW); 
  return ret;
}
static std_ReturnType lcd_8bits_send_enable_signal(const chr_lcd_8bit_t *lcd)
{
   std_ReturnType ret =E_OK;
   ret =gpio_pin_write_logic(&(lcd->lcd_en),GPIO_HIGH); 
   __delay_us(5);
   ret =gpio_pin_write_logic(&(lcd->lcd_en),GPIO_LOW); 
  return ret;   
}

static std_ReturnType lcd_8bit_set_cursor(const chr_lcd_8bit_t *lcd ,uint8 row ,uint8 column)
{
    std_ReturnType ret =E_OK;
    switch (row)
    {
        case ROW1: ret =lcd_8bit_send_command(lcd,(0x80 + column)); break;
        case ROW2: ret =lcd_8bit_send_command(lcd,(0xc0 + column)); break;
        case ROW3: ret =lcd_8bit_send_command(lcd,(0x94 + column)); break;
        case ROW4: ret =lcd_8bit_send_command(lcd,(0xd4 + column)); break;
        default : ;
    }
    
    
    return ret;    
}

static std_ReturnType lcd_4bit_set_cursor(const chr_lcd_4bit_t *lcd ,uint8 row ,uint8 column)
{
    std_ReturnType ret =E_OK;
    switch (row)
    {
        case ROW1: ret =lcd_4bit_send_command(lcd,(0x80 + column)); break;
        case ROW2: ret =lcd_4bit_send_command(lcd,(0xc0 + column)); break;
        case ROW3: ret =lcd_4bit_send_command(lcd,(0x94 + column)); break;
        case ROW4: ret =lcd_4bit_send_command(lcd,(0xd4 + column)); break;
        default : ;
    }
    
    
    return ret;    
}
