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

    enableBackupPower();
}

void loop()
{
    if (externalPowerConnected())
    {
        sendOnSignalIfRequired();
        delay(500);
        return;
    }

    Serial.println("External power loss detected");

    sendOffSignal();
    pauseBeforePowerDown();
    disableBackupPower();
    waitForEnd();

    // will reach here if power is resumed
    enableBackupPower();
}

bool externalPowerConnected()
{
    return (digitalRead(EXTERNAL_POWER_MONITOR) == HIGH);
}

void sendOnSignalIfRequired()
{
    if (!turnedOn)
    {
        sendOnSignal();
    }
}

void pauseBeforePowerDown()
{
    Serial.println("Powering down in 1 seconds...");
    delay(1000);
}

void disableBackupPower()
{
    Serial.println("Disabling backup power...");
    digitalWrite(POWER_SWITCH, LOW);
}

void enableBackupPower()
{
    Serial.println("Enabling backup power...");
    digitalWrite(POWER_SWITCH, HIGH);
}

void sendOnSignal()
{
    Serial.println("Sending ON signal...");
    for (byte i = 0; i < 3; i++)
    {
        IrSender.sendNEC(0xFF00FF, 32);
        delay(300);
    }
    turnedOn = true;
}

void sendOffSignal()
{
    Serial.println("Sending OFF signal...");
    for (byte i = 0; i < 3; i++)
    {
        IrSender.sendNEC(0xFF40BF, 32);
        delay(300);
    }
    turnedOn = false;
}

void waitForEnd()
{
    Serial.println("Waiting for the end...");
    delay(2000);
}
