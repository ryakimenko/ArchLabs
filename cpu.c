#include "cpu.h"

int ALU(int command, int operand)
{
	int accum_val = sc_accumGet();
	int comm, oper;
	sc_commandDecode(accum_val, &comm, &oper);
	int res_val;

	switch (command)
	{
	case 30:
		oper += operand;
		sc_commandEncode(comm, oper, &res_val);
		sc_accumSet(res_val);
		return 0;
	case 31:
		oper -= operand;
		sc_commandEncode(comm, oper, &res_val);
		sc_accumSet(res_val);
		return 0;
	case 32:
		oper *= operand;
		sc_commandEncode(comm, oper, &res_val);
		sc_accumSet(res_val);
		sc_accumSet(sc_accumGet() * operand);
		return 0;
	case 33:
		oper /= operand;
		sc_commandEncode(comm, oper, &res_val);
		sc_accumSet(res_val);
		return 0;
	case 76:
		return 0;
	default:
		break;
	}
	return -1;
}

int CU()
{
	int mem_val, command, operand;
	sc_memoryGet(sc_counterGet(), &mem_val);
	if (sc_commandDecode(mem_val, &command, &operand) != 0)
	{
		sc_regSet(CLOCKIGNORE, 1);
		return -1;
	}
	if ((command >= 30 && command <= 33) || command == 76)
		ALU(command, operand);
	else {
		switch (command)
		{
		case 10:

		case 11:

		case 20:

		case 21:

		case 40:

		case 41:

		case 42:

		case 43:

		default:
			break;
		}
	}
	return 0;
}
