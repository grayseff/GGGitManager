CC = clang
CFLAGS = -Wall -Wextra -pedantic $(shell pkg-config --cflags gtk4)
LIBS = $(shell pkg-config --libs gtk4) 

PREFIX ?= $(HOME)/.local
DATADIR = $(PREFIX)/share/gggitmanager

SRC = src/main.c \
	  src/window.c \
	  src/git.c \
	  src/buttons.c \
	  src/repository.c
		


gggitmanager: $(SRC)
	$(CC) $(CFLAGS) -DGGGM_DATADIR=\"$(DATADIR)\" $(SRC) -o gggitmanager $(LIBS)
gggm: gggitmanager
	./gggitmanager

clean:
	rm -f gggitmanager

install: gggitmanager
	mkdir -p $(PREFIX)/bin
	mkdir -p $(DATADIR)
	mkdir -p $(PREFIX)/share/applications
	mkdir -p $(PREFIX)/share/icons/hicolor/scalable/apps
	cp gggitmanager $(PREFIX)/bin/gggitmanager
	cp data/style.css $(DATADIR)/style.css
	cp data/com.gray.gggitmanager.desktop \
	    $(PREFIX)/share/applications/com.gray.gggitmanager.desktop
	cp data/com.gray.gggitmanager.svg \
	    $(PREFIX)/share/icons/hicolor/scalable/apps/com.gray.gggitmanager.svg

uninstall:
	rm -f $(PREFIX)/bin/gggitmanager
	rm -f $(PREFIX)/share/applications/com.gray.gggitmanager.desktop
	rm -f $(PREFIX)/share/icons/hicolor/scalable/apps/com.gray.gggitmanager.svg
