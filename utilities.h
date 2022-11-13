//This module has all common functions
#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#define USER "/sys/class/gpio/gpio72/value"


void runCommand(char *command);
bool readUser();
long long getTimeInMs(void);
void sleepForMs(long long delayInMs);

#endif