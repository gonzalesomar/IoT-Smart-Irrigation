/*
  Description:
*/

#include <LowPower.h>

const int moisturePin1 = A1; // Adjust pins as per your connections
const int moisturePin2 = A2;
const int moisturePin3 = A3;
const int interruptPin = A0; // ADC_0 pin
const int threshold = 150;

void setup() {

  Serial.begin(9600);
  
  // Initialize your sensor pins
  pinMode(moisturePin1, INPUT);
  pinMode(moisturePin2, INPUT);
  pinMode(moisturePin3, INPUT);

  // Attach interrupt for waking up
  attachInterrupt(digitalPinToInterrupt(interruptPin), wakeUp, RISING);
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(2, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {

  // Read moisture levels
  //int moisture1 = analogRead(moisturePin1);
  //int moisture2 = analogRead(moisturePin2);
  //int moisture3 = analogRead(moisturePin3);


  digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(3000);                       // wait for a second
  digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
  delay(3000);                       // wait for a second


  // Turn off ADC
  ADCSRA &= ~(1<<ADEN);

  // Disable the analog comparator
  ACSR |= (1<<ACD);
  
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

  // Turn on ADC
  ADCSRA |= (1<<ADEN);
}


void wakeUp() {
  // This function will be called once the interrupt is triggered
  // No need to put any code here unless you want to perform a specific action upon waking up
  detachInterrupt(digitalPinToInterrupt(interruptPin)); // Detach interrupt to avoid repeated triggering
}
