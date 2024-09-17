# MAKEFILE FOR LPIH
#  Run "make" in your terminal from this file's directory to execute the build/compile instructions in here.
# -fsanitize=address

CC = clang
CFLAGS = -Wall -g -Wextra -Werror -flto -I../include -Iinclude
LIBS = -lGL
TARGET = lpih
SRC = lpih-main.c lpih-window.c info-window.c utility.c

# Check for the Linux distribution
DISTRO := $(shell grep '^ID=' /etc/os-release | cut -d'=' -f2)

ifeq ($(DISTRO),debian)
    CFLAGS += $(shell pkg-config --cflags gtk4)
    LIBS += $(shell pkg-config --libs gtk4)
   # DEPENDENCIES = libgtk-4-1 libgtk-4-dev
else ifeq ($(DISTRO),ubuntu)
    CFLAGS += $(shell pkg-config --cflags gtk4)
    LIBS += $(shell pkg-config --libs gtk4)
   # DEPENDENCIES = libgtk-4-1 libgtk-4-dev
else ifeq ($(DISTRO),fedora)
    CFLAGS += $(shell pkg-config --cflags gtk4)
    LIBS += $(shell pkg-config --libs gtk4)
   # DEPENDENCIES = gtk4 gtk4-devel mesa-libGL-devel 
else ifneq ($(DISTRO),fedora)
    CFLAGS += $(shell pkg-config --cflags gtk4)
    LIBS += $(shell pkg-config --libs gtk4)
endif

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(TARGET)


# For hunting memory leaks, run the binary with:
# G_SLICE=debug-blocks valgrind --tool=memcheck --leak-check=full ./lpih
    
