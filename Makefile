rpi-kt0803l: rpi-kt0803l.c rpi-kt0803l.h
	gcc -o rpi-kt0803l rpi-kt0803l.c -O2 -l wiringPi -Wall

.PHONY: install
install:
	install -m 755 rpi-kt0803l /usr/local/bin
