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

#define BOX_HORI "q"
#define BOX_VERT "x"
#define BOX_TL "l"
#define BOX_TR "k"
#define BOX_BL "m"
#define BOX_BR "j"
#define BOX_FILL "a"



int mt_clrscr();
int mt_gotoXY(int x, int y);
int mt_getscreensize(int* rows, int* cols);
int mt_setfgcolor(colors color);
int mt_setbgcolor(colors color);
int bc_printA(char* str);
int bc_box(int x1, int y1, int x2, int y2);
int bc_printbigchar(int* big, int x, int y, colors fg, colors bg);
int bc_setbigcharpos(int* big, int x, int y, int value);
int bc_getbigcharpos(int* big, int x, int y, int* value);
int bc_bigcharwrite(int fd, int* big, int count);
int bc_bigcharread(int fd, int* big, int need_count, int* count);

static void get_zero(int value[2]) {
	value[0] = 1717976064;
	value[1] = 3958374;
}

static void  get_one(int value[2]) {
	value[0] = 1010315264;
	value[1] = 3158064;
}

static void  get_two(int value[2]) {
	value[0] = 1010842624;
	value[1] = 8258050;
}

static void  get_three(int value[2]) {
	value[0] = 2120252928;
	value[1] = 8282238;
}

static void  get_four(int value[2]) {
	value[0] = 2120640000;
	value[1] = 6316158;
}

static void  get_five(int value[2]) {
	value[0] = 1040350720;
	value[1] = 4079680;
}

static void  get_six(int value[2]) {
	value[0] = 35789824;
	value[1] = 1974814;
}

static void  get_seven(int value[2]) {
	value[0] = 811630080;
	value[1] = 396312;
}

static void  get_eight(int value[2]) {
	value[0] = 1013332992;
	value[1] = 3958374;
}

static void  get_nine(int value[2]) {
	value[0] = 2087074816;
	value[1] = 3956832;
}

static void  get_plus(int value[2]) {
	value[0] = 2115508224;
	value[1] = 1579134;
}
static void  get_minus(int value[2]) {
	value[0] = 2113929216;
	value[1] = 126;
}

#endif // ! TERMINAL_h

