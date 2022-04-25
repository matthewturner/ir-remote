#include "main.h"

void setup()
{
  Serial.begin(115200);

  pinMode(POWER_SWITCH, OUTPUT);
  pinMode(EXTERNAL_POWER_MONITOR, INPUT);

  attachInterrupt(digitalPinToInterrupt(EXTERNAL_POWER_MONITOR), wakeup, LOW);

  Serial.println(F("Setup complete. Continuing..."));
}

void loop()
{
  switch (state)
  {
  case POWERING_UP:
    enableBackupPower();
    sendOnSignal();
    setState(WAITING);
    break;
  case WAITING:
    sleep();
    break;
  case POWERING_DOWN:
    sendOffSignal();
    disableBackupPower();
    setState(POWERED_DOWN);
    break;
  case POWERED_DOWN:
    if (externalPowerConnected())
    {
      setState(POWERING_UP);
    }
    else
    {
      waitForEnd();
    }
    break;
  }

  delay(500);
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
}

void enableBackupPower()
{
  Serial.println(F("Enabling backup power..."));
  digitalWrite(POWER_SWITCH, HIGH);
}

void sendOnSignal()
{
  Serial.println(F("Sending ON signal..."));
  for (byte i = 0; i < 3; i++)
  {
    // IrSender.sendNEC(0xFF00FF, 32);
    delay(300);
  }
}

void sendOffSignal()
{
  Serial.println(F("Sending OFF signal..."));
  for (byte i = 0; i < 3; i++)
  {
    // IrSender.sendNEC(0xFF40BF, 32);
    delay(300);
  }
}

void waitForEnd()
{
  Serial.println(F("Waiting for the end..."));
  delay(500);
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
