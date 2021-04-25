#include "terminal.h"
#include <stdio.h>
#include "shell.h"
#include "disk.h"
#include <stdlib.h>

int main()
{

	tCHS* chs = malloc(20);
	tLBA* lba = malloc(32);
	tLARGE* large = malloc(24);
	tIDECHS* idechs = malloc(28);
	int h, s, c;
	int choose;
	int section_size = 0;
	int i = 0;
	int free_space = 8192;

	typedef struct {
		int active;
		tLBA* l_start;
		tCHS* c_start;
		tCHS* finish;
		uint32_t size;
	}Table;

	Table table[128];

	char type;

	printf("Input IDECHS geometry\n");
	printf("C: ");
	scanf("%d", &c);
	printf("H: ");
	scanf("%d", &h);
	printf("S: ");
	scanf("%d", &s);

	idechs->c = c;
	idechs->h = h;
	idechs->s = s;

	g_idechs2lba(idechs, lba);

	double disk_size =  (lba->s* 512)/pow(2,30);

	printf("Disk size = %lf Gb\n", disk_size);

	printf("Choose type:\n1 - FAT32    2 - Linux swap    3 - HPFS\\NTFS\n");
	scanf("%d", &choose);

	printf("Free space = %d\n", free_space);

	table[i].c_start = malloc(20);
	table[i].finish = malloc(20);
	table[i].l_start = malloc(32);
	table[i].l_start->s = 1;
	g_lba2chs(table[i].l_start, table[i].c_start);

	while (free_space - section_size > 0)
	{
		printf("Input section size: ");
		scanf("%d", &section_size);
		if (section_size == 0)
			break;
		if (free_space - section_size < 0)
			break;
		table[i].size = section_size;
		if (i != 0) {
			table[i].c_start = malloc(20);
			table[i].finish = malloc(20);
			table[i].l_start = malloc(32);
			table[i].l_start->s = table[i - 1].l_start->s + table[i - 1].size;
			g_lba2chs(table[i].l_start, table[i].c_start);
		}
		free_space -= section_size;
		lba->s = section_size + table[i].l_start->s;
		g_lba2chs(lba, table[i].finish);
		printf("Set this section as active:\n Yes - 1\n No -  0\n");
		scanf("%d", &table[i].active);
		if ((table[i].active == 1) && (i != 0)) {
			for (int j = 0; j < i; j++) {
				table[j].active = 0;
			}
		}
		printf("Free space: %d\n\n", free_space);
		section_size = 0;
		i++;
	}
	for (int j = 0; j < i; j++)
	{
		printf("Active: %d\n", table[j].active);
		printf("CHS start: %d | %d | %d\n", table[j].c_start->c, table[j].c_start->h, table[j].c_start->s);
		printf("Type: \n", &choose);
		printf("CHS Finish: %d | %d | %d\n", table[j].finish->c, table[j].finish->h, table[j].finish->s);
		printf("Size: %d | %d\n\n", table[j].l_start->s, table[j].size);
	}

	//shell();
	return 0;
}