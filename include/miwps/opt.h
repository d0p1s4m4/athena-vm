/*
 * Copyright (C) 2022 d0p1 <contact[at]d0p1.eu>
 *
 * This file is part of miwps-vm.
 *
 * miwps-vm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * miwps-vm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with miwps-vm.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MIWPS_OPT_H
# define MIWPS_OPT_H 1

# include <stdint.h>

typedef struct {
	uint32_t flags;
	char *board;
} Opt;

#endif /* !MIWPS_OPT_H */
