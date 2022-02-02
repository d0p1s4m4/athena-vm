# Copyright (C) 2022 d0p1 <contact[at]d0p1.eu>
#
# This file is part of miwps-vm.
#
# miwps-vm is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# miwps-vm is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with miwps-vm.  If not, see <http://www.gnu.org/licenses/>.

.DEFAULT_GOAL := all

CC	?= gcc

CROSS_CC	= 
CROSS_LD	=

RM	= rm -f

-include meta/make/build-env.mk
-include meta/make/host-info.mk

CFLAGS = -Iinclude/ -Wall -Wextra -Werror -ansi -pedantic -Wno-variadic-macros \
			$(ENV_FLAGS) $(HOST_INFO_FLAGS)
LDFLAGS	= 

SRCS	= main.c
OBJS	= $(addprefix src/, $(SRCS:.c=.o))

TARGET = miwps-vm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(OBJS)
	$(RM) $(TARGET)

re: clean all
