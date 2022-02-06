#include "TinyXtra.h"

#if defined(__AVR_ATtiny85__)  
  #define wdReg WDTCR
#else
  #define wdReg WDTCSR
#endif

// Delay for serial debug print at baud 2400 with cpu at 8 mhz.
#ifndef TEXTRA_DBG_DELAY
#define TEXTRA_DBG_DELAY 417
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
  serialTx = -1;
}

void TinyXtra::sleep() {
  sleep_enable ();
  sleep_cpu ();
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

void TinyXtra::setupInterrupt(int i) {
  if ((i<0) || (5<i)) return;
  cli();                // Disable interrupts during setup
  PCMSK |= (1 << i);    // Enable interrupt handler (ISR) for our chosen interrupt pin (PCINT1/PB1/pin 6)
  GIMSK |= (1 << PCIE); // Enable PCINT interrupt in the general interrupt mask
  sei();                // last line of setup - enable interrupts after setup
}

void TinyXtra::disableAdc() {
  ADCSRA &= ~(1<<ADEN); //Disable ADC, saves ~230uA
}

void TinyXtra::enableAdc() {
  ADCSRA |= (1<<ADEN);
}

void TinyXtra::dbgSetTxPin(uint8_t tx)
{
  serialTx = tx;
  pinMode(tx, OUTPUT);
  digitalWrite(tx, HIGH);
}

void TinyXtra::dbgString(char *s) {
  while (*s) {
    dbgChar(*s);
    s++;
  }
}

void TinyXtra::dbgChar(char c) {
//  const int txdel = TEXTRA_SERIAL_DELAY_MS;
  if (serialTx < 0) return;
  digitalWrite(serialTx, LOW);
  delayMicroseconds(TINY_XTRA_SERIAL_DELAY_MS);
  for (byte mask = 0x01; mask; mask <<= 1)
  {
    if (c & mask) // choose bit
      digitalWrite(serialTx, HIGH);
    else
      digitalWrite(serialTx, LOW);

    delayMicroseconds(TINY_XTRA_SERIAL_DELAY_MS);
  }

  digitalWrite(serialTx, HIGH);// restore pin to natural state

  delayMicroseconds(TINY_XTRA_SERIAL_DELAY_MS);
}
