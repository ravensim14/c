# $ g++ -o bin/app src/main.cc
#
# or

CC = g++

src = $(shell find src -type f -name '*.cc')
obj += $(patsubst src/%.cpp, obj/%.o, $(src))
dirs += $(patsubst src/%, obj/%, $(shell find src -type d))

dirs += bin
dirs += obj
dirs += lib

libs += -L lib
libs += -I glad/include

libs += -ldl
libs += -lGL
libs += -lglfw
libs += -lglad

all: $(dirs) lib/libglad.a bin/app

bin:
	mkdir -p $@

obj:
	mkdir -p $@

lib:
	mkdir -p $@

lib/libglad.a: glad/src/glad.cc
	$(CC) -c $^ -o glad.o -I glad/include
	ar rvs $@ glad.o

bin/app: $(obj)
	$(CC) -o $@ $^ $(libs)

obj/%.o: src/%.cpp
	$(CC) -c $< -o $@ $(libs)

obj/%: src/%
	mkdir -p $@

clean:
	rm -r obj
