# IrrigaTron3000
Automatic night-time plant watering using an Arduino, LDR, temperature sensor, soil moisture sensor and solenoid.

Automatically waters the plants in the greenhouse at night using a combination of LDR (light-dependent resistor),
temperature sensor, soil moisture sensor and a relay board which can switch a solenoid valve on and off.

Some measure of intelligence is built-in:
* Watering only occurs at night, to prevent watering while the sun would heat up the wet soil and damage the roots.
* Determines whether it's growing season based on maximum daytime temperature.
* Refuses to water when night-time temperature falls below a certain level (5°C), to prevent damaging the pipes.

## To do:
* Implement soil moisture measurement as to not over-water the plants.
* Figure out a way to hook up window actuators, to open/close the roof windows based on the internal temperature.
* Maybe add a status display, if I can find one that won't be damaged from sitting in the sun all day long.
