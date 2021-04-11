#include "shell.h"

void inputMemory();
void reset();
void inputAccumulator();
void inputCounter();
void iterCounter();
void sighandler(int signo)
{
    mt_gotoXY(1, 25);
    int clockFlag;
    sc_regGet(CLOCKIGNORE, &clockFlag);
    if (clockFlag)
        return;
    if (signo == SIGALRM) {
        mt_gotoXY(1, 25);
        iterCounter();
        mt_gotoXY(1, 25);
        instructionCounter_box_render();
        mt_gotoXY(1, 25);
        memory_box_render();
        mt_gotoXY(1, 25);
        bigChar_box_render();
        mt_gotoXY(1, 25);
    }
    if (signo == SIGUSR1) {
        reset();
        mt_gotoXY(1, 25);
    }
    mt_gotoXY(1, 25);

}

static struct Cell {
    int posRow;
    int posCol;
} currCell;

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
    mt_gotoXY(1, 25);
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
    mt_setbgcolor(Black);
    mt_setfgcolor(White);
    mt_gotoXY((currCell.posCol * 6) + 2, currCell.posRow + 2);
    render_single_memory_cell(currCell.posRow * 10 + currCell.posCol);
    mt_setbgcolor(White);
    mt_setfgcolor(Black);
    mt_gotoXY(1, 25);
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
    mt_gotoXY(1, 25);
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
    mt_gotoXY(1, 25);
}

//Operation rendering

static void getOperationBuff(char buff[7], int const command, int const operand)
{
    buff[0] = '+';
    buff[3] = ':';
    buff[1] = command / 10 + 0x30;
    buff[2] = command % 10 + 0x30;
    buff[4] = operand / 10 + 0x30;
    buff[5] = operand % 10 + 0x30;
    buff[6] = '\0';
}

void wrong_str_operation(char buff[7]) {
    buff[0] = '-';
    for (int i = 1; i < 6; i++)
        buff[i] = '0';
    buff[3] = ':';
    buff[6] = '\0';
}

void operation_box_render(int index)
{
    int memValue;
    int command;
    int operand;
    int retval;
    char buff[7];

    bc_box(63, 7, 21, 3);
    mt_gotoXY(5 + 63, 7);
    printf(" Operation ");
    mt_gotoXY(7 + 63, 7 + 1);
    retval = sc_memoryGet(index, &memValue);
    if (retval != 0) {
        wrong_str_operation(buff);
        printf("%s", buff);
    }
    retval = sc_commandDecode(memValue, &command, &operand);
    if (retval != 0) {
        wrong_str_operation(buff);
        printf("%s", buff);
    }
    getOperationBuff(buff, command, operand);
    printf("%s", buff);
    mt_gotoXY(1, 25);
}

//Flags rendering
void flags_box_render()
{
    bc_box(63, 10, 21, 3);
    mt_gotoXY(63 + 6, 10);
    printf(" Flags ");
    mt_gotoXY(63 + 7, 1 + 10);
    flags_render();
    mt_gotoXY(1, 25);
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
    mt_gotoXY(1, 25);
}

//Keys rendering
void keys_box_render()
{
    bc_box(47, 13, 37, 10);
    mt_gotoXY(47 + 1, 13);
    printf(" Keys:");
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
    mt_gotoXY(1, 25);
}

