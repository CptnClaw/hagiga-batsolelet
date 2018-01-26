#include <gb/gb.h>
#include "DHM.c"
#include "bg_data.c"
#include "bg_data.map"

int up_x;
int up_y;
int down_x;
int down_y;

int state;
#define RECEIVE 0
#define SEND    1

int msg_num;
int msg_length;
int click_counter;
#define MAX_MSG_NUM 2

#define MSG_BEETHOVEN 1
#define MSG_SOCCER 2

void relocated_button(int new_x, int new_y)
{
    move_sprite(0, new_x, new_y);
    move_sprite(1, new_x-1, new_y);
    move_sprite(2, new_x-2, new_y);
    move_sprite(3, new_x-3, new_y);
    move_sprite(4, new_x-2, new_y-1);
    move_sprite(5, new_x-2, new_y-2);
    move_sprite(6, new_x-2, new_y-3);
    move_sprite(7, new_x-3, new_y-3);
    move_sprite(8, new_x-3, new_y-2);
}

void new_beep()
{
    NR10_REG = 0x00U;
    NR11_REG = 0x30U;
    NR12_REG = 0xF0U;
    NR13_REG = 0x0AU;
    NR14_REG = 0x86U;
    NR51_REG |= 0x11;
}

void kill_beep()
{
    NR12_REG = 0x00U;
}

void beep_time(unsigned long ms)
{
    new_beep();
    delay(ms - 50);
    kill_beep();
    delay(50);
    msg_length += 1;
}

void press_down(int down_x, int down_y)
{   
    new_beep();

    // Press sprite down
    relocated_button(down_x, down_y);
}

void leave_button(int up_x, int up_y)
{
    kill_beep();

    // Move sprite back up
    relocated_button(up_x, up_y);
}

void animate_send()
{
    delay(1000);
}

void animate_receive()
{
    delay(1000);
}

void main() 
{
    /* Locations for sprite */
    up_x = 80;
    up_y = 80;
    down_x = 80;
    down_y = 85;

    DISPLAY_OFF;
    
    /* Enable sound */
    NR50_REG = 0xFF;
    NR51_REG = 0xFF;
    NR52_REG = 0x80;

    /* Initialize tiles */
    SPRITES_8x8;
    set_bkg_data(0, 24, sprites);
    VBK_REG = 1;
    VBK_REG = 0;
    set_sprite_data(0x00U, 0x18U, sprites);

    /* Initialize background */
    set_bkg_tiles(0, 0, 20, 18, background);
    set_bkg_tiles(10, 8, 4, 2, machine1);
    set_bkg_tiles(9, 9, 1, 1, machine2);

    /* Initialize sprites */
    set_sprite_tile(0, 8);
    set_sprite_tile(1, 8);
    set_sprite_tile(2, 7);
    set_sprite_tile(3, 6);
    set_sprite_tile(4, 4);
    set_sprite_tile(5, 4);
    set_sprite_tile(6, 4);
    set_sprite_tile(7, 5);
    set_sprite_tile(8, 5);
    relocated_button(up_x, up_y);

    /* Start showing stuff on screen */
    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;

    state = RECEIVE;
    msg_num = 1;
    msg_length = 0;  

    while(1)
    {
        if (SEND == state)
        {
            waitpad(J_A);
            press_down(down_x, down_y);
            waitpadup();
            leave_button(up_x, up_y);

            click_counter += 1;
            if (msg_length == click_counter)
            {
                state = RECEIVE;
                msg_length = 0;
                msg_num += 1;
                if (msg_num > MAX_MSG_NUM)
                {
                    msg_num = 1;
                }
            }
        }        
        if (RECEIVE == state)
        {
            animate_receive();

            if (MSG_BEETHOVEN == msg_num)
            {
                beep_time(250);
                beep_time(250);
                beep_time(250);
                beep_time(1000);
                delay(100);

                beep_time(250);
                beep_time(250);
                beep_time(250);
                beep_time(1000);

                delay(500);
            }
            else if (MSG_SOCCER == msg_num)
            {
                beep_time(500);
                beep_time(500);
                
                beep_time(250);
                beep_time(250);
                beep_time(500);

                beep_time(250);
                beep_time(250);
                beep_time(250);
                beep_time(500);

                beep_time(250);
                beep_time(500);
            }
            
            state = SEND;
            click_counter = 0;
            animate_send();
        }
    }
}
