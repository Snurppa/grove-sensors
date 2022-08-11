// Demo code for Grove - Temperature Sensor V1.1/1.2
// Loovee @ 2015-8-26
 
#include <math.h>
 
#define B  4275               // B value of the thermistor
#define R0 100000            // R0 = 100k
#define SENSOR A0     // Grove - Temperature Sensor connect to A0 in Seeeduino V4.2
 
 
float temp_sensor_main()
{
    delay(100);
    const int a = analogRead(SENSOR);
 
    float R = 1023.0/a-1.0;
    R = R0*R;
 
    const float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet
    return temperature;
}
