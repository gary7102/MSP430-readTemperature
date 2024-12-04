// write_to_fram.h
#ifndef WRITE_TO_FRAM_H
#define WRITE_TO_FRAM_H 

void writeTemperatureToFRAM_celsius(unsigned long temperature);
void writeTemperatureToFRAM_fahrenheit(unsigned long temperature);
signed short readTemperatureFromFRAM();

#endif // WRITE_TO_FRAM_H

