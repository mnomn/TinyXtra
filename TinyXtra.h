#ifndef TINY_XTRA_H
#define TINY_XTRA_H

#include <avr/sleep.h>
#include <avr/wdt.h>
#include "Arduino.h"    

/*
Based on
http://arduino.stackexchange.com/questions/19501/watchdog-approach
https://thewanderingengineer.com/2014/08/11/pin-change-interrupts-on-attiny85/
*/

class TinyXtra {
public:
    TinyXtra();
    void sleep_8s();
    void setupInterrupt0();
    static volatile bool interrupt;
};

#endif //TINY_XTRA_H
