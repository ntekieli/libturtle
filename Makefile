all: libembroidery libturtle demo

FORCE:

libembroidery: FORCE
	cd libembroidery && $(MAKE)

libturtle: FORCE
	cd src && $(MAKE)

demo: FORCE
	cd demo && $(MAKE)

clean: 
	cd demo && $(MAKE) clean
	cd src && $(MAKE) clean
	cd libembroidery && $(MAKE) clean