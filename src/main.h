#include <LowPower.h>
#include <IRremote.h>
#include <Eventually.h>

const byte POWER_SWITCH = 8;
const byte EXTERNAL_POWER_MONITOR = 2;

const byte POWERING_UP = 0;
const byte WAITING = 1;
const byte POWERING_DOWN = 2;
const byte POWERED_DOWN = 3;

// sends on pin 3
IRsend IrSender;
EvtManager mgr;
StateMachineListener stateMachine;

bool waiting();
bool externalPowerConnected();
void disableBackupPower();
void enableBackupPower();
void sendOnSignal();
void sendOffSignal();
bool poweringUp();
bool poweringDown();
bool poweredDown();
void onInterrupt();