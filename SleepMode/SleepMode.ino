#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

const int intervalsUntilAction = 1; // Number of watchdog intervals for 10 minutes
volatile int watchdogCounter = 0;
volatile bool ledState = false; // This will keep track of the LED state

// Watchdog Timer Interrupt Service Routine (ISR)
ISR(WDT_vect) {
  if (++watchdogCounter >= intervalsUntilAction) {
    // Reset the counter
    watchdogCounter = 0;
    // Toggle the LED state
    ledState = !ledState; 
    digitalWrite(2, ledState ? HIGH : LOW);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);

  // Configure the Watchdog Timer to 8 seconds
  MCUSR &= ~(1 << WDRF); // Clear the reset flag
  WDTCSR |= (1 << WDCE) | (1 << WDE); // Start timed sequence
  WDTCSR = (1 << WDIE) | (1 << WDP3) | (1 << WDP0); // Set interrupt mode and 8-second interval

  // Set sleep mode to power down
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void loop() {
  // Prepare to sleep by turning off various modules
  noInterrupts(); // Disable interrupts
  wdt_reset(); // Reset the watchdog
  ADCSRA &= ~(1 << ADEN); // Disable ADC
  power_all_disable(); // Power down all peripherals
  sleep_bod_disable(); // Disable brown-out detection

  interrupts(); // Enable interrupts
  sleep_enable(); // Enable sleep mode

  // Go to sleep
  sleep_cpu();

  // The processor will wake up here after the watchdog timer expires

  // Disable sleep and re-enable peripherals
  sleep_disable();
  power_all_enable(); // Re-enable all peripherals
  ADCSRA |= (1 << ADEN); // Re-enable ADC

  // Handle LED state changes outside of ISR
  if (ledState) {
    // If the LED was turned on, keep it on for 1 second, then turn it off
    delay(3000); // Keep it on for 1 second
    digitalWrite(2, LOW); // Turn off the LED
    ledState = false; // Reset the LED state
  }
}
