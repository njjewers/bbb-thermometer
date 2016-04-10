# Beaglebone Black Thermometer

Not a particularly impressive project, but it was a good learning experience to play around with Linux device tree overlays and the like. Hopefully other people can take some of the lessons from this project as a jumping-off point for using the PRUSS subsystems on the BeagleBone Black.

Temperature input is taken from a DS18B20 one-wire thermometer through the sysfs interface.

The temperature is displayed on a cheap multiplexed common cathode 7-segment display. This display is driven using the AM335x's Programmable Realtime Units to take this logic off of the CPU.

The default pin configuration is for a [Knight Lights Dual Digit Display](http://www.mainelectronics.com/KLDualDigitLED.htm) (though mine is labelled incorrectly...), but it can be remapped without having to recompile the driver.

BBB Pin | P9.31 | P9.29 | P9.30 | P9.28 | P9.42 | P9.27 | P9.41 | P9.25 |  P8.12 | P8.11 |
PRU0 Pin | r30_0 | r30_1 | r30_2 | r30_3 | r30_4 | r30_5 | r30_6 | r30_7 | r30_14 | r30_15 | 
KDD Pin | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 |



## Dependencies

- [AM335x_pru_package](https://github.com/beagleboard/am335x_pru_package)
- Compiled device tree overlays in /lib/firmware

## Usage

1. Configure the BeagleBone Black using the Device Tree Overlays.
```bash
echo W1 > /sys/devices/bone_capemgr.9/slots
echo NJ-PRU-01 > /sys/devices/bone_capemgr.9/slots
```
2. run `./thermometer`, `^C` to quit.

## References

- Derek Molloy's excellent Exploring Beaglebone
- [DS18B20 device tree overlay cribbed from BoneBrews.com.](http://www.bonebrews.com/temperature-monitoring-with-the-ds18b20-on-a-beaglebone-black/)
