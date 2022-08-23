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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <athena/opt.h>
#include <athena/cpu.h>
#include <athena/board.h>
#include <athena/dump.h>

#define IS_OPTARG(s, l) argv[index][1] == s || \
					 strcmp(argv[index] + 1, "-" l) == 0

#define IS_OPTLONGARG(l) strcmp(argv[index] + 1, "-" l) == 0

#define GET_OPT_VALUE() impl_opt_get_value(argc, argv, &index)

#define GET_OPT_VALUE_OR_DEFAULT(x) impl_opt_get_value_or_default(argc, argv, &index, x)

static void
show_error_usage(char const *prg_name)
{
	fprintf(stderr, "Error usage: %s [options] <file>\n", prg_name);
	exit(EXIT_FAILURE);
}

static void
show_help(char const *prg_name)
{
	printf("Usage: %s [option] file\n", prg_name);
	printf("Flags:\n");
	printf("\t-h,--help\tDisplay this information.\n");
	printf("\t-j,--jit\tEnable JIT\n");
	printf("Options:\n");
	printf("\t-b,--board <board>\tSpecify board (default: miniathena)");
	exit(EXIT_SUCCESS);
}

static void
show_version(char const *prg_name)
{
	printf("%s v%s\n", prg_name, VERSION);
	printf("Copyright (C) 2022 d0p1.\n");
	printf("License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n");
	printf("This is free software: you are free to change and redistribute it.\n");
	printf("There is NO WARRANTY, to the extent permitted by law.\n");
	exit(EXIT_SUCCESS);
}

static char *
impl_opt_get_value(int argc, char const *argv[], int *index)
{
	*index = *index + 1;

	if (*index >= argc)
	{
		show_error_usage(argv[0]);
	}

	if (argv[*index][0] == '-')
	{
		show_error_usage(argv[0]);
	}

	/* XXX: ugly as fuck */
	return ((char *)argv[*index]);
}

static char *
impl_opt_get_value_or_default(int argc, char const *argv[], int *index, char *fallback)
{
	int tmp;

	tmp = *index + 1;

	if (tmp >= argc)
	{
		return (fallback);
	}

	if (argv[tmp][0] == '-')
	{
		return (fallback);
	}

	*index = tmp;
	/* XXX: ugly as fuck */
	return ((char *)argv[*index]);
}

static int
parse_flags(int argc, char const *argv[], Opt *opt)
{
	int index;
	char *val;

	opt->flags = 0x0;

	for (index = 1; index < argc; index++)
	{
		if (argv[index][0] == '-')
		{
			if (IS_OPTARG('h', "help"))
			{
				show_help(argv[0]);
			}
			else if (IS_OPTARG('v', "version"))
			{
				show_version(argv[0]);
			}
			else if (IS_OPTARG('j', "jit"))
			{
				opt->flags |= OPT_FLAG_JIT;
			}
			else if (IS_OPTARG('b', "board"))
			{
				opt->board = GET_OPT_VALUE();
			}
			else if (IS_OPTLONGARG("dump-asm"))
			{
				val = GET_OPT_VALUE_OR_DEFAULT("normal");

				if (strcmp("normal", val) == 0)
				{
					opt->dump_asm = DUMP_REGS_NORMAL;
				}
				else if (strcmp("pretty", val) == 0)
				{
					opt->dump_asm = DUMP_REGS_PRETTY; 
				}
				else
				{
					opt->dump_asm = DUMP_REGS_NORMAL;
					index--;
				}
			}
			else if (IS_OPTLONGARG("dump-regs"))
			{
				val = GET_OPT_VALUE_OR_DEFAULT("normal");

				if (strcmp("normal", val) == 0)
				{
					opt->dump_regs = DUMP_REGS_NORMAL;
				}
				else if (strcmp("pretty", val) == 0)
				{
					opt->dump_regs = DUMP_REGS_PRETTY; 
				}
				else
				{
					opt->dump_regs = DUMP_REGS_NORMAL;
					index--;
				}
			}
			else
			{
				show_error_usage(argv[0]);
			}
		}
		else
		{
			break;
		}
	}
	return (index);
}

int
load_rom(Opt *opt, char const *rom)
{
	Board board;
	int fd;

	printf("Rom %s\n", rom);

	fd = open(rom, O_RDONLY);
	if (fd < 0)
	{
		return (-1);
	}

	memset(&board, 0, sizeof(Board));

	memory_initialize(&board.memory, 512);
	memory_load_from_file(&board.memory, 0, 512, fd);

	cpu_initialize(&board.cpu);

	while (1)
	{
		dump_assembly(&board, opt->dump_asm);

		cpu_cycle(&board.cpu, &board.memory);

		dump_registers(&board.cpu, opt->dump_regs);
	}

	close(fd);

	return (0);
}

int
main(int argc, char const *argv[])
{
	Opt opt;
	int index;

	if (argc < 2)
	{
		show_error_usage(argv[0]);
	}
	memset(&opt, 0, sizeof(Opt));
	index = parse_flags(argc, argv, &opt);
	if (index >= argc)
	{
		show_error_usage(argv[0]);
	}
	return (load_rom(&opt, argv[index]));
}
