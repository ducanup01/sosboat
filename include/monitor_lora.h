#ifndef __MONITOR_LORA__
#define __MONITOR_LORA__

#include <Arduino.h>
#include <SPI.h>

// ----- Pin mapping -----
#define LORA_SCK   14
#define LORA_MISO  12
#define LORA_MOSI  13
#define LORA_CS    15
#define LORA_RST   -1
#define LORA_DIO0  -1

// #define LORA_SCK   18
// #define LORA_MISO  19
// #define LORA_MOSI  23
// #define LORA_CS    5 only for testing on lora station

// Declare SPI instance (defined in .cpp)
extern SPIClass loraSPI;

// Task function declaration
void monitor_lora(void *pvParameters);

#endif
