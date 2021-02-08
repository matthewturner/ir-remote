#include <IRremote.h>

#define POWER_SWITCH 8
#define EXTERNAL_POWER_MONITOR 12

// sends on pin 3
IRsend IrSender;
bool turnedOn = false;

void setup()
{
    Serial.begin(9600);

    pinMode(POWER_SWITCH, OUTPUT);
    pinMode(EXTERNAL_POWER_MONITOR, INPUT);

    digitalWrite(POWER_SWITCH, HIGH);
}

void loop()
{
    if (!turnedOn)
    {
        Serial.println("Sending ON signal...");
        IrSender.sendNEC(0xFF00FF, 32);
        turnedOn = true;
    }

    if (digitalRead(EXTERNAL_POWER_MONITOR) == HIGH)
    {
        Serial.println("External power detected");
        delay(1000);
    }
    else
    {
        Serial.println("External power loss detected");

        Serial.println("Sending OFF signal...");
        IrSender.sendNEC(0xFF40BF, 32);

        Serial.println("Powering down in 7 seconds...");
        delay(7000);
        Serial.println("Powering down...");
        digitalWrite(POWER_SWITCH, LOW);

        Serial.println("Waiting for the end...");
        delay(2000);
    }
}
