// temperature_monitor.c
#include <msp430.h>  
#include "temperature_monitor.h"  
#include "msp430fr4133.h"


#define LED_PIN BIT0  // 定義 P1.0 為 LED 引腳（紅燈）
#define TEMPERATURE_THRESHOLD_C 290  // 設定threshold為 29.0°C
#define TEMPERATURE_THRESHOLD_F 830  // 設定threshold為 83.0°F



void checkTemperatureAndTriggerLED_C(unsigned long current_temp) {
    
    if (current_temp > TEMPERATURE_THRESHOLD_C) {
        P1OUT |= LED_PIN;  // 溫度超過閾值，點亮 P1.0 上的 LED（紅燈）
    } else {
        P1OUT &= ~LED_PIN; // 溫度未超過閾值，保持 LED 關閉
    }
}


void checkTemperatureAndTriggerLED_F(unsigned long current_temp) {
    if (current_temp > TEMPERATURE_THRESHOLD_F) {
        P1OUT |= LED_PIN;  // 溫度超過閾值，點亮 P1.0 上的 LED（紅燈）
    } else {
        P1OUT &= ~LED_PIN; // 溫度未超過閾值，保持 LED 關閉
    }
}
