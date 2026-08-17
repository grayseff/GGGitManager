CC = clang
CFLAGS = -Wall -Wextra -pedantic $(shell pkg-config --cflags gtk4)
LIBS = $(shell pkg-config --libs gtk4) 


SRC = src/main.c \
	  src/window.c \
	  src/git.c \
	  src/repository.c
		


gggitmanager: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o gggitmanager $(LIBS)

gggm: gggitmanager
	./gggitmanager

clean:
	rm -f gggitmanager
