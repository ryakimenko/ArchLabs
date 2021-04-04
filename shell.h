#ifndef SHELL_H
#define SHELL_H

#include "terminal.h"
#include "memory.h"
#include "readKey.h"
#include <stdio.h>
#include "sys/ioctl.h"
#include <termios.h>
#include <unistd.h>

void get_cell(char buff[6], int value);

//Memory rendering
void memory_box_render();
void memory_box_filler();
void render_single_memory_cell(int index);
void invalid_memory_cell(char memory_cell[6]);
void get_memory_cell(char memory_cell[6], int command, int operand);

//Accumulator rendering
void accumulator_box_render();

//InstructionCounter rendering
void instructionCounter_box_render();

//Operation rendering
void operation_box_render(int index);

//Flags rendering
void flags_box_render();
void flags_render();

//Keys rendering
void keys_box_render();

//Big char box render
void bigChar_box_render();

//Interface render
void interface_render();

//Shell
void shell();

#endif // SHELL_H
