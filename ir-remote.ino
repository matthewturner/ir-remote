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
        // turn on
        IrSender.sendNEC(0xFF00FF, 32);
        turnedOn = true;
    }

    if (digitalRead(EXTERNAL_POWER_MONITOR) == HIGH)
    {
        delay(1000);
    }
    else
    {
        // turn off
        IrSender.sendNEC(0xFF40BF, 32);
        delay(2000);
        digitalWrite(POWER_SWITCH, LOW);
    }
}
