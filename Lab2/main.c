#include "terminal.h"
#include <stdio.h>
int main()
{
	mt_clrscr();
	mt_gotoXY(3, 3);
	int rows, cols;
	mt_getscreensize(&rows, &cols);
	mt_setfgcolor(Red);
	mt_setbgcolor(White);
	printf("rows = %d, cols = %d\n", rows, cols);
	return 0;
}