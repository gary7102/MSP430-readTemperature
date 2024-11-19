// write_to_fram.c
#include <msp430.h>
#include "write_to_fram.h" 


// #define TEMPSUM_SNAPSHOT_cel 0x1830  // 定義 FRAM_celcius 存儲溫度數據的地址
#define TEMPSUM_SNAPSHOT_cel 0x1800
#define VALID_FLAG_ADDRESS 0x1868  // 定義有效標記的地址
#define VALID_FLAG_VALUE 6666  // check VALID_FLAG_ADDRESS for valid


/* 以下為存入整數 */
void writeTemperatureToFRAM_celsius(signed short temperature) {
    SYSCFG0 &= ~DFWP;                      // Disable FRAM write protection
    
    // 寫入溫度數據
    unsigned long *FRAM_write_ptr = (unsigned long *)TEMPSUM_SNAPSHOT_cel;
    *FRAM_write_ptr = temperature;         // Write temperature to FRAM
    
    // 寫入有效標記
    unsigned long *valid_flag_ptr = (unsigned long *)VALID_FLAG_ADDRESS;
    *valid_flag_ptr = VALID_FLAG_VALUE;
    
    SYSCFG0 |= DFWP;                       // Enable FRAM write protection
}
/*
void writeTemperatureToFRAM_fahrenheit(signed short temperature) {
    SYSCFG0 &= ~DFWP;                      // Disable FRAM write protection
    // *(signed short*)TEMPSUM_SNAPSHOT_fah = temperature;
    unsigned long *FRAM_write_ptr = (unsigned long *)TEMPSUM_SNAPSHOT_fah;
    *FRAM_write_ptr = temperature;         // Write temperature to FRAM
    SYSCFG0 |= DFWP;                       // Enable FRAM write protection
}
*/
signed short readTemperatureFromFRAM() {
    // 先檢查有效標記
    signed short *valid_flag_ptr = (signed short *)VALID_FLAG_ADDRESS;
    if (*valid_flag_ptr == VALID_FLAG_VALUE) {
        // 有效標記(VALID_FLAG_ADDRESS)存在，讀取溫度數據
        signed short *FRAM_read_ptr = (signed short *)TEMPSUM_SNAPSHOT_cel;
        return *FRAM_read_ptr;
    } else {
        // 無效標記，表示數據無效
        return -1;  // 或返回其他無效值
    }
}
