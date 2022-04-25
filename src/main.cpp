#include "main.h"

void setup()
{
  Serial.begin(115200);

  pinMode(POWER_SWITCH, OUTPUT);
  pinMode(EXTERNAL_POWER_MONITOR, INPUT);

  attachInterrupt(digitalPinToInterrupt(EXTERNAL_POWER_MONITOR), wakeup, LOW);

  stateMachine.addState(POWERING_UP, (EvtAction)powerUp, WAITING);
  stateMachine.addState(WAITING, (EvtAction)sleep, WAITING);
  stateMachine.addState(POWERING_DOWN, (EvtAction)powerDown, POWERED_DOWN);
  stateMachine.addState(POWERED_DOWN, (EvtAction)poweredDown, POWERED_DOWN, POWERING_UP);

  mgr.addListener(&stateMachine);

  Serial.println(F("Setup complete. Continuing..."));
}

bool powerUp()
{
  enableBackupPower();
  sendOnSignal();
  return true;
}

bool powerDown()
{
  sendOffSignal();
  disableBackupPower();
  return true;
}

void wakeup()
{
  if (stateMachine.currentState() == WAITING)
  {
    stateMachine.setState(POWERING_DOWN);
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

bool poweredDown()
{
  if (externalPowerConnected())
  {
    return false;
  }

  Serial.println(F("Waiting for the end..."));
  delay(500);
  return true;
}

void loop()
{
  mgr.loopIteration();
}
