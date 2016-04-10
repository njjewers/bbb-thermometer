all: digits

digits: digits.c sevenseg_driver_bin.h
	gcc -g -o digits -lprussdrv digits.c

sevenseg_driver_bin.h: sevenseg_driver.p sevenseg_driver.hp
	pasm -V3 -c -Csevenseg_driver sevenseg_driver.p

.PHONY: clean
clean:
	rm digits sevenseg_driver_bin.h


