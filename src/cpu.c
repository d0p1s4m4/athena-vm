#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <endian.h>
#include <athena/cpu.h>

static uint32_t
cpu_get_register(Cpu *cpu, uint8_t reg)
{
	if (reg == 0)
	{
		return (0);
	}

	return (cpu->registers[reg]);
}

static void
cpu_set_register(Cpu *cpu, uint8_t reg, uint32_t val)
{
	if (reg != 0)
	{
		cpu->registers[reg] = val;
	}
}


static void
cpu_execute_srformat(Cpu *cpu, uint32_t instr)
{
	uint8_t func;

	func = (instr >> 26) & 0x3F;
	switch (func)
	{
		case SF_ERET:
			cpu->pc = cpu->special_registers[SR_EPC];
			break;

		case SF_MVSRR:
			break;

		case SF_MVSRW:
			break;

		default:
			break;
	}
}

static void
cpu_execute_rformat(Cpu *cpu, uint32_t instr)
{
	uint8_t func;
	uint8_t ra;
	uint8_t rb;
	uint8_t rc;
	uint8_t shamt;
	int32_t i32res;
	int64_t i64res;
	uint64_t u64res;

	func = (instr >> 26) & 0x3F;
	ra = (instr >> 6) & 0x1F;
	rb = (instr >> 11) & 0x1F;
	rc = (instr >> 16) & 0x1F;
	shamt = (instr >> 21) & 0x1F;

	switch (func)
	{
		case F_NOP:
			/* do nothing */
			printf("NOP\n");
			break;

		case F_ADD:
			printf("ADD r%d, r%d, r%d\n", ra, rb, rc);
			if (__builtin_sadd_overflow((int)cpu_get_register(cpu, rb), (int)cpu_get_register(cpu, rc), &i32res))
			{
				exit(-1);
			}
			else
			{
				cpu_set_register(cpu, ra, i32res);
			}
			break;

		case F_ADDU:
			cpu_set_register(cpu, ra, cpu_get_register(cpu, rb) + cpu_get_register(cpu, rc));
			break;

		case F_SUB:
			if (__builtin_ssub_overflow((int)cpu_get_register(cpu, rb), (int)cpu_get_register(cpu, rc), &i32res))
			{
				exit(-1);
			}
			else
			{
				cpu_set_register(cpu, ra, i32res);
			}
			break;

		case F_SUBU:
			cpu_set_register(cpu, ra, cpu_get_register(cpu, rb) - cpu_get_register(cpu, rc));
			break;

		case F_AND:
			cpu_set_register(cpu, ra, cpu_get_register(cpu, rb) & cpu_get_register(cpu, rc));
			break;

		case F_DIV:
			cpu_set_register(cpu, ra, (int32_t)cpu_get_register(cpu, rb) / (int32_t)cpu_get_register(cpu, rc));
			break;

		case F_DIVU:
			cpu_set_register(cpu, ra, (uint32_t)cpu_get_register(cpu, rb) / (uint32_t)cpu_get_register(cpu, rc));
			break;

		case F_MULT:
			i64res = (int64_t)cpu_get_register(cpu, rc) * (int64_t)cpu_get_register(cpu, shamt);
			cpu_set_register(cpu, ra, i64res >> 32);
			cpu_set_register(cpu, rb, i64res & 0xFFFFFFFF);
			break;

		case F_MULTU:
			u64res = (uint64_t)cpu_get_register(cpu, rc) * (uint64_t)cpu_get_register(cpu, shamt);
			cpu_set_register(cpu, ra, u64res >> 32);
			cpu_set_register(cpu, rb, u64res & 0xFFFFFFFF);
			break;

		case F_NOR:
			cpu_set_register(cpu, ra, ~(cpu_get_register(cpu, rb) | cpu_get_register(cpu, rc)));
			break;

		case F_OR:
			cpu_set_register(cpu, ra, cpu_get_register(cpu, rb)| cpu_get_register(cpu, rc));
			break;

		case F_XOR:
			cpu_set_register(cpu, ra, cpu_get_register(cpu, rb) ^ cpu_get_register(cpu, rc));
			break;

		case F_SLL:
			cpu_set_register(cpu, ra, cpu_get_register(cpu, rb) << shamt);
			break;

		case F_SRL:
			break;

		case F_SRA:
			cpu_set_register(cpu, ra, (int32_t)cpu_get_register(cpu, rb) >> shamt);
			break;

		case F_SLLR:
			cpu_set_register(cpu, ra, cpu_get_register(cpu, rb) << cpu_get_register(cpu, rc));
			break;

		case F_SRAR:
			cpu_set_register(cpu, ra, (int32_t)cpu_get_register(cpu, rb) >> (int32_t)cpu_get_register(cpu, rc));
			break;

		case F_SRLR:
			break;

		case F_MOD:
			cpu_set_register(cpu, ra, (int32_t)cpu_get_register(cpu,rb) % (int32_t)cpu_get_register(cpu, rc));
			break;

		case F_MODU:
			cpu_set_register(cpu, ra, (uint32_t)cpu_get_register(cpu,rb) % (uint32_t)cpu_get_register(cpu, rc));
			break;

		case F_TRAP:
			exit(0);
			break;

		default:
			/* TODO: invalid opcode */
			break;
	}
}

