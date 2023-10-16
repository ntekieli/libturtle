CXX = clang++
CXXFLAGS = -g -Os -std=c++17 -I../libembroidery/ -Wall -Wextra -pedantic -fPIC
targets := $(patsubst %.cpp,lib%.so,$(wildcard *.cpp))

all: ${targets}

lib%.so: %.o
	clang++ -shared -undefined dynamic_lookup -o $@ $<

clean:
	rm ${objects} ${targets}
