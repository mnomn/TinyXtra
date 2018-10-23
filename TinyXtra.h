#ifndef TINY_XTRA_H
#define TINY_XTRA_H

#include <avr/sleep.h>
#include <avr/wdt.h>
#include "Arduino.h"    

/*
Based on
http://arduino.stackexchange.com/questions/19501/watchdog-approach
https://thewanderingengineer.com/2014/08/11/pin-change-interrupts-on-attiny85/
Digispark SoftSerial
*/

// Debug serial out delay for baud rate 2400, 8 mhz.
// Define this un "application" to set other value.
#ifndef TINY_XTRA_SERIAL_DELAY_MS
#define TINY_XTRA_SERIAL_DELAY_MS 417
#endif

class TinyXtra {
public:
    TinyXtra();
    void sleep_8s();
    // Enable pin change interrupt on pin 0 - 5.
    // Interrupt set flag TinyXtra::interrupt
    void setupInterrupt(int pin);
    void disableAdc();
    void enableAdc();

    // Debug serial print at 2400 baud, Requires that board is in 8 MHZ
    void dbgSetTxPin(uint8_t tx);
    void dbgString(char *s);
    void dbgChar(char c);
    static volatile bool interrupt;

private:
  int serialTx;
  void tx_pin_write(uint8_t pin_state);
  static inline void tunedDelay(uint16_t delay);
};

#endif //TINY_XTRA_H
