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

#ifndef ATHENA_VM_MEMORY_H
# define ATHENA_VM_MEMORY_H 1

# include <stdint.h>

/* Maybe you may want to use something else than just binary data
 * but it will be easier to implement an MMU later on.
 */

typedef struct {
	uint32_t size;
	uint8_t *physical_data;
} Memory;

void memory_initialize(Memory *memory, uint32_t size);

void memory_deinitialize(Memory *memory);

void memory_load_from_file(Memory *memory, uint32_t base, uint32_t size, int fd);

uint32_t memory_load32(Memory *memory, uint32_t address);

void memory_write32(Memory *memory, uint32_t address, uint32_t value);

uint8_t* memory_ptr(Memory *memory, uint32_t address);

#endif /* !ATHENA_VM_MEMORY_H */
