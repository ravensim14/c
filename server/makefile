# $ g++ -o bin/app src/main.cc
#
# or

CC = g++

src = $(shell find src -type f -name '*.cc')
obj += $(patsubst src/%.cpp, obj/%.o, $(src))
dirs += $(patsubst src/%, obj/%, $(shell find src -type d))

dirs += obj

libs += -L ../lib
libs += -I ../glad/include

libs += -ldl
libs += -lGL
libs += -lglfw
libs += -lglad

all: $(dirs) ../bin/server

obj:
	mkdir -p $@

lib:
	mkdir -p $@

../bin/server: $(obj)
	$(CC) -o $@ $^ $(libs)

obj/%.o: src/%.cpp
	$(CC) -c $< -o $@ $(libs)

obj/%: src/%
	mkdir -p $@

clean:
	rm -r obj
