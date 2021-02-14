#include <stdio.h>

#ifndef MEMORY_H
#define MEMORY_H
#define OVERFLOW 0b00000001 // overflow occured in action
#define ZEROERROR 0b00000010 // division by 0
#define OUTMEMORY 0b00000100 // out of memory
#define CLOCKIGNORE 0b00001000 // ignore impulse
#define COMMANDERROR 0b00010000 // wrong command

int sc_memoryInit();
int sc_memorySet(int address, int value);
int sc_memoryGet(int address, int* value);
int sc_memorySave(char* filename);
int sc_memoryLoad(char* filename);
int sc_regInit();
int sc_regSet(int reg, int value);
int sc_regGet(int  reg, int* value);
void sc_printReg();

#endif // !MEMORY_H
