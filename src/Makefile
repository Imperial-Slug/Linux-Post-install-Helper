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
ifeq ($(DISTRO),debian)
	@echo ""
	@echo "Checking for required packages..."
	@for pkg in $(DEPENDENCIES); do \
	    dpkg-query -W -f='${Status}' $$pkg 2>/dev/null | grep -q "install ok installed" || { echo "$$pkg is not installed. Installing..."; sudo apt install -y $$pkg; }; \
	done
	@echo ""
else ifeq ($(DISTRO),ubuntu)
	@echo ""
	@echo "Checking for required packages..."
	@for pkg in $(DEPENDENCIES); do \
	    dpkg-query -W -f='${Status}' $$pkg 2>/dev/null | grep -q "install ok installed" || { echo "$$pkg is not installed. Installing..."; sudo apt install -y $$pkg; }; \
	done
	@echo ""
else ifeq ($(DISTRO),fedora)
	@echo ""
	@echo "Checking for required packages..."
	@for pkg in $(DEPENDENCIES); do \
	    rpm -q $$pkg >/dev/null 2>&1 || { echo "$$pkg is not installed. Installing..."; sudo dnf install -y $$pkg; }; \
	done
	@echo ""
else
	@echo ""
	@echo "This distro does not look like Debian or Fedora. If you want to compile this, you will need to install the GTK4 development libraries for your distro."
	@echo ""
endif


$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(TARGET)


    