void
cpu_cycle(Cpu *cpu, uint8_t *memory)
{
	uint32_t instr;
	uint8_t opcode;
	uint8_t ra;
	uint8_t rb;
	uint16_t imm;
	uint32_t jump_offset;
	int32_t i32res;

	/* fetch */
	instr = be32toh(*(uint32_t *)(memory + cpu->pc));
	printf("[pc: %06X] ", cpu->pc);
	cpu->pc += 4;

	/* decode */
	opcode = instr & 0x3F;
	ra = (instr >> 6) & 0x1F;
	rb = (instr >> 11) & 0x1F;
	imm = (instr >> 16) & 0xFFFF;
	jump_offset = (instr >> 11) & 0x1FFFFF;
	
	/* execute */
	switch (opcode)
	{
		case OP_REG:
			cpu_execute_rformat(cpu, instr);
			break;

		case OP_ADDI:
			printf("ADDI r%d, r%d, %d\n", ra, rb, imm);
			if (__builtin_sadd_overflow((int)cpu_get_register(cpu, rb), (int)imm, &i32res))
			{
				/* TODO: integer overflow */
				printf("overflow: %d + %d\n", cpu_get_register(cpu, rb), imm);
				exit(-1);
			}
			else
			{
				cpu_set_register(cpu, ra, i32res);
			}
			break;

		case OP_ADDIU:
			cpu_set_register(cpu, ra, cpu_get_register(cpu, rb) + (uint32_t)imm);
			break;

		case OP_SUBI:
			printf("SUBI r%d, r%d, %d\n", ra, rb, imm);
			if (__builtin_ssub_overflow((int)cpu_get_register(cpu, rb), (int)imm, &i32res))
			{
				/* TODO: integer overflow */
			}
			else
			{
				cpu_set_register(cpu, ra, i32res);
			}
			break;

		case OP_SUBIU:
			cpu_set_register(cpu, ra, cpu_get_register(cpu, rb) - (uint32_t)imm);
			break;

		case OP_ANDI:
			cpu_set_register(cpu, ra, cpu_get_register(cpu, rb) & (uint32_t)imm);
			break;

		case OP_ORI:
			printf("ORI r%d, r%d, %d\n", ra, rb, imm);
			cpu_set_register(cpu, ra, cpu_get_register(cpu, rb) | (uint32_t)imm);
			break;

		case OP_XORI:
			cpu_set_register(cpu, ra, cpu_get_register(cpu, rb) ^ (uint32_t)imm);
			break;

		case OP_STORE:
			break;

		case OP_LOAD:
			break;

		case OP_JMP:
			printf("JMP r%d, %d\n", ra, jump_offset);
			cpu->pc = cpu_get_register(cpu, ra) + (jump_offset * 4);
			break;

		case OP_LIH:
			cpu_set_register(cpu, ra, (cpu_get_register(cpu, ra) & ~(0xFFFF << 16)) | (((uint32_t)imm) << 16 ));
			break;

		case OP_LOADU:
			break;

		case OP_CALL:
			printf("CALL r%d, %d\n", ra, jump_offset);
			cpu->registers[31] = cpu->pc;
			cpu->pc = cpu_get_register(cpu, ra) + (jump_offset * 4);
			break;

		case OP_BEQ:
			printf("BEQ r%d, r%d, %d\n", ra, rb, imm);
			if (cpu_get_register(cpu, ra) == cpu_get_register(cpu, rb))
			{
				cpu->pc = cpu->pc + (int32_t)(imm * 4);
			}
			break;

		case OP_BNE:
			printf("BNE r%d, r%d, %d\n", ra, rb, imm);
			if (cpu_get_register(cpu, ra) != cpu_get_register(cpu, rb))
			{
				cpu->pc = cpu->pc + (int32_t)(imm * 4);
			}
			break;

		case OP_BLT:
			printf("BLT r%d, r%d, %d\n", ra, rb, imm);
			if ((int32_t)cpu_get_register(cpu, ra) < (int32_t)cpu_get_register(cpu, rb))
			{
				cpu->pc = cpu->pc + (int32_t)(imm * 4);
			}
			break;

		case OP_BGT:
			printf("BGT r%d, r%d, %d\n", ra, rb, imm);
			if ((int32_t)cpu_get_register(cpu, ra) > (int32_t)cpu_get_register(cpu, rb))
			{
				cpu->pc = cpu->pc + (int32_t)(imm * 4);
			}
			break;

		case OP_REGSPE:
			cpu_execute_srformat(cpu, instr);
			break;

		default:
			/* TODO: invalid opcode */
			exit(-1);
			break;
	}
}

void
cpu_initialize(Cpu *cpu)
{
	cpu->special_registers[SR_VENDOR_ID] = CPU_VENDOR;
}

