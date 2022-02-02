ifeq ($(OS), Windows_NT)
	HOST_OS := windows
else
	UNAME_SYSTEM := $(shell uname -s)
	ifeq ($(UNAME_SYSTEM), Linux)
		HOST_OS := gnulinux
	endif
	ifeq ($(UNAME_SYSTEM), Darwin)
		HOST_OS := darwin
	endif
endif

ifeq ($(HOST_OS), windows)
	HOST_ARCH := unknown
else
	UNAME_ARCH := $(shell uname -m)
	HOST_ARCH := $(UNAME_ARCH)
endif

HOST_INFO_FLAGS := -DHOST_OS=\"$(HOST_OS)\" -DHOST_ARCH=\"$(HOST_ARCH)\"
