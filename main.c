/*
 * Name : N Venkata prasad
 *Project : Car Black Box
 * Submited Date : 03/12/2024 
 */
#include <xc.h>
#include "clcd.h"
#include "mkp.h"
#include "adc.h"
#include "main.h"
#include "external_eeprom.h"
#include "Uart.h" 
#include "i2c.h"
#include "ds1307.h"

unsigned char key;
extern unsigned int main_flag=0;
static void init_config(void)

{
    init_clcd();
    init_matrix_keypad();
    init_adc();
    init_uart();
    init_i2c();
	init_ds1307();
   
}
void main(void)
{
    init_config();
    while(1)
    { 
         key = read_switches(STATE_CHANGE);
        get_time();
        if(main_flag==0)
        {
           dashboard_menu();
        }
        else if(main_flag==1)
        {
            //CLEAR_DISP_SCREEN;
            display_menu();
        }
        else if(main_flag==2)
        {
              viewLog();
        }
        else if(main_flag == 3)
        {
            clear_log();
        }
        else if(main_flag == 4)
        {
            download_log();
        }
        else if(main_flag==5)
        {
            set_time();
        }
    }
    
}
 
