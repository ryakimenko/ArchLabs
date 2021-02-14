#include "command.h"
#include "memory.h"

int main()
{
	sc_memoryInit();
	sc_regInit();
	int command, operand;
	int value = 0;
	int encoded = sc_commandEncode(40, 34, &value);
	int decoded = sc_commandDecode(value, &command, &operand);
	printf("encode: %d\n value: %d\n decode: %d\n command: %d\n operand: %d\n", encoded, value, decoded, command, operand);
	return 0;
}
