#include <signal.h>

#ifndef GLOBALS_H
#define GLOBALS_H

pid_t otherPid;
int signalsReceived;
volatile sig_atomic_t waitForSignal;

#endif