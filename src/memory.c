#include <athena/memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void
memory_initialize(Memory *memory, uint32_t size)
{
	memory->size = size;
	memory->physical_data = malloc(size);

	memset(memory->physical_data, 0, size);
}

void
memory_deinitialize(Memory *memory)
{
	memory->size = 0;
	free(memory->physical_data);
}

void
memory_load_from_file(Memory *memory, uint32_t base, uint32_t size, int fd)
{
	if(base + size > memory->size)
	{
		perror("The memory is too small for loading the file");
		exit(1);
	}

	if (read(fd, memory->physical_data + base, size) < 0)
	{
		perror("unable to load file");
		exit(1);
	}
}

uint32_t
memory_load32(Memory *memory, uint32_t address)
{
	uint32_t *data;

	/* fixme: use interruption / error handling instead */
	if(address + sizeof(uint32_t) > memory->size)
	{
		perror("Memory out-of-bound when trying to read value");
		exit(1);
	}

	data = (uint32_t *)(memory->physical_data + address);
	return *data;
}

void
memory_write32(Memory *memory, uint32_t address, uint32_t value)
{
	uint32_t *data;

	/* fixme: use interruption / error handling instead */
	if(address + sizeof(uint32_t) > memory->size)
	{
		perror("Memory out-of-bound when trying to write value");
		exit(1);
	}

	data = (uint32_t *)(memory->physical_data + address);
	*data = value;
}
