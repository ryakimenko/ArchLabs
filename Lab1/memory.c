#include "memory.h"
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#define MEMORY_SIZE 100
#define MAX_VALUE (1 << 15) - 1
#define OVERFLOW 0b00000001 // overflow occured in action
#define ZEROERROR 0b00000010 // division by 0
#define OUTMEMORY 0b00000100 // out of memory
#define CLOCKIGNORE 0b00001000 // ignore impulse
#define COMMANDERROR 0b00010000 // wrong command
static int memory[MEMORY_SIZE];
static int flags;

int sc_memoryInit()
{
    return memset(memory, 0, MEMORY_SIZE), 0;
}

int sc_memorySet(int address, int value)
{
    if (address > MEMORY_SIZE || address < 0)
    {
        sc_regSet(OUTMEMORY, 1);
        return -1;
    }

    if (value > MAX_VALUE)
    {
        return -2;
    }

    memory[address] = value;
    return 0;
}

int sc_memoryGet(int address, int* value)
{
    if (address > MEMORY_SIZE || address < 0)
    {
        sc_regSet(OUTMEMORY, 1);
        return -1;
    }

    *value = memory[address];
    return *value;
}

int sc_memorySave(char* filename)
{
    FILE* file = fopen(filename, "wb");
    int flag = fwrite(memory, sizeof(int), MEMORY_SIZE, file);
    if (!flag)
        return -1;
    fclose(file);
    return 0;
}

int sc_memoryLoad(char* filename)
{
    FILE* file = fopen(filename, "rb");
    int flag = fread(memory, sizeof(int), MEMORY_SIZE, file);
    if (!flag)
        return -1;
    fclose(file);
    return 0;
}

int sc_regInit()
{
    flags = 0;
    return 0;
}

int sc_regSet(int reg, int value)
{
    if (reg > 16 || reg < 1)
        return -1;
    if (value == 0)
        flags = flags & ~reg;
    else if (value == 1)
        flags = flags | reg;
    else return -2;

    return 0;
}

int sc_regGet(int  reg, int* value)
{
    if (reg > 16 || reg < 1)
        return -1;
    if ((flags & reg) == 0)
        *value = 0;
    else
        *value = 1;
    return 0;
}

void sc_printReg() {
    printf("%d", flags);
}
