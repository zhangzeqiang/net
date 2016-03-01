BIN=bin
SRC=src

INC=/home/c/.vim/tags/libevent-1.4.13-stable/:${PWD}/src
TOOLS=g++

cpp_obj := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
c_obj := $(patsubst %.c,%.o,$(wildcard *.c))

pkg_cobj := $(patsubst pkg/%.c,pkg/%.o,$(wildcard pkg/*.c))
pkg_cppobj := $(patsubst pkg/%.cpp,pkg/%.o,$(wildcard pkg/*.cpp))

include_cobj := $(patsubst include/%.c,include/%.o,$(wildcard include/*.c))
include_cppobj := $(patsubst include/%.cpp,include/%.o,$(wildcard include/*.cpp))

main:${cpp_obj} ${c_obj} ${pkg_cobj} ${pkg_cppobj} ${include_cobj} ${include_cppobj}
	${TOOLS} -o $@ $^ -levent -I ${INC}

# %cpp:%o
#	${TOOLS} -o $@ $<   

install: 
	sh install.sh

clean: 
	rm *.o
	rm pkg/*.o
	rm include/*.o

