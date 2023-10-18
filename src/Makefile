CXX = clang++
CXXFLAGS = -g -Os -std=c++17 -I../libembroidery/ -Wall -Wextra -pedantic
targets := $(patsubst %.cpp,lib%.a,$(wildcard *.cpp))

all: ${targets}

lib%.a: %.o
	ar r $@ $<
clean:
	rm ${objects} ${targets}
