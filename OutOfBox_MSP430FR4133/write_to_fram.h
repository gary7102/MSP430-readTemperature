// write_to_fram.h
#ifndef WRITE_TO_FRAM_H
#define WRITE_TO_FRAM_H 

void writeTemperatureToFRAM_celsius(signed short temperature);
void writeTemperatureToFRAM_fahrenheit(signed short temperature);
signed short readTemperatureFromFRAM();

#endif // WRITE_TO_FRAM_H