void bigChar_box_render()
{
    int memVal, command, operand;
    int offsetRow = 13;
    int offsetCol = 1;
    int valueChar[2];
    bc_box(offsetCol, offsetRow, 46, 10);
    sc_memoryGet(currCell.posRow * 10 + currCell.posCol, &memVal);
    if (sc_commandDecode(memVal, &command, &operand) != 0) {
        command = 0;
        operand = 0;
        get_minus(valueChar);
        bc_printbigchar(valueChar, offsetCol + 1, offsetRow + 1, Black, White);
        offsetCol += 9;
        get_zero(valueChar);
        bc_printbigchar(valueChar, offsetCol, offsetRow + 1, Black, White);
        offsetCol += 9;
        get_zero(valueChar);
        bc_printbigchar(valueChar, offsetCol, offsetRow + 1, Black, White);
        offsetCol += 9;
        get_zero(valueChar);
        bc_printbigchar(valueChar, offsetCol, offsetRow + 1, Black, White);
        offsetCol += 9;
        get_zero(valueChar);
        bc_printbigchar(valueChar, offsetCol, offsetRow + 1, Black, White);
    }
    else {
        int val1, val2, val3, val4;
        val4 = memVal % 10;
        memVal = memVal / 10;
        val3 = memVal % 10;
        memVal = memVal / 10;
        val2 = memVal % 10;
        memVal = memVal / 10;
        val1 = memVal % 10;
        get_plus(valueChar);
        bc_printbigchar(valueChar, offsetCol + 1, offsetRow + 1, Black, White);
        offsetCol += 9;
        choiseBigVal(val1, valueChar);
        bc_printbigchar(valueChar, offsetCol, offsetRow + 1, Black, White);
        offsetCol += 9;
        choiseBigVal(val2, valueChar);
        bc_printbigchar(valueChar, offsetCol, offsetRow + 1, Black, White);
        offsetCol += 9;
        choiseBigVal(val3, valueChar);
        bc_printbigchar(valueChar, offsetCol, offsetRow + 1, Black, White);
        offsetCol += 9;
        choiseBigVal(val4, valueChar);
        bc_printbigchar(valueChar, offsetCol, offsetRow + 1, Black, White);
    }
    mt_gotoXY(1, 25);
}

void fillContext() {
    int height, width;
    mt_getscreensize(&height, &width);

    mt_gotoXY(0, 0);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            putchar(' ');
        putchar('\n');
    }
}

void interface_render()
{
    mt_setbgcolor(White);
    mt_setfgcolor(Black);
    fillContext();
    mt_clrscr();
    memory_box_render();
    accumulator_box_render();
    instructionCounter_box_render();
    operation_box_render(currCell.posRow * 10 + currCell.posCol);
    bigChar_box_render();
    flags_box_render();
    keys_box_render();
    mt_gotoXY(1, 25);
}

