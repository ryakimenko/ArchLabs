#include "terminal.h"
#include <stdio.h>
#include "sys/ioctl.h"
#include <termios.h>
#include <unistd.h>

winsize ws;

int mt_clrscr()
{
	printf("\E[H\E[2J");
	return 0;

}

int mt_gotoXY(int y, int x)
{
	int rows, cols;
	mt_getscreensize(&rows, &cols);
	if (y < 0 || y > rows || x < 0 || x > cols) {
		return -1;
	}
	printf("\E[%d;%dH", y, x);
	return 0;
}

int mt_getscreensize(int* rows, int* cols)
{
	if (!ioctl(1, TIOCGWINSZ, &ws)) {
		*rows = ws.ws_row;
		*cols = ws.ws_col;
	}
	else {
		fprintf(stderr, "Ошибка получения размера экрана.\n");
		return -1;
	}
	return 0;
}

int mt_setfgcolor(colors color)
{
	printf("\E[3%dm", color);
	return 0;
}

int mt_setbgcolor(colors color)
{
	printf("\E[4%dm", color);
	return 0;
}