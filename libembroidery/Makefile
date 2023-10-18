CC = clang
CFLAGS = -fPIC -fcommon
objects := $(patsubst %.c,%.o,$(wildcard *.c))

targets := libembroidery.a

all: ${targets}

libembroidery.a: ${objects}
	ar r libembroidery.a ${objects}

clean:
	rm ${objects} ${targets}
