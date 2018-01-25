#include <gb/gb.h>

/* Simple sprite */
unsigned char ufo[] =
{
  0x5A,0x3C,0xE3,0x42,0x7C,0x99,0xEB,0xA5,
  0xFB,0xA5,0x66,0x99,0xE7,0x42,0x5A,0x3C
};

void main() {

    /* Locations for sprite */
    int up_x = 55;
    int up_y = 75;
    int down_x = 55;
    int down_y = 80;

    /* Enable sound */
    NR50_REG = 0xFF;
    NR51_REG = 0xFF;
    NR52_REG = 0x80;

    /* Initialize sprite */
    SPRITES_8x8;
    set_sprite_data(0, 0, ufo);
    set_sprite_tile(0, 0);
    move_sprite(0, up_x, up_y);
    SHOW_SPRITES;

    while(1)
    {
        if(joypad() & J_A) 
        {
            /* Press sprite down */
            move_sprite(0, down_x, down_y);

            /* Beep */
            NR10_REG = 0x38U;
            NR11_REG = 0x70U;
            NR12_REG = 0xE0U;
            NR13_REG = 0x0AU;
            NR14_REG = 0xC6U;
            NR51_REG |= 0x11;
            delay(400);
        }
        else 
        {
            /* Move sprite back up */
            move_sprite(0, up_x, up_y);
            delay(10);
        }
    }
}
