#include <LowPower.h>
#include <IRremote.h>

const byte POWER_SWITCH = 8;
const byte EXTERNAL_POWER_MONITOR = 2;

const byte POWERING_UP = 0;
const byte WAITING = 1;
const byte POWERING_DOWN = 2;
const byte POWERED_DOWN = 3;
volatile byte state = 0;

// sends on pin 3
IRsend IrSender;

void setup()
{
    Serial.begin(9600);

    pinMode(POWER_SWITCH, OUTPUT);
    pinMode(EXTERNAL_POWER_MONITOR, INPUT);

    attachInterrupt(digitalPinToInterrupt(EXTERNAL_POWER_MONITOR), wakeup, LOW);

    enableBackupPower();

    Serial.println(F("Setup complete. Continuing..."));
}

void loop()
{
  switch(state)
  {
    case POWERING_UP:
      sendOnSignal();
      break;
    case WAITING:
      sleep();
      break;
    case POWERING_DOWN:
      sendOffSignal();
      disableBackupPower();
      break;
    case POWERED_DOWN:
      waitForEnd();
      break;
  }
}

void wakeup()
{
  if (state == WAITING)
  {
    setState(POWERING_DOWN);
  }
}

bool sleep()
{
  Serial.println(F("Sleeping..."));
  Serial.flush();
  LowPower.idle(SLEEP_FOREVER, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,
                SPI_OFF, USART0_OFF, TWI_OFF);
  return true;
}

bool externalPowerConnected()
{
    return (digitalRead(EXTERNAL_POWER_MONITOR) == HIGH);
}

void disableBackupPower()
{
    Serial.println(F("Disabling backup power..."));
    digitalWrite(POWER_SWITCH, LOW);
    setState(POWERED_DOWN);
}

void enableBackupPower()
{
    Serial.println(F("Enabling backup power..."));
    digitalWrite(POWER_SWITCH, HIGH);
    setState(POWERING_UP);
}

void sendOnSignal()
{
    Serial.println(F("Sending ON signal..."));
    for (byte i = 0; i < 3; i++)
    {
        IrSender.sendNEC(0xFF00FF, 32);
        delay(300);
    }
    setState(WAITING);
}

void sendOffSignal()
{
    Serial.println(F("Sending OFF signal..."));
    for (byte i = 0; i < 3; i++)
    {
        IrSender.sendNEC(0xFF40BF, 32);
        delay(300);
    }
    setState(POWERING_DOWN);
}

void waitForEnd()
{
    if (externalPowerConnected())
    {
      enableBackupPower();
    }
    else
    {
      Serial.println(F("Waiting for the end..."));
      delay(500); 
    }
}

void setState(byte newState)
{
  Serial.print(F("Setting state: "));
  switch (newState)
  {
  case POWERING_UP:
    Serial.println(F("POWERING_UP"));
    break;
  case WAITING:
    Serial.println(F("WAITING"));
    break;
  case POWERING_DOWN:
    Serial.println(F("POWERING_DOWN"));
    break;
  case POWERED_DOWN:
    Serial.println(F("POWERED_DOWN"));
    break;
  }
  state = newState;
}
