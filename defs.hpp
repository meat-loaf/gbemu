#ifndef _DEFS_H_
#define _DEFS_H_ 1

//hw registers
#define DIVR 0xFF04
#define TIMA 0xFF05
#define TMA 0xFF06
#define TMC 0xFF07
//bit - LCD CONTROL REGISTER FLAG
//7 - LCD Display Enable
//6 - Window Tile Map Display Select (0 0x9800-0x9BFF, 1 0x9C00-0x9FFF)
//5 - Window Display Enable
//4 - BG & Window Tile Data Select (0 0x8800-97FF, 1 0x8000 0x8FFF)
//3 - BG Tile Map Display Select (0 0x9800-0x9BFF, 1 0x9C00 0x9FFF)
//2 - OBJ (Sprite) Size (0 8x8, 1 8x16)
//1 - OBJ (Sprite) Display Enable
//0 - BG Display
#define LCD_CONTROL 0xFF40
#define LCD_STATUS 0xFF41
#define LCD_SCRY 0xFF42
#define LCD_SCRX 0xFF43
#define LCD_SCANLINE 0xFF44
#define LCD_WINY 0xFF4A
#define LCD_WINX 0xFF4B
//various
#define CLK_SPD 4194304 //hz
#define CLK_SPD_GBC CLK_SPD*2
#define TIME_1 4096
#define TIME_2 262144
#define TIME_3 65536
#define TIME_4 16384 //div reg always uses this val
#define MAX_CYCLES 69905

#endif
