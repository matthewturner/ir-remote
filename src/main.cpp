#include "main.h"

void setup()
{
  Serial.begin(115200);

  pinMode(POWER_SWITCH, OUTPUT);
  pinMode(EXTERNAL_POWER_MONITOR, INPUT);

  attachInterrupt(digitalPinToInterrupt(EXTERNAL_POWER_MONITOR), onInterrupt, FALLING);

  stateMachine.when(POWERING_UP, (EvtAction)poweringUp, WAITING);
  stateMachine.when(WAITING, (EvtAction)waiting);
  stateMachine.when(POWERING_DOWN, (EvtAction)poweringDown, POWERED_DOWN);
  stateMachine.when(POWERED_DOWN, (EvtAction)poweredDown, NO_TRANSITION, POWERING_UP, 500);
  stateMachine.whenInterrupted(WAITING, POWERING_DOWN);

  mgr.addListener(&stateMachine);

  Serial.println(F("Setup complete. Continuing..."));
}

bool poweringUp()
{
  enableBackupPower();
  sendOnSignal();
  return true;
}

bool poweringDown()
{
  sendOffSignal();
  disableBackupPower();
  return true;
}

bool waiting()
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
  return true;
}

void loop()
{
  mgr.loopIteration();
}

void onInterrupt()
{
  stateMachine.onInterrupt();
}