CC = g++

LIBS += `pkg-config --libs --cflags gtk+-3.0`

all: app

app: main.o
	$(CC) -o $@ $^ $(LIBS)

main.o: main.cc
	$(CC) -c -o $@ $^ $(LIBS)

clean:
	rm -f app *.o
