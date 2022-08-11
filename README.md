# Grove sensors w/ Seeduino V4.2

Testing out Grove VOC+CO2 sensor and temp monitor.

The [SGP30 library](https://github.com/Seeed-Studio/SGP30_Gas_Sensor) is used as dependency.

TODO: run baseline calibration

## Limited RAM

Was running short of RAM, did not get readings from Gas sensor, as it most likely crashed in RAM shortage.

But realized the fixed print strings take memory. Found the Arduino [F()](https://www.baldengineer.com/arduino-f-macro.html) macro to the rescue.
