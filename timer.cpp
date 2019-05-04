
#include <Arduino.h>
#include <HardwareSerial.h>

//volatile long millis_ = 0;
long millis_ = 0;

#define DISABLE_INTERRUPTS cli()
#define ENABLE_INTERRUPTS sei()

ISR(TIMER1_COMPA_vect) {
    millis_++;
}

// milliseconds works just like millis
// we have re-implemented it as a project requirement.
// This function is not as precise as millis, so use it
// with caution. i.e. although `millis() % 1000 == 0`
// will work with millis, it will not work with this function
// until several seconds have passed.
// 
unsigned long milliseconds() {
    DISABLE_INTERRUPTS;
    long m = millis_;
    ENABLE_INTERRUPTS;
    return m;
}

// Set up a timer interrupt to trigger every milisecond
// 
// Sources:
// Sparkfun
//  - https://www.youtube.com/watch?v=2kr5A350H7E
//  - https://www.sparkfun.com/datasheets/Components/SMD/ATMega328.pdf
// Tables
//  - http://maxembedded.com/2011/07/avr-timers-ctc-mode/
// 
void setup_timer_interrupts() {
  DISABLE_INTERRUPTS;

  // Reset timer counter control register 1A and 1B since they have default values
  TCCR1A = 0;
  TCCR1B = 0;

  // Clear timer on compare (CTC) wave generation mode (WGM) for timer 1
  // Mode WGM13 WGM12 WGM11 WGM10   Mode of Operation    TOP
  //  4     0     1     0     0           CTC           OCR1A
  TCCR1B |= (1 << WGM12);
  
  // Setting the output compare register
  // Equation from the video:
  //    OCR1A = (0.001 s * 16 MHz) / prescaler * 1,000,000 
  OCR1A = 1000 / 4;

  // Setting the prescaler
  // 
  // CS12 CS11 CS10 Description
  //    0    0    0   stop timer
  //    0    0    1   no prescaling
  //    0    1    0   clock / 8
  //    0    1    1   clock / 16
  //    1    0    0   clock / 256
  //    1    0    1   clock / 1024
  TCCR1B &= ~(1 << CS12);
  TCCR1B |= (1 << CS11);
  TCCR1B |= (1 << CS10);

  // Set timer 1 interrupt mask to enable output compare "A" interrupt for timer 1
  // aka: enable timer 1 compare "A" 
  TIMSK1 = (1 << OCIE1A);
  
  ENABLE_INTERRUPTS;
}
