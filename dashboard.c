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

unsigned int t_flag=1,h=0,m=0,s=0,blink = 0,swap=0,even=1;

unsigned char event[9][3]={"ON","GN","G1","G2","G3","G4","G5","GR","C_"};
//unsigned char time[9]={"00:00:00"};
unsigned int speed;
extern unsigned char key;
extern unsigned char add=0;
unsigned char sp[3]={"00"};
static unsigned int i,n;
unsigned int event_count;
unsigned int main_flag,view_flag;
unsigned int c_key,eve_count;
unsigned char clock_reg[3];
unsigned char time[9];

void  dashboard_menu(void)
{
       
        
        clcd_print("TIME     EV SP  ",LINE1(0)); 
        clcd_print(time,LINE2(0));
        clcd_putch(' ',LINE2(8));
        if(n==0)
        {   
            if(even)
            {
                event_count++;
                store_event();
                even = 0;
            }
            clcd_print(event[n],LINE2(9));
        }
        //Gear Increment
        if(key==MK_SW2)
        { 
             event_count++;
            
            if(c_key==1)
            {
                c_key=0;
                n=1;
            }
            else
            {
             n++;
            }
            if(n>=7)
                n=7;
              store_event();
        }
        //Gear decrement
        else if(key==MK_SW3)
        {
            event_count++;
            if(c_key == 1)
            {
                n=1;
                c_key=0;
            }
            else
            {
                if(n!=0)
                {
                    n--;
                    if(n<=1)
                    n=1;
                }
            }
            store_event();

        }
        // For collision c_flag for collision detection
        else if(key==MK_SW1)
        {
            event_count++;
            
             n=8;
            c_key=1;
            store_event();
        }
        //to go for display_menu
        else if(key == MK_SW11)
        {
            main_flag=1;
        }
        clcd_print(event[n],LINE2(9));
        speed = read_adc(CHANNEL4)/10.23;
        if(speed >= 99)
            speed=99;
        sp[0]=speed/10 + '0';
        sp[1] = speed%10 + '0';
        sp[2] ='\0';
        clcd_putch(' ',LINE2(11));
        clcd_print(sp,LINE2(12));

}

void store_event(void)
{
    if(add >= 120)
    {
        add=0;
    }
    for(int i=0;i<8;i++)
    {
        write_EEPROM(add++ , time[i] );
    }
    for(int j=0;j<2;j++)
    {
        write_EEPROM(add++,event[n][j]);
    }
    for(int j=0;j<2;j++)
    {
        write_EEPROM(add++,sp[j]);
    }
    
   
}

void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x40)
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}

void set_time(void)
{
    if(t_flag == 1){
     h = (time[0]-48)*10;
     h = h+(time[1]-48);
     
     m = (time[3]-48)*10;
     m = m+(time[4]-48);
     
     s = (time[6]-48)*10;
     s = s+(time[7]-48);
      t_flag = 0;
    }
    blink = !blink;
    clcd_print("HH:MM:SS        ",LINE1(0));
    if(key == MK_SW12){
        if(swap >= 0 && swap <2)
            swap++;
        else if(swap >=2)
            swap = 0;
    }
    if(key == MK_SW11){
        if(swap == 0){
            if(h>=0 && h < 23)
                h++;
            else
                h = 0;
        }
        if(swap == 1){
            if(m>=0 && m < 59)
                m++;
            else
                m = 0;
        }
        if(swap == 2){
            if(s>=0 && s < 59)
                s++;
            else
                s = 0;
        }
    }
    clcd_putch('0'+(h/10),LINE2(0));
    clcd_putch('0'+(h%10),LINE2(1));
    clcd_putch(':',LINE2(2));
    clcd_putch('0'+(m/10),LINE2(3));
    clcd_putch('0'+(m%10),LINE2(4));
    clcd_putch(':',LINE2(5));
    clcd_putch('0'+(s/10),LINE2(6));
    clcd_putch('0'+(s%10),LINE2(7));
    clcd_print("        ",LINE2(8));
    
    if(swap == 0){
        if(blink){
           clcd_putch('0'+(h/10),LINE2(0));
           clcd_putch('0'+(h%10),LINE2(1)); 
        }
        else{
            clcd_putch(' ',LINE2(0));
            clcd_putch(' ',LINE2(1));
        }
    }
    if(swap == 1){
        if(blink){
           clcd_putch('0'+(m/10),LINE2(3));
           clcd_putch('0'+(m%10),LINE2(4)); 
        }
        else{
            clcd_putch(' ',LINE2(3));
            clcd_putch(' ',LINE2(4));
        }
    }
    if(swap == 2){
        if(blink){
           clcd_putch('0'+(s/10),LINE2(6));
           clcd_putch('0'+(s%10),LINE2(7)); 
        }
        else{
            clcd_putch(' ',LINE2(6));
            clcd_putch(' ',LINE2(7));
        }
    }
    for(unsigned long int i = 100000;i--;);
    
    if(key == MK_SW1){
        write_ds1307(HOUR_ADDR,(h/10)<<4 | (h%10));
        write_ds1307(MIN_ADDR,(m/10)<<4 | (m%10));
        write_ds1307(SEC_ADDR,(s/10)<<4 | (s%10));
        swap = 0;
        main_flag = 0;
         t_flag = 1;
    }
    else if(key == MK_SW2){
        
        swap = 0;
        main_flag = 1;
         t_flag = 1;
    }
   
}