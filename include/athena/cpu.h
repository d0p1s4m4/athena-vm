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

#ifndef ATHENA_VM_CPU_H
# define ATHENA_VM_CPU_H 1

# include <stdint.h>

# define CPU_VENDOR 0x564d564d

typedef enum
{
	OP_REG = 0,
	OP_ADDI,
	OP_ADDIU,
	OP_SUBI,
	OP_SUBIU,
	OP_ANDI,
	OP_ORI,
	OP_XORI,
	OP_STORE,
	OP_LOAD,
	OP_JMP,
	OP_LIH,
	OP_LOADU,
	OP_CALL,
	OP_BEQ,
	OP_BNE,
	OP_BLT,
	OP_BGT,

	OP_REGSPE = 0x3F
} Opcode;

typedef enum
{
	F_NOP,
	F_ADD,
	F_ADDU,
	F_SUB,
	F_SUBU,
	F_AND,
	F_DIV,
	F_DIVU,
	F_MULT,
	F_MULTU,
	F_NOR,
	F_OR,
	F_XOR,
	F_SLL,
	F_SRL,
	F_SRA,
	F_SLLR,
	F_SRAR,
	F_SRLR,
	F_MOD,
	F_MODU,

	F_TRAP = 0x3F
} Func;

typedef enum
{
	SF_ERET = 0,

	SF_MVSRR = 0x3E,
	SF_MVSRW
} SpeFunc;

typedef enum
{
	SR_MACH_ISA = 0,
	SR_VENDOR_ID,
	SR_STATUS,
	SR_TRAP_VECTOR,
	SR_EPC,
	SR_CAUSE,

	SR_END
} SpecialReg;

typedef struct {
	uint32_t registers[32];
	uint32_t special_registers[SR_END];
	uint32_t pc;
} Cpu;


void cpu_initialize(Cpu *cpu);
void cpu_cycle(Cpu *cpu, uint8_t *memory);

#endif /* !ATHENA_VM_CPU_H */
