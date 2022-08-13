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

#ifndef ATHENA_VM_DUMP_H
# define ATHENA_VM_DUMP_H 1

# include <stdint.h>

# define DUMP_REGISTERS_NAME \
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", \
    "r9", "r10", "r11", "r12", "13", "r14", "r15", "r16", "r17", \
    "r18", "r19", "r20", "r21", "r22", "r23", "r24", "r25", "r26", \
    "r27", "r28", "r29", "r30", "r31"

# define DUMP_REGISTERS_PRETTY_NAME \
    "zero", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7" \
    "a0", "a1", "r11", "r12", "13", "r14", "r15", "r16", "r17", \
    "r18", "r19", "r20", "r21", "r22", "r23", "r24", "k0", "k1", \
    "at", "sp", "gp", "fp", "ra"

void dump_registers(Cpu *, OptDumpRegs);

#endif /* !ATHENA_VM_DUMP_H */
