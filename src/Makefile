# MAKEFILE FOR LPIH
#  Run "make" in your terminal from this file's directory to execute the build/compile instructions in here.

CC = clang
CFLAGS = -Wall -g -Wextra -Werror -I../include -Iinclude
LIBS = -lGL
TARGET = lpih
SRC = lpih-main.c deb-window.c fed-window.c utility.c

# Check for the Linux distribution
DISTRO := $(shell grep '^ID=' /etc/os-release | cut -d'=' -f2)

ifeq ($(DISTRO),debian)
    CFLAGS += $(shell pkg-config --cflags gtk4)
    LIBS += $(shell pkg-config --libs gtk4)
    DEPENDENCIES = libgtk-4-1 libgtk-4-dev
else ifeq ($(DISTRO),ubuntu)
    CFLAGS += $(shell pkg-config --cflags gtk4)
    LIBS += $(shell pkg-config --libs gtk4)
    DEPENDENCIES = libgtk-4-1 libgtk-4-dev
else ifeq ($(DISTRO),fedora)
    CFLAGS += $(shell pkg-config --cflags gtk4)
    LIBS += $(shell pkg-config --libs gtk4)
    DEPENDENCIES = gtk4 gtk4-devel mesa-libGL-devel 
else ifneq ($(DISTRO),fedora)
    CFLAGS += $(shell pkg-config --cflags gtk4)
    LIBS += $(shell pkg-config --libs gtk4)
endif

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

install-dependencies:
ifeq ($(DISTRO),Debian)
	sudo apt install -y $(DEPENDENCIES)
else ifeq ($(DISTRO),Fedora)
	sudo dnf install -y $(DEPENDENCIES)
endif

clean:
	rm -f $(TARGET)


    
