# MAKEFILE FOR LPIH
#  Run "make" in your terminal from this file's directory to execute the build/compile instructions in here.

CC = gcc
CFLAGS = -Wall -g -Wextra -Werror -Iinclude
LIBS = -lGL
TARGET = lpih
SRC = lpih-main.c


# Check for the Linux distribution
DISTRO := $(shell lsb_release -si)

ifeq ($(DISTRO),Debian)
    CFLAGS += $(shell pkg-config --cflags gtk4)
    LIBS += $(shell pkg-config --libs gtk4)
    DEPENDENCIES = libgtk-4-1 libgtk-4-dev
else ifeq ($(DISTRO),Fedora)
    CFLAGS += $(shell pkg-config --cflags gtk4)
    LIBS += $(shell pkg-config --libs gtk4)
    DEPENDENCIES = gtk4 gtk4-devel mesa-libGL-devel
else
    CFLAGS += $(shell pkg-config --cflags gtk4)
    LIBS += $(shell pkg-config --libs gtk4)
endif

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

install-dependencies:
ifeq ($(DISTRO),Debian)
	sudo apt-get install -y $(DEPENDENCIES)
else ifeq ($(DISTRO),Fedora)
	sudo dnf install -y $(DEPENDENCIES)
endif

clean:
	rm -f $(TARGET)
