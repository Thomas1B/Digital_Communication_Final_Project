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

#define RX_PIN 2     // pin connected to receiver
#define GREEN_LED 4  // GREEN LED for system status
#define LED_PIN 5    // PWM pin connected to LED (for controlling brightness)

#define BUFFER_SIZE 3
uint8_t buffer[BUFFER_SIZE];

uint16_t Rx_num = 0;
uint16_t error_count = 0;
uint16_t success_count = 0;

// ******* Declaring Function *******
void ledController();
void checksum();

// ******* Setup Function *******
void setup() {
  Serial.begin(1200);
  printf("Setup Started\n");
  pinMode(GREEN_LED, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  man.setupReceive(RX_PIN, MAN_300);
  man.beginReceiveArray(BUFFER_SIZE, buffer);

  analogWrite(LED_PIN, 0);
  digitalWrite(GREEN_LED, HIGH);
  printf("Setup Finished\n");
  printf("Main Loop now running\n");
}

// ******* Main Loop *******
void loop() {
  if (man.receiveComplete()) {
    Rx_num++;
    printf("\nTranmisson Received!\n");
    printf("Message #%d:\n", Rx_num);
    digitalWrite(GREEN_LED, LOW);


    for (int i = 0; i < BUFFER_SIZE; i++) {
      printf("Buffer[%d] = %u\n", i, buffer[i]);
    }
    printf("end of message.\n");

    checksum();

    man.beginReceiveArray(BUFFER_SIZE, buffer);
    delay(500);
    digitalWrite(GREEN_LED, HIGH);
  }
}

// ******* Defining Functions *******
void ledController() {
  // simple function to turn on led.
  uint8_t pot_value = buffer[1];
  analogWrite(LED_PIN, pot_value);
}

void checksum() {
  uint8_t calculatedChecksum = 0;
  for (int i = 0; i < BUFFER_SIZE - 1; i++) {
    calculatedChecksum += buffer[i];
  }

  printf("Cal. checksum = %u\n", calculatedChecksum);
  printf("Rec. checksum = %u\n", buffer[BUFFER_SIZE - 1]);

  if (buffer[2] == calculatedChecksum) {
    success_count++;
    ledController();
  } else {
    error_count++;
  }
  printf("Success Count: %d\n", success_count);
  printf("Error Count: %d\n", error_count);
}
