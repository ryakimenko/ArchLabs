#ifndef READKEY_H
#define READKEY_H

#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>

enum keys {
	KEY_l,
	KEY_s,
	KEY_r,
	KEY_t,
	KEY_i,
	KEY_q,
	KEY_f5,
	KEY_f6,
	KEY_up,
	KEY_down,
	KEY_left,
	KEY_right,
	KEY_enter,
	KEY_other
};

int rk_readkey(int* key);
int rk_mytermsave(void);
int rk_mytermrestore(void);
int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint);

#endif // !READKEY_H
