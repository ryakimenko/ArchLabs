#include "shell.h"
#include "terminal.h"
#include "memory.h"
#include <stdio.h>
#include "sys/ioctl.h"
#include <termios.h>
#include <unistd.h>

void get_cell(char buff[6], int value)
{
    buff[0] = '+';
    buff[4] = value % 10 + 0x30;
    value = value / 10;
    buff[3] = value % 10 + 0x30;
    value = value / 10;
    buff[2] = value % 10 + 0x30;
    value = value / 10;
    buff[1] = value % 10 + 0x30;
    buff[5] = '\0';
}

//Memory rendering
void memory_box_render()
{
    bc_box(1, 1, 62, 12);
    mt_gotoXY(26, 1);
    printf(" Memory ");
    memory_box_filler();
}

void memory_box_filler()
{
    mt_gotoXY(2, 2);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            mt_gotoXY((j * 6) + 2, i + 2);
            render_single_memory_cell(i * 10 + j);
            putchar(' ');
        }
    }
}


void render_single_memory_cell(int index)
{
    int value;
    int ret_value;
    int command;
    int operand;
    char memory_cell[6];

    ret_value = sc_memoryGet(index, &value);
    if (ret_value != 0) {
        invalid_memory_cell(memory_cell);
        printf("%s", memory_cell);
    }

    ret_value = sc_commandDecode(value, &command, &operand);
    if (ret_value != 0) {
        invalid_memory_cell(memory_cell);
        printf("%s", memory_cell);
    }

    get_memory_cell(memory_cell, command, operand);
    printf("%s", memory_cell);

}

void invalid_memory_cell(char memory_cell[6])
{
    memory_cell[0] = '-';
    for (int i = 1; i < 5; i++)
        memory_cell[i] = '0';
    memory_cell[5] = '\0';
}

void get_memory_cell(char memory_cell[6], int command, int operand)
{
    memory_cell[0] = '+';
    memory_cell[1] = command / 10 + 0x30;
    memory_cell[2] = command % 10 + 0x30;
    memory_cell[3] = operand / 10 + 0x30;
    memory_cell[4] = operand % 10 + 0x30;
    memory_cell[5] = '\0';
}


//Accumulator rendering
void accumulator_box_render()
{
    int acum_value = sc_accumGet();
    int command;
    int operand;
    char acum_memory_cell[6];

    sc_commandDecode(acum_value, &command, &operand);
    bc_box(63, 1, 21, 3);
    mt_gotoXY(67, 1);
    printf(" Accumulator ");
    mt_gotoXY(70, 2);

    get_memory_cell(acum_memory_cell, command, operand);
    printf("%s", acum_memory_cell);
}

 //InstructionCounter rendering
void instructionCounter_box_render()
{
    int counter_value = sc_counterGet();
    char counter_memory_cell[6];
    bc_box(63, 4, 21, 3);
    mt_gotoXY(63 + 2, 4);
    printf("instructionCounter");
    mt_gotoXY(63 + 7, 1 + 4);
    get_cell(counter_memory_cell, counter_value);
    printf("%s",counter_memory_cell);
}

//Operation rendering
void operation_box_render()
{

    bc_box(63, 7, 21, 3);
    mt_gotoXY(5 + 63, 7);
    printf(" Operation ");
    mt_gotoXY(7 + 63, 7 + 1);
    printf("+00 : 00");
}

//Flags rendering
void flags_box_render()
{
    bc_box(63, 10, 21, 3);
    mt_gotoXY(63 + 6, 10);
    printf(" Flags ");
    mt_gotoXY(63 + 7, 1 + 10);
    flags_render();
}

void flags_render()
{
    int val;
    char flagsBuff[5] = { ' ', ' ', ' ', ' ', ' ' };

    sc_regGet(OVERFLOW, &val);
    if (val)
        flagsBuff[0] = 'P';
    sc_regGet(ZEROERROR, &val);
    if (val)
        flagsBuff[1] = 'O';
    sc_regGet(OUTMEMORY, &val);
    if (val)
        flagsBuff[2] = 'M';
    sc_regGet(CLOCKIGNORE, &val);
    if (val)
        flagsBuff[3] = 'T';
    sc_regGet(COMMANDERROR, &val);
    if (val)
        flagsBuff[4] = 'E';

    for (int i = 0; i < 5; i++) {
        putchar(flagsBuff[i]);
        putchar(' ');
    }
}

//Keys rendering
void keys_box_render()
{
    bc_box(47, 13, 37, 10);
    mt_gotoXY(47 + 1, 13);
    printf(" Keys:");
    mt_gotoXY(47 + 1, 13 + 1);
    printf("q - quit");
    mt_gotoXY(47 + 1, 13 + 2);
    printf("l - load");
    mt_gotoXY(47 + 1, 13 + 3);
    printf("s - save");
    mt_gotoXY(47 + 1, 13 + 4);
    printf("r - run");
    mt_gotoXY(47 + 1, 13 + 5);
    printf("t - step");
    mt_gotoXY(47 + 1, 13 + 6);
    printf("i - reset");
    mt_gotoXY(47 + 1, 13 + 7);
    printf("F5 - accumulator");
    mt_gotoXY(47 + 1, 13 + 8);
    printf("F6 - instructionCounter");
}

void bigChar_box_render()
{
    int memVal, command, operand;
    int offsetRow = 13;
    int offsetCol = 1;
    int valueChar[2];
    bc_box(offsetCol, offsetRow, 46, 10);
    get_plus(valueChar);
    bc_printbigchar(valueChar, offsetCol + 1, offsetRow + 1, White, Black);
    get_zero(valueChar);
    offsetCol += 9;
    bc_printbigchar(valueChar, offsetCol, offsetRow + 1, White, Black);
    offsetCol += 9;
    bc_printbigchar(valueChar, offsetCol, offsetRow + 1, White, Black);
    offsetCol += 9;
    bc_printbigchar(valueChar, offsetCol, offsetRow + 1, White, Black);
    offsetCol += 9;
    bc_printbigchar(valueChar, offsetCol, offsetRow + 1, White, Black);
}

void interface_render()
{
    mt_clrscr();
    memory_box_render();
    accumulator_box_render();
    instructionCounter_box_render();
    operation_box_render();
    flags_box_render();
    keys_box_render();
    bigChar_box_render();
}
