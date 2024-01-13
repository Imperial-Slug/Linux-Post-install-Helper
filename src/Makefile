CC = gcc
CFLAGS = 
LIBS = -lGL

TARGET = lpih-main
SRC = lpih-main.c

# Check for the Linux distribution
DISTRO := $(shell lsb_release -si)

ifeq ($(DISTRO),Debian)
    CFLAGS += $(shell pkg-config --cflags gtk4)
    LIBS += $(shell pkg-config --libs gtk4)
    DEPENDENCIES = libgtk-4-1 libgtk-4-dev gtk-4-examples
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
