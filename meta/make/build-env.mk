BUILD_ENV ?= debug

ifeq ($(BUILD_ENV), debug)
	ENV_FLAGS	:= -g -ggdb -O0
else
	ENV_FLAGS	:= -DNDEBUG
endif

ENV_FLAGS	:= -DVERSION=\"0\"
