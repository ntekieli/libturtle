CC = clang
CFLAGS = -fPIC -fcommon
objects := $(patsubst %.c,%.o,$(wildcard *.c))

targets := libembroidery.so

all: ${targets}

libembroidery.so: ${objects}
	clang -shared -undefined dynamic_lookup -o libembroidery.so ${objects}

clean:
	rm ${objects} ${targets}
