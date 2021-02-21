#ifndef  TERMINAL_H
#define TERMINAL_H

typedef struct {
	unsigned short ws_row;
	unsigned short ws_col;
	unsigned short ws_xpixel;
	unsigned short ws_ypixel;
}winsize;

typedef enum {
	White = 7,
	Black = 0,
	Red = 1,
	Green = 2,
	Yellow = 3,
	Blue = 4,
	Magenta = 5,
	Cyan = 6
}colors;

int mt_clrscr();
int mt_gotoXY(int y, int x);
int mt_getscreensize(int* rows, int* cols);
int mt_setfgcolor(colors color);
int mt_setbgcolor(colors color);

#endif // ! TERMINAL_h

