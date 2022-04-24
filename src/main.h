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

bool sleep();
bool externalPowerConnected();
void disableBackupPower();
void enableBackupPower();
void sendOnSignal();
void sendOffSignal();
void waitForEnd();
void setState(byte newState);
void wakeup();