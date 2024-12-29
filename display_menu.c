/*
 * Name : N Venkata prasad
 *Project : Car Black Box
 * Submited Date : 03/12/2024 
 */
#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "mkp.h"
#include "adc.h"
#include "external_eeprom.h"
#include "Uart.h"
#include "i2c.h"
#include "ds1307.h"

unsigned int j, k, p, option, v_flag = 1, f_flag = 1;
extern unsigned int main_flag;
unsigned char add=0;
char opt[4][17] = {" view_log       ", " clear_log      ", " Download_log     ", " Set_time       "};
unsigned char log[10][17];
extern unsigned int event_count;
extern unsigned char key;
static int arr1;

void display_menu(void) 
{
    static unsigned int i;
    static unsigned int flag,p_flag = 5;
        if (key == MK_SW12) 
        {
            if (flag == 1) 
            {
                if (j < 2) 
                    j++;
            } 
            else
                flag = 1;
        } 
        else if (key == MK_SW11) 
        {
            if (flag == 0) 
            {
                if (j > 0)
                    j--;
            } 
            else
                flag = 0;
        }
        clcd_print(opt[j], LINE1(0));
        clcd_print(opt[j + 1], LINE2(0));
        //if(p_flag != flag)
        {
            if (flag == 0) 
            {
                clcd_putch('>', LINE1(0));
                clcd_putch(' ', LINE2(0));
            } 
            else 
            {
                clcd_putch(' ', LINE1(0));
                clcd_putch('>', LINE2(0));
            }
           // p_flag = flag;
        }
        if (key == MK_SW1) 
        {
            main_flag  = j + flag + 2;
            flag = 0;
            j = 0;
        }
        else if(key==MK_SW2)
        {
            main_flag  = 0;
        }
 }

void arraystore(void) 
{
    unsigned int m = 0, n = 0, c = 0;
    unsigned char a = 0;
    c = event_count > 10 ? (10 - (event_count) % 10) : 0;
    for(m=0;m<(event_count>10?10:event_count);m++)
    {
        for (n = 0; n < 16; n++) 
        {
            if (n == 0) 
            {
                log[c][0] = c + '0';
            } 
            else if (n == 1 || n == 10 || n == 13) 
            {
                log[c][n] = ' ';
            } 
            else
                log[c][n] = read_EEPROM(a++);
        }
        log[c][16] = '\0';
        c = (c+1)%10;
    }
}

void viewLog(void) 
{
    static unsigned long int DELAY3;
    if (event_count == 0) 
    {
        if (DELAY3++ < 1000) 
        {
        clcd_print("No logs to      ", LINE1(0));
        clcd_print("Display         ", LINE2(0));
        }
        else
        {
        main_flag = 1;
        DELAY3 =0;
        }
    } 
    else 
    {
        arraystore();
        clcd_print("L TIME     EV SP", LINE1(0));
        clcd_print(log[p], LINE2(0));
        if (key == MK_SW11) 
        {
            if (p < 9)
                p++;
        }
        else if (key == MK_SW12) 
        {
            if (p > 0)
                p--;
        }
        if (key == MK_SW2) 
        {
            p=0;
           main_flag=1;  
        }
    }
}

void clear_log(void) 
{
    static unsigned long int DELAY1 = 0;
    
    if (DELAY1++ < 1000) 
    {
        if (event_count == 0) 
        {
            clcd_print("No logs to  be  ", LINE1(0));
            clcd_print("Cleared         ", LINE2(0));
        }
        else
        {
            clcd_print("logs cleared    ", LINE1(0));
            clcd_print("Successfully    ", LINE2(0));
        }
    }
    else
    {
        event_count = 0;
        add = 0;
        DELAY1 = 0;
        main_flag=1;
    }   
}

void download_log(void) 
{
    static unsigned long int DELAY = 0;
    arraystore();      
    if (DELAY++ < 100) 
    {
        if (event_count == 0) 
        {
            if(DELAY==99)
                puts("NO logs to Download\n\r");
            clcd_print("No logs to  be  ", LINE1(0));
            clcd_print("Downloaded      ", LINE2(0));
            for(int i=5000;i--;);
        }
        else
        {
        clcd_print("Downloaded logs ", LINE1(0));
        clcd_print("Successfully    ", LINE2(0));
        if(DELAY==99)
         puts("L Time     Ev SP\n\r");
        }
    }
    else
    {
        
        for (int p = 0; p < (event_count>10?10:event_count); p++) 
        {
            //clcd_print("Downloaded      ", LINE2(0));
            puts(log[p]);
            puts("\n\r"); 
        }
        DELAY = 0;
        main_flag = 1;
        
    }
}

