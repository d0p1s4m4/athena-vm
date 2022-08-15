BUILD_ENV ?= debug

ifeq ($(BUILD_ENV), debug)
	ENV_CFLAGS	:= -g -ggdb -O0 -fsanitize=address -fsanitize=undefined
	ENV_LDFLAGS	:= -lasan -lubsan
else
	ENV_CFLAGS	:= -DNDEBUG
endif

ENV_CFLAGS	+= -DVERSION=\"0\"
