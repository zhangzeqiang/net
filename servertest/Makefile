BIN=bin
SRC=src

INC=/home/c/.vim/tags/libevent-1.4.13-stable/:${PWD}/src
TOOLS=g++

obj := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

main:${obj}
	${TOOLS} -o $@ $^ -levent -I ${INC}

# %cpp:%o
#	${TOOLS} -o $@ $<   

clean: 
	rm *.o

