#include <gb/gb.h>

unsigned char ufo[] =
{
  0x5A,0x3C,0xE3,0x42,0x7C,0x99,0xEB,0xA5,
  0xFB,0xA5,0x66,0x99,0xE7,0x42,0x5A,0x3C
};

void main() {
    int x = 55;
    int y = 75;

    SPRITES_8x8;
    set_sprite_data(0, 0, ufo);
    set_sprite_tile(0, 0);
    move_sprite(0, x, y);

    SHOW_SPRITES;

    while(1) {
        if(joypad() & J_RIGHT) {
            x++;
            move_sprite(0, x, y);
            delay(10);
        }
        if(joypad() & J_LEFT) {
            x--;
            move_sprite(0, x, y);
            delay(10);
        }
        if(joypad() & J_UP) {
            y--;
            move_sprite(0, x, y);
            delay(10);
        }
        if(joypad() & J_DOWN) {
            y++;
            move_sprite(0, x, y);
            delay(10);
        }
    }
}
