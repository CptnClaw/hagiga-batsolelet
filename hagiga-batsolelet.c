#include <gb/gb.h>
#include "DHM.c"
#include "background.h"
#include "headbg.h"
#include "headturnbg.h"
#include "hand.h"

// Global variables (more efficient)
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
#define MAX_MSG_NUM 3

#define MSG_BEETHOVEN   1
#define MSG_SOCCER      2
#define MSG_STARS       3

unsigned char track_original[10];
unsigned char track_played[10];
unsigned int success;

int i,j;

void relocate_hand(int new_x, int new_y)
{
    for (i = 0; i != 4; i += 1)
    {
        for (j = 0; j != 3; j += 1)
        {
            move_sprite(i+i+i + j, new_x + (8*j), new_y + (8*i));
        }
    }
}

void new_beep()
{ //80 3F D4 FF BF
    NR10_REG = 0x00U;
    NR11_REG = 0x30U;
    NR12_REG = 0xF0U;
    NR13_REG = 0x0AU;
    NR14_REG = 0x86U;
/*
    NR10_REG = 0x80U;
    NR11_REG = 0x3FU;
    NR12_REG = 0xD4U;
    NR13_REG = 0xFFU;
    NR14_REG = 0x86U;*/
    NR51_REG |= 0x11;
}

void kill_beep()
{
    NR12_REG = 0x00U;
}

void beep_time(unsigned long ms)
{
    if (ms > 300)
    {
        track_original[msg_length] = 1;
    }
    else
    {
        track_original[msg_length] = 0;
    }
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
    relocate_hand(down_x, down_y);
}

void leave_button(int up_x, int up_y)
{
    kill_beep();

    // Move sprite back up
    relocate_hand(up_x, up_y);
}

void animate_send()
{
    delay(1000);
    set_bkg_tiles(5, 2, 8, 7, headbg_map_data);
}

void animate_receive()
{
    delay(1000);
    set_bkg_tiles(5, 2, 8, 7, headturnbg_map_data);
    delay(1000);
}

void animate_win(unsigned int win)
{

}

void main() 
{
    DISPLAY_OFF;
    
    // Locations for sprite
    up_x = 40;
    up_y = 72;
    down_x = 40;
    down_y = 82;
    
    // Turn on sound
    NR50_REG = 0xFF;
    NR51_REG = 0xFF;
    NR52_REG = 0x80;

    // Initialize tiles
    SPRITES_8x8;
    set_bkg_data(0, 153, background_tile_data);
    set_bkg_data(153, 44, headbg_tile_data);
    set_bkg_data(197, 42, headturnbg_tile_data);
    //set_bkg_data(169, 24, sprites);
    VBK_REG = 1;
    VBK_REG = 0;
    set_sprite_data(0, 12, hand_tile_data);

    // Initialize background maps
    set_bkg_tiles(0, 0, 20, 18, background_map_data);
    set_bkg_tiles(5, 2, 8, 7, headbg_map_data);
    //set_bkg_tiles(15, 11, 4, 2, machine1);
    //set_bkg_tiles(14, 12, 1, 1, machine2);

    // Initialize sprite maps
    for (i = 0; i != 15; i += 1)
    {
        set_sprite_tile(i, hand_map_data[i]);
    }
    relocate_hand(up_x, up_y);

    // Start showing stuff on screen
    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;

    // Initialize receiving state
    state = RECEIVE;
    msg_num = 1;
    msg_length = 0;  

    while(1)
    {
        if (SEND == state)
        {
            // Wait for input
            waitpad(J_A);
            press_down(down_x, down_y);
            waitpadup();
            leave_button(up_x, up_y);

            // Check if track ended
            click_counter += 1;
            if (msg_length == click_counter)
            {
                // Verify success criteria
                success = 1;
                for (i = 0; i != msg_length; i += 1)
                {
                    if (track_original[i] != track_played[i])
                    {
                        success = 0;
                    }
                }
                animate_win(success);

                // Move back to receiving state
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
            // Turn head
            animate_receive();

            // Play original track
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
            else if (MSG_STARS == msg_num)
            {
                beep_time(550);
                delay(200);

                beep_time(250);
                beep_time(250);
                delay(250);

                beep_time(250);
                beep_time(125);
                beep_time(550);
                delay(200);

                beep_time(250);
                beep_time(250);
                delay(250);

                beep_time(250);
                beep_time(125);
                beep_time(550);
                delay(200);
            }
            
            // Go the sending state
            state = SEND;
            click_counter = 0;
            animate_send();
        }
    }
}
