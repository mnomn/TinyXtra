#include "TinyXtra.h"

#if defined(__AVR_ATtiny85__)  
  #define wdReg WDTCR
#else
  #define wdReg WDTCSR
#endif

// Define needed to handle watchdog
ISR (WDT_vect) {
  wdt_disable();  // disable watchdog
}  // end of WDT_vect

ISR(PCINT0_vect) {
  TinyXtra::interrupt = true;
}

volatile bool TinyXtra::interrupt = false;

TinyXtra::TinyXtra(){
  wdt_reset();  
  set_sleep_mode (SLEEP_MODE_PWR_DOWN);
}

void TinyXtra::sleep_8s() {
  noInterrupts ();       // timed sequence coming up
  // pat the dog
  wdt_reset();  

  // clear various "reset" flags
  MCUSR = 0;     
  // allow changes, disable reset, clear existing interrupt
  wdReg = bit (WDCE) | bit (WDE) | bit (WDIF);
  // set interrupt mode and an interval (WDE must be changed from 1 to 0 here)
  wdReg = bit (WDIE) | bit (WDP3) | bit (WDP0);    // set WDIE, and 8 seconds delay

  sleep_enable ();       // ready to sleep
  interrupts ();         // interrupts are required now
  sleep_cpu ();          // sleep                
  sleep_disable ();      // precaution
}

void TinyXtra::setupInterrupt0() {
  GIMSK = 0b00100000;    // turns on pin change interrupts
  PCMSK = 0b00010001;    // turn on interrupts on pins PB0
  sei();                 // enables interrupts
}

