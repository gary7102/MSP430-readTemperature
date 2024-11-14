# Intro
**2024 NCU Fall新興記憶儲存系統元件設計 Assignment 1**  

1. Writing a Simple Linux Driver and Recompiling the Kernel  
2. Collecting and Processing Temperature Data on MSP430 FR4133 using FRAM 

# Question 1

<font size = 4>**Setting up the Environment:**</font>
```
uname -r

sudo apt install build-essential linux-headers-$(uname -r)
```
![image](https://hackmd.io/_uploads/BJm33ci-1x.png)

<font size = 4>**確認 linux-headers 套件已正確安裝於開發環境**</font>  
```
dpkg -L linux-headers-`uname -r` | grep "/lib/modules/.*/build"
```
<font size = 4>**Expected Result:**</font>  
```
/lib/modules/6.8.0-40-generic/build
```

---

<font size = 4>**“Hello World” kernel module and Makefile**</font>  


```
# 創建一個名叫 kernel_modules 的資料夾
mkdir kernel_modules

# 把目錄轉到 kernel_modules 資料夾
cd kernel_modules
```
<font size = 4>**建立hello.c**</font>  
```
#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/kernel.h> 

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("gary");
MODULE_DESCRIPTION("A simple Hello World module"); 

static int __init hello_init(void) { 
    printk(KERN_INFO "Hello, world!\n"); 
    return 0; 
} 
static void __exit hello_exit(void) { 
    printk(KERN_INFO "Goodbye, world!\n"); 
} 
module_init(hello_init); 
module_exit(hello_exit); 
```  
<font size = 4>**建立Makefile**</font>
```
obj-m += hello.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```   

<font size = 4>**Compiling module**</font>  
這邊編譯模組只需執行 make
```
make
```
<font size = 4>**Result**</font>  
![image](https://hackmd.io/_uploads/ryqCOjj-Jx.png)

成功後會產出許多檔案，這邊我們會用到的只有 `hello.ko`  
![image](https://hackmd.io/_uploads/ryxltosZyx.png)

<font size = 4>**Load the module**</font>  
產生 `hello.ko` 之後，可將其掛載:
```
sudo insmod hello.ko
```
<font size = 4>**使用 dmesg 來查看 kernel 內的訊息**</font>  
```
sudo dmesg
```
![image](https://hackmd.io/_uploads/B1zNiss-1x.png)  

<font size = 4>**Unload module**</font>  
```
sudo rmmod hello

demsg
```
![image](https://hackmd.io/_uploads/ry8Rqjo-yg.png)

---  

<font size = 4>**Build the module as a built-in part of the kernel**</font>  

先去下載下載 Kernel Source
```
wget -P ~/ https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-6.8.1.tar.xz


# 把檔案解壓縮到 /usr/src 目錄底下
tar -xvf linux-6.8.1.tar.xz -C /usr/src
```
<font size = 4>**將hello.c 移至drivers/misc**</font>

```
cp hello.c /usr/src/linux-6.8.1/drivers/misc
```

<font size = 4>**修改 drivers/misc/Makefile**</font>  
```
obj-y += hello.o
```
![螢幕擷取畫面 2024-11-09 201431](https://hackmd.io/_uploads/BkwSzRn-1l.png)

告訴kernel compile時需要將 hello.o 內建到最終的kernel中，而不是作為loadable module  

<font size = 4>**修改 `Kconfig` 文件**</font>  

可以在`Kconfig`中加上:
```
config HELLO_MODULE
    bool "Hello World built-in module"
    default y
```
這樣可以讓這個module成為內建選項並且默認啟用

<font size = 4>**Kernel Configuration**</font>  
```
make menuconfig
```
在Device Drivers -> Misc devices中可以找到剛剛加入的"Hello World built-in module"  
確認是要呈現`[*]`(啟用)就可以了  
![image](https://hackmd.io/_uploads/Bk6hI3sZ1x.png)

<font size = 4>**Compile kernel**</font>  

![image](https://hackmd.io/_uploads/rJztWRnZye.png)

```
reboot
```
記得選擇最新版本  

<font size = 4>**使用dmesg看kernel訊息**</font>  
```
dmesg | grep "Hello, world!"
```
![image](https://hackmd.io/_uploads/ryf1RTnbkg.png)

這樣一來使得hello world kernle module在每次系統啟動時都會自動載入，成為kernel的一部分，而不需要手動使用 `insmod` 來載入模組。


<font size = 4>**demonstrate that the module loads automatically at boot or via manual insertion (modprobe)**</font>  
使用 `modprobe` 手動插入模組  

`modprobe` 是用來載入loadable module的，它會在`make modules_install`之後，在 `/lib/modules/$(uname -r)/` 目錄下尋找對應的 `.ko` 文件

![image](https://hackmd.io/_uploads/Bk55cCnb1e.png)  

`modprobe` 在執行後沒有任何輸出，這表示系統並未找到 `hello.ko` 文件，但它也不會報錯，因為該模組已經存在於kernel中作為內建部分   
 
可以依據 `dmesg` 的輸出來確認模組的確是內建的。模組的初始化訊息 "Hello, world!" 出現在系統啟動的早期階段（例如 `[1.457667]` 時間戳），這表示它是在kernel啟動過程中被執行，而不是通過 `modprobe` 載入的

為了驗證可以使用 `sudo modprobe -r hello` 嘗試來卸載，
<font size = 4>**結果:**</font>  
![image](https://hackmd.io/_uploads/SJKSuRhWJl.png)  

顯示module 已經是built-in module因此無法卸載，因此確認了hello 為built-in module

---


# Question 2

文件最後有[Demo](##Demo)

這題我們需要去，[CCSSTUDIO](https://www.ti.com/tool/CCSTUDIO#downloads) 下載`CCSTUDIO-THEIA`，因為舊版的`CCSTUDIO` somehow不知道為甚麼build不出來，

![upload_3e8c1c44613eac8d5d0b00cd9286fee3](https://hackmd.io/_uploads/Hkpwj-QM1x.png)  

在框起來的地方找到`OutOfBox_MSP430FR4133`，這就是第一次將板子接上電源的時候出現Welocome、stopWatch mode、tempersensorMode的程式碼

## 新增 寫入FRAM的funciton

新增`write_to_fram.h`及`write_to_fram.c`
:::spoiler `write_to_fram.h`
```
// write_to_fram.h
#ifndef WRITE_TO_FRAM_H
#define WRITE_TO_FRAM_H 

void writeTemperatureToFRAM_celsius(signed short temperature);
void writeTemperatureToFRAM_fahrenheit(signed short temperature);
signed short readTemperatureFromFRAM();

#endif // WRITE_TO_FRAM_H
```
:::

:::spoiler `write_to_fram.c`
```
// write_to_fram.c
#include <msp430.h>
#include "write_to_fram.h" 


#define TEMPSUM_SNAPSHOT_cel 0x1830  // 定義 FRAM_celcius 存儲溫度數據的地址
// #define TEMPSUM_SNAPSHOT_fah 0x1830
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

```
:::  

並且在 `TempSensorMode.c` 中加入: `writeTemperatureToFRAM_celsius(*degC);`，用來把`degC`寫入FRAM當中

接下來就可以去CCS中view->memory中察看memory資料，寫入的地址前面有寫到，在0x1800這個位址

![S__34045954](https://hackmd.io/_uploads/rJ_HJzQf1e.jpg)

![螢幕擷取畫面 2024-11-14 134628](https://hackmd.io/_uploads/rJ3aAZQGkx.png)

這邊的279代表27.9度C，因為無法寫入floating point，只能寫入int or hexdecimal  
也可以看到在0x1834中的6666也是我用來測試是否正確寫入地址的資料



## 斷電reboot檢查FRAM資料還存在  
接下來要把板子斷電再重新接上電源檢查資料有正確寫入且具有non-volatile特性能夠正常留在FRAM

這邊先把剛剛的`writeTemperatureToFRAM_celsius(*degC);`註解掉，避免寫入新資料把舊的刷掉，並且新增`signed short lastTemp = readTemperatureFromFRAM();`這個function  

![image](https://hackmd.io/_uploads/B1E9zzmMkg.png)  


拔掉電源之後再重新build一次  

![螢幕擷取畫面 2024-11-14 140201](https://hackmd.io/_uploads/B1G_zfXG1e.png)  

可以看到在`writeTemperatureToFRAM_celsius()`關掉的情況下，在左邊的variables視窗中 `lastTemp` 確實是279，且在memory中0x1800中實際資料也是279，另外在0x1834的地方6666也還存在著，如此一來確認了儘管**斷電後再接上電源**，資料確實會留存再FRAM當中


## Set a threshold 

### 攝氏溫度
設定一個threshold使current temperature大於這個threshold的時候能夠亮燈，我這邊是設定若大於29.0°C，則亮紅燈，否則不亮燈

```
#define TEMPERATURE_THRESHOLD 290  // 設定threshold為 29.0°C
```

新增一個`temperature_monitor.h`及`temperature_monitor.c`
:::spoiler `temperature_monitor.h`
```
// temperature_monitor.h
#ifndef TEMPERATURE_MONITOR_H
#define TEMPERATURE_MONITOR_H

void checkTemperatureAndTriggerLED(unsigned short current_temp);

#endif // TEMPERATURE_MONITOR_H
```
:::

:::spoiler `temperature_monitor.c`
```
// temperature_monitor.c
#include <msp430.h>  
#include "temperature_monitor.h"  
#include "msp430fr4133.h"

#define LED_PIN BIT0  // 定義 P1.0 為 LED 引腳（紅燈）
#define TEMPERATURE_THRESHOLD 290  // 設定閾值為 29.0°C

void checkTemperatureAndTriggerLED(unsigned short current_temp) {
    if (current_temp > TEMPERATURE_THRESHOLD) {
        P1OUT |= LED_PIN;  // 溫度超過閾值，點亮 P1.0 上的 LED（紅燈）
    } else {
        P1OUT &= ~LED_PIN; // 溫度未超過閾值，保持 LED 關閉
    }
}
```
:::

![螢幕擷取畫面 2024-11-14 163432](https://hackmd.io/_uploads/BkJ4INQzke.png)


<font size = 4>**執行結果:**</font>


![S__34045960_0](https://hackmd.io/_uploads/S1v9IVmGJx.jpg)  
可以看到大於29.0°C時紅燈亮起  
![S__34045963_0](https://hackmd.io/_uploads/HJIoLNXMye.jpg)  
小於29.0°C紅燈不亮

### 華氏溫度
華氏溫度threshold設定83.0°F，
```
#define TEMPERATURE_THRESHOLD_F 830  // 設定threshold為 83.0°F
```
![image](https://hackmd.io/_uploads/Bkb3jN7Gyl.png)
<font size = 4>**執行結果:**</font>

![S__34045969_0](https://hackmd.io/_uploads/r19a9N7f1g.jpg)
同樣大於83.0°F紅燈亮起
![S__34045971_0](https://hackmd.io/_uploads/rJ5pqEXGkl.jpg)
小於83.0°F紅燈不亮

## Demo

* Continuous writing into FRAM [Demo](https://youtube.com/shorts/iu9oI-G9iaY)
* Check data validity after power loss [Demo](https://youtube.com/shorts/8-NvmiIOopw)
* Set Threshold [Demo](https://youtube.com/shorts/FPSrvM0eNr0?feature=share)

