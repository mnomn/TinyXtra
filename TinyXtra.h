#ifndef TINY_XTRA_H
#define TINY_XTRA_H

#include <avr/sleep.h>
#include <avr/wdt.h>
#include "Arduino.h"    

/*
Based on
http://arduino.stackexchange.com/questions/19501/watchdog-approach
*/

class TinyXtra {
public:
    TinyXtra();
    void sleep_8s();
};

// Call in your setup function
void tiny_sleep_setup();

// Call to put chip to sleep.
void tiny_sleep_8s();

#endif //TINY_XTRA_H
