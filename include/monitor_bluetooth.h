#ifndef __MONITOR_BLUETOOTH__
#define __MONITOR_BLUETOOTH__

#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void monitor_bluetooth(void *pvParameters)
{
    Serial.begin(115200);
    SerialBT.begin("Chiec thuyen ngoai xa");
    Serial.println("Bluetooth device started. Pair and connect!");

    
    while (1)
    {
        if (SerialBT.available())
        {
            char cmd = SerialBT.read();
            Serial.print("Received: ");
            Serial.println(cmd);
    
            if (cmd == '1')
            {
                Serial.println("LED on");
                SerialBT.println("LED turned ON");
            }
            else if (cmd == '0')
            {
                Serial.println("LED off");
                SerialBT.println("LED turned OFF");            
            }
            else if (cmd == 't')
            {
                Serial.println("LED TOGGLED");
                SerialBT.println("LED toggled");            
            }
            
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}






#endif