DTBOS = W1-00A0.dtbo NJ-PRU-01-00A0.dtbo

all: thermometer dtbo

dtbo_install: dtbo
	cp $(DTBOS) /lib/firmware

.PHONY: dtbo
dtbo: $(DTBOS)

%.dtbo : %.dts
	dtc -O dtb -o $@ -@ -b 0 $<

thermometer: main.c sevenseg_driver_bin.h
	gcc -g -std=c99 -o thermometer -lprussdrv main.c

sevenseg_driver_bin.h: sevenseg_driver.p sevenseg_driver.hp
	pasm -V3 -c -Csevenseg_driver sevenseg_driver.p

.PHONY: clean
clean:
	-rm thermometer sevenseg_driver_bin.h $(DTBOS)


