#include <LibPrintf.h>
#include "manchester.h"  // Include Manchester encoding library used for RF transmission

/*
ECET 251 Final Project

Transmitter

This transmitter reads a pot a value and converts it to a voltage, when a button is pressed
the pot value is transmitted using a 433MHz RF module. The receiver reads the data and then 
adjusts an led's brightness accordingly.
*/

/*
  Manchester Transmitter example

  In this example transmitter will send 10 bytes array per transmission
  try different speeds using this constants, your maximum possible speed will
  depend on various factors like transmitter type, distance, microcontroller speed, ...
  MAN_300 0
  MAN_600 1
  MAN_1200 2
  MAN_2400 3
  MAN_4800 4
  MAN_9600 5
  MAN_19200 6
  MAN_38400 7
*/

#define TX_PIN 9     // Pin where the RF transmitter data pin is connected
#define GREEN_LED 4  // Pin connected to the Green LED (indicates system ready)
#define BLUE_LED 5   // Pin connected to the Yellow LED (indicates transmission)
#define button 3     // Pin where the active-HIGH pushbutton is connected
#define pot A7       // Pin where potentiometer is connected.

#define datalength 7    // Size of the data array being transmitted
#define delayTime 2000  // delay time (milliseconds) between transmissions, stops spamming of messages.
int count = 0;

uint8_t data[3] = { 0, 0, 0 };  // Data array that will be transmitted over RF
unsigned int lastTransmit = 0;  // last time data was transmitted.

// ****** Declaring Functions *******
uint8_t get_datalength();

// ****** Setup Function *******
void setup() {
  printf("Starting Setup\n");

  // Configure button pin as input
  pinMode(button, INPUT);
  // Configure pot pin as input
  pinMode(pot, INPUT);

  // Configure Green LED pin as an output
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, LOW);  // Start with LED off

  // Configure BLUE LED pin as an output
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(BLUE_LED, LOW);  // Start with LED off


  // Flash both LEDs 5 times at startup as a power-up indicator
  for (byte i = 0; i < 5; i++) {
    digitalWrite(BLUE_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    delay(100);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, LOW);
    delay(100);
  }
  // Required workaround if using 1 MHz ATtiny chips
  //man.workAround1MhzTinyCore();

  // Initialize Manchester transmitter
  // TX_PIN = transmitter data pin
  // MAN_300 = transmission speed (300 baud Manchester encoded)
  man.setupTransmit(TX_PIN, MAN_300);

  Serial.begin(9600);
  printf("Setup Done\n");
  printf("Main Loop now running:\n");
}

// ****** Main Loop *******
void loop() {

  uint16_t pot_value = analogRead(pot);

  unsigned int now = millis();

  // If button has been pressed AND the allocated time has passed.
  // This is to stop spamming of button presses.
  if (digitalRead(button) && now - lastTransmit >= delayTime) {
    // turn green off to show button pressed.
    digitalWrite(GREEN_LED, LOW);

    data[0] = get_datalength();
    data[1] = lowByte(pot_value);  // splitting 16-bit number into 2 8-bit number
    data[2] = highByte(pot_value);

    count++;
    man.transmitArray(get_datalength(), data);  // transmitting data
    printf("Count = %d\n", count);
    printf("Data length: %d\n", get_datalength());
    for (int i = 0; i < get_datalength(); i++) {
      printf("Data[i] = %d\n", i);
    }
    printf("\n");

    // Blink the Blue LED 5 times to indicate data was transmitted
    for (int i = 0; i < 5; i++) {
      digitalWrite(BLUE_LED, 1);
      delay(100);
      digitalWrite(BLUE_LED, 0);
      delay(100);
    }
    lastTransmit = now;  // updating last transmission time.
  }

  // if allocated time has passed, turn green led on to indicate another transmission can be sent.
  else if (now - lastTransmit >= delayTime) {
    digitalWrite(GREEN_LED, HIGH);
  }
}


// ****** Declaring Functions *******
uint8_t get_datalength() {
  return sizeof(data) / sizeof(data[0]);
}
