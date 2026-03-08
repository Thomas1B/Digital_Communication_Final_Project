#include <LibPrintf.h>
#include "manchester.h"

/*

  Manchester Receiver example
  
  In this example receiver will receive array of 10 bytes per transmittion

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

#define RX_PIN 2    // pin connected to receiver
#define LED_PIN A7  // analog pin connected to LED
#define GREEN_LED 3

#define BUFFER_SIZE 2
uint8_t buffer[BUFFER_SIZE];
uint8_t Rx_num = 0;


// ******* Setup Function *******
void setup() {
  Serial.begin(1200);
  printf("Setup Started\n");
  pinMode(LED_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  digitalWrite(LED_PIN, 0);
  man.setupReceive(RX_PIN, MAN_300);
  man.beginReceiveArray(BUFFER_SIZE, buffer);

  digitalWrite(GREEN_LED, HIGH);
  printf("Setup Finished\n");
  printf("Main Loop now running\n");
}

// ******* Main Loop *******
void loop() {

  if (man.receiveComplete()) {
    printf("Tranmisson Received!\n");
    Rx_num++;
    printf("Received... message # \n");
    printf(Rx_num);
    analogWrite(LED_PIN, 255);

    // printf(buffer[0]);
    for (int i = 0; i < BUFFER_SIZE; i++) {
      printf("%u", buffer[i]);
    }
    printf('\n');
    delay(500);
    analogWrite(LED_PIN, 0);
    man.beginReceiveArray(BUFFER_SIZE, buffer);
    printf("end of received tranmission.\n");
  }
}
