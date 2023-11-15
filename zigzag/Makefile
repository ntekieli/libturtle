all: zigzag

zigzag: zigzag.o
	clang++ zigzag.o ../src/libturtle.a ../src/libpoint.a ../libembroidery/libembroidery.a -o zigzag

zigzag.o: zigzag.cpp
	clang++ -g -c -std=c++1z -Wall -Wextra -pedantic -I../libembroidery zigzag.cpp

clean:
	rm *.o zigzag
