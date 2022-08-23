/*
 * Copyright (C) 2022 d0p1 <contact[at]d0p1.eu>
 *
 * This file is part of athena-vm.
 *
 * athena-vm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * athena-vm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with athena-vm.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <endian.h>

#include <athena/cpu.h>
#include <athena/opt.h>
#include <athena/instruction.h>
#include <athena/dump.h>

static const char *registers_name[CPU_REG_COUNT] = 
{
	DUMP_REGISTERS_NAME
};

static const char *pretty_registers_name[CPU_REG_COUNT] = 
{
	DUMP_REGISTERS_PRETTY_NAME
};

static const InstructionInfo instruction[] =
{
	{0x04000000, 0xFC00003F, "add",   INSTR_REG,    NULL},
	{0x00000001, 0x0000003F, "addi",  INSTR_IMM,    NULL},
	{0x00000002, 0x0000003F, "addiu", INSTR_IMM,    NULL},
	{0x08000000, 0xFC00003F, "addu",  INSTR_REG,    NULL},
	{0x14000000, 0xFC00003F, "and",   INSTR_REG,    NULL},
	{0x00000005, 0x0000003F, "andi",  INSTR_IMM,    NULL},
	{0x0000000E, 0x0000FFFF, "b",     INSTR_IMM,    NULL},
	{0x0000000E, 0x0000003F, "beq",   INSTR_IMM,    NULL},
	{0x0000000E, 0x0000F83F, "beqz",  INSTR_IMM,    NULL},
	{0x00000011, 0x0000003F, "bgt",   INSTR_IMM,    NULL},
	{0x00000011, 0x0000F83F, "bgtz",  INSTR_IMM,    NULL},
	{0x00000010, 0x0000003F, "blt",   INSTR_IMM,    NULL},
	{0x00000010, 0x0000F83F, "bltz",  INSTR_IMM,    NULL},
	{0x0000000F, 0x0000003F, "bne",   INSTR_IMM,    NULL},
	{0x0000000F, 0x0000003F, "bnez",  INSTR_IMM,    NULL},
	{0x0000000D, 0x0000003F, "call",  INSTR_JUMP,   NULL},
	{0x18000000, 0xFC00003F, "div",   INSTR_REG,    NULL},
	{0x1C000000, 0xFC00003F, "divu",  INSTR_REG,    NULL},
	{0x0000000A, 0x0000003F, "jmp",   INSTR_JUMP,   NULL},
	{0x00000009, 0x0003003F, "lb",    INSTR_LOAD,   NULL},
	{0x0000000C, 0x0003003F, "lbu",   INSTR_LOAD,   NULL},
	{0x0001000C, 0x0003003F, "lh",    INSTR_LOAD,   NULL},
	{0x00010009, 0x0003003F, "lhu",   INSTR_LOAD,   NULL},
	{0x0000000B, 0x0000F83F, "lih",   INSTR_IMM,    NULL},
	{0x00020009, 0x0003003F, "lw",    INSTR_LOAD,   NULL},
	{0x58000000, 0xFC00003F, "mod",   INSTR_REG,    NULL},
	{0x50000000, 0xFC00003F, "modu",  INSTR_REG,    NULL},
	{0x20000000, 0xFC00003F, "mult",  INSTR_REG4,   NULL},
	{0x24000000, 0xFC00003F, "multu", INSTR_REG4,   NULL},
	{0xF800003F, 0xFC00003F, "mvsrr", INSTR_REG,    NULL},
	{0xFC00003F, 0xFC00003F, "mvsrw", INSTR_REG,    NULL},
	{0x00000000, 0xFFFFFFFF, "nop",   INSTR_SINGLE, NULL},
	{0x28000000, 0xFC00003F, "nor",   INSTR_REG,    NULL},
	{0x2C000000, 0xFC00003F, "or",    INSTR_REG,    NULL},
	{0x00000006, 0x0000003F, "ori",   INSTR_IMM,    NULL},
	{0x00000008, 0x0003003F, "sb",    INSTR_STORE,  NULL},
	{0x00010008, 0x0003003F, "sh",    INSTR_STORE,  NULL},
	{0x34000000, 0xFC00003F, "sll",   INSTR_SHIFT,  NULL},
	{0x40000000, 0xFC00003F, "sllr",  INSTR_REG,    NULL},
	{0x3C000000, 0xFC00003F, "sra",   INSTR_SHIFT,  NULL},
	{0x44000000, 0xFC00003F, "srar",  INSTR_REG,    NULL},
	{0x38000000, 0xFC00003F, "srl",   INSTR_SHIFT,  NULL},
	{0x48000000, 0xFC00003F, "srlr",  INSTR_REG,    NULL},
	{0x0C000000, 0xFC00003F, "sub",   INSTR_REG,    NULL},
	{0x00000003, 0x0000003F, "subi",  INSTR_IMM,    NULL},
	{0x00000004, 0x0000003F, "subiu", INSTR_IMM,    NULL},
	{0x10000000, 0xFC00003F, "subu",  INSTR_REG,    NULL},
	{0x00020008, 0x0003003F, "sw",    INSTR_STORE,  NULL},
	{0xFC000000, 0xFC00003F, "trap",  INSTR_SINGLE, NULL},
	{0x30000000, 0xFC00003F, "xor",   INSTR_REG,    NULL},
	{0x00000007, 0x0000003F, "xori",  INSTR_IMM,    NULL},

	{0, 0, NULL, 0, NULL}
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

void
dump_assembly(Board *board, OptDumpRegs format)
{
	size_t idx;
	const char **regname;
	uint32_t instr;

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

	instr = be32toh(memory_load32(&board->memory, board->cpu.pc ));

	printf("0x%08X:\t", board->cpu.pc);
	for (idx = 0; instruction[idx].name != NULL; idx++)
	{

		if ((instr & instruction[idx].mask) == instruction[idx].opcode)
		{
			switch (instruction[idx].type)
			{
				case INSTR_IMM:
					printf("%s %s, %s, 0x%04X\n",
						instruction[idx].name,
						regname[(instr >> 6) & 0x1F], /* ra */
						regname[(instr >> 11) & 0x1F], /* rb */
						(instr >> 16) & 0xFFFF /* imm */
					);
					break;
				case INSTR_JUMP:
					printf("%s %u(%s)\n",
						instruction[idx].name,
						(instr >> 11) & 0x1FFFFF, /* jump offset */
						regname[(instr >> 6) & 0x1F] /* ra */
					);
					break;
				case INSTR_LOAD:
					printf("%s %s, %u(%s)\n",
						instruction[idx].name,
						regname[(instr >> 6) & 0x1F], /* ra */
						(instr >> 18), /* offset */
						regname[(instr >> 11) & 0x1F] /* rb */
					);
					break;
				case INSTR_STORE:
					printf("%s %u(%s), %s\n",
						instruction[idx].name,
						(instr >> 18), /* offset */
						regname[(instr >> 6) & 0x1F], /* ra */
						regname[(instr >> 11) & 0x1F] /* rb */
					);
					break;
				case INSTR_REG:
					printf("%s %s, %s, %s\n",
						instruction[idx].name,
						regname[(instr >> 6) & 0x1F], /* ra */
						regname[(instr >> 11) & 0x1F], /* rb */
						regname[(instr >> 16) & 0x1F] /* rc */
					);
					break;
				case INSTR_SHIFT:
					printf("%s %s, %s, %d\n",
						instruction[idx].name,
						regname[(instr >> 6) & 0x1F], /* ra */
						regname[(instr >> 11) & 0x1F], /* rb */
						(instr >> 21) & 0x1F /* shamt */
					);
					break;
				case INSTR_REG4:
					printf("%s %s, %s, %s, %s\n",
						instruction[idx].name,
						regname[(instr >> 6) & 0x1F], /* ra */
						regname[(instr >> 11) & 0x1F], /* rb */
						regname[(instr >> 16) & 0x1F], /* rc */
						regname[(instr >> 21) & 0x1F] /* rd */
					);
					break;
				case INSTR_SINGLE:
					printf("%s\n", instruction[idx].name);
					break;

				default:
					break;
			}
			return ;
		}
	}

	printf("???\n");
}