void shell()
{

    sc_regInit();
    sc_memoryInit();

    currCell.posCol = 0;
    currCell.posRow = 0;

    /*sc_regSet(CLOCKIGNORE, 1);*/

    struct sigaction act;
    act.sa_handler = &sighandler;
    act.sa_flags = SA_RESTART;

    sigemptyset(&act.sa_mask);

    sigaction(SIGALRM, &act, NULL);
    sigaction(SIGUSR1, &act, NULL);

    struct itimerval nval, oval;
    nval.it_interval.tv_sec = 1;
    nval.it_interval.tv_usec = 0;
    nval.it_value.tv_sec = 1;
    nval.it_value.tv_usec = 0;

    setitimer(ITIMER_REAL, &nval, &oval);

    rk_mytermregime(0, 0, 0, 0, 1);

    interface_render();

    int key;

    while (1)
    {
        mt_gotoXY(1, 25);
        rk_readkey(&key);
        switch (key)
        {
        case KEY_up:
            if (currCell.posRow > 0) {
                currCell.posRow--;
                interface_render();
            }
            break;
        case KEY_down:
            if (currCell.posRow < 9) {
                currCell.posRow++;
                interface_render();
            }
            break;
        case KEY_left:
            if (currCell.posCol > 0) {
                currCell.posCol--;
                interface_render();
            }
            break;
        case KEY_right:
            if (currCell.posCol < 9) {
                currCell.posCol++;
                interface_render();
            }
            break;
        case KEY_enter: {
            alarm(0);
            mt_gotoXY(1, 25);
            inputMemory();
            getchar();
            interface_render();
            setitimer(ITIMER_REAL, &nval, &oval);
            break;
        }

        case KEY_s: {
            alarm(0);
            mt_gotoXY(1, 25);
            rk_mytermregime(1, 0, 0, 1, 1);
            printf("file name:\n");
            char fileName[264];
            scanf("%s", fileName);
            getchar();
            rk_mytermregime(0, 0, 0, 0, 1);
            sc_memorySave(fileName);
            interface_render();
            setitimer(ITIMER_REAL, &nval, &oval);
            break;
        }
        
        case KEY_l: {
            alarm(0);
            mt_gotoXY(1, 25);
            rk_mytermregime(1, 0, 0, 1, 1);
            printf("file name:\n");
            char fileName[264];
            scanf("%s", fileName);
            getchar();
            rk_mytermregime(0, 0, 0, 0, 1);
            sc_memoryLoad(fileName);
            interface_render();
            setitimer(ITIMER_REAL, &nval, &oval);
            break;
        }
        case KEY_i: {
            mt_gotoXY(1, 25);
            raise(SIGUSR1);
            break;
        }

        case KEY_f5: {
            alarm(0);
            mt_gotoXY(1, 25);
            inputAccumulator();
            getchar();
            interface_render();
            setitimer(ITIMER_REAL, &nval, &oval);
            break;
        }

        case KEY_f6: {
            alarm(0);
            mt_gotoXY(1, 25);
            inputCounter();
            getchar();
            interface_render();
            setitimer(ITIMER_REAL, &nval, &oval);
            break;
        }

        case KEY_other:
            break;
        }
    }


    
}

void inputMemory()
{
    printf("Input value:\n");
    rk_mytermregime(1, 0, 0, 1, 1);
    int command, operand, result;
    scanf("%2d%2d", &command, &operand);
    int retval = sc_commandEncode(command, operand, &result);
    if (retval != 0)
        printf("Input error");
    else
        sc_memorySet((currCell.posRow * 10 + currCell.posCol), result);

    rk_mytermregime(0, 0, 0, 0, 1);
}


void choiseBigVal(int val, int retVal[2])
{
    switch (val) {
    case 0:
        get_zero(retVal);
        return;
    case 1:
        get_one(retVal);
        return;
    case 2:
        get_two(retVal);
        return;
    case 3:
        get_three(retVal);
        return;
    case 4:
        get_four(retVal);
        return;
    case 5:
        get_five(retVal);
        return;
    case 6:
        get_six(retVal);
        return;
    case 7:
        get_seven(retVal);
        return;
    case 8:
        get_eight(retVal);
        return;
    case 9:
        get_nine(retVal);
        return;
    }
}



void reset()
{
    sc_regInit();
    sc_memoryInit();
    sc_accumSet(0);
    sc_counterSet(0);
    interface_render();
}

void inputAccumulator()
{
    printf("Input value:\n");
    rk_mytermregime(1, 0, 0, 1, 1);
    int command, operand, result;
    scanf("%2d%2d", &command, &operand);
    int retval = sc_commandEncode(command, operand, &result);
    if (retval != 0) {
        printf("Input error");
        rk_mytermregime(0, 0, 0, 0, 1);;
    }
    sc_accumSet(result);
    rk_mytermregime(0, 0, 0, 0, 1);
}

void inputCounter()
{
    printf("Input value:\n");
    rk_mytermregime(1, 0, 0, 1, 0);
    int val;
    scanf("%4d", &val);
    if (val > 9999) {
        printf("Input error");
        rk_mytermregime(0, 0, 0, 0, 1);;
    }
    sc_counterSet(val);
    rk_mytermregime(0, 0, 0, 0, 1);
}

void iterCounter() {
    int val, a;
    a = sc_counterGet();
    sc_regGet(CLOCKIGNORE, &val);
    if (!val)
        a++;
    sc_counterSet(a);
}
