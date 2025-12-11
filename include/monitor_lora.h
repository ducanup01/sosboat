#ifndef __MONITOR_LORA__
#define __MONITOR_LORA__

#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

// ----- Pin mapping -----
#define LORA_SCK   14
#define LORA_MISO  12
#define LORA_MOSI  13
#define LORA_CS    15
#define LORA_RST   -1
#define LORA_DIO0  -1

// Declare SPI instance (defined in .cpp)
extern SPIClass loraSPI;

// ----- Functions -----
void monitor_lora(void *pvParameters);
void handleLoRaCommand(String input);

// NEW: Serial helper function
void monitor_serial_input();

// ----- LoRa helper functions (defined elsewhere) -----
String LoRaRead();
void LoRaPrintln(const String &msg);

#endif
