#include <avr/sleep.h>
#include <avr/wdt.h>

// Define the number of watchdog intervals needed for approximately 10 minutes.
// The watchdog timer can be set to 8 seconds, so 75 intervals make 10 minutes (75 * 8 seconds = 600 seconds).
const int intervalsUntilAction = 75;
volatile int watchdogCounter = 0;


// This ISR will be triggered when the watchdog timer expires
ISR(WDT_vect) {
  if (++watchdogCounter >= intervalsUntilAction) {
    // Reset the counter
    watchdogCounter = 0;

    // Perform the action after waking up
    digitalWrite(LED_BUILTIN, HIGH); // Turn on the LED
    delay(1000); // Keep it on for 1 second
    digitalWrite(LED_BUILTIN, LOW); // Turn off the LED
  }
}


void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  // Configure the Watchdog
  MCUSR &= ~(1 << WDRF); // Clear the reset flag
  WDTCSR |= (1 << WDCE) | (1 << WDE); // Allow changes, disable reset, set timeout to 8 seconds
  WDTCSR = (1 << WDIE) | (1 << WDP3) | (1 << WDP0); // Set interrupt mode and 8-second period

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode to power down
  sleep_enable(); // Enable sleep mode
}


void loop() {
  // Prepare to sleep
  noInterrupts(); // Disable interrupts
  wdt_reset(); // Reset the watchdog
  ADCSRA &= ~(1 << ADEN); // Disable ADC
  power_all_disable(); // Power down all peripherals
  sleep_bod_disable(); // Disable brown-out detection

  interrupts(); // Enable interrupts

  // Go to sleep
  sleep_cpu(); // Put the CPU to sleep

  // The program will continue from here after waking up
  sleep_disable(); // Disable sleep mode
  power_all_enable(); // Re-enable all peripherals
  ADCSRA |= (1 << ADEN); // Re-enable ADC

  // A small delay to prevent possible unwanted rapid re-triggering of WDT (e.g., due to noise)
  delay(100);
}

void goToSleep() {
  sleep_enable(); // Enable sleep mode
  sleep_cpu(); // Go to sleep
  sleep_disable(); // Wake up here
}
