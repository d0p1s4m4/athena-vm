#include <stdio.h>

#include <athena/cpu.h>
#include <athena/opt.h>
#include <athena/dump.h>

static const char *registers_name[CPU_REG_COUNT] = 
{
	DUMP_REGISTERS_NAME
};

static const char *pretty_registers_name[CPU_REG_COUNT] = 
{
	DUMP_REGISTERS_PRETTY_NAME
};

void
dump_registers(Cpu *cpu, OptDumpRegs format)
{
	size_t idx;
	const char **regname;

	if (format == DUMP_REGS_NONE)
	{
		return;
	}
	else if (format == DUMP_REGS_NORMAL)
	{
		regname = registers_name;
	}
	else
	{
		regname = pretty_registers_name;
	}

	for (idx = 0; idx < CPU_REG_COUNT; idx++)
	{
		printf("\033[32m%s\033[0m: %06X ", regname[idx], cpu->registers[idx]);
		if (idx % 5 == 0 && idx > 0)
		{
			printf("\n");
		}
	}
	printf("\n");
}