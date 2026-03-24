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
uint8_t Rx_num = 0;

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
    printf("Tranmisson Received!\n");
    printf("Message #%d:\n", Rx_num);
    digitalWrite(GREEN_LED, LOW);

    
    for (int i = 0; i < BUFFER_SIZE; i++) {
      printf("Buffer index: %d -> %u\n", i, buffer[i]);
    }
    printf("end of tranmission.\n");

   checksum();

    man.beginReceiveArray(BUFFER_SIZE, buffer);
    delay(500);
    digitalWrite(GREEN_LED, HIGH);
  }
}

// ******* Defining Functions *******
void ledController() {
  /*
  This function reconstructs the original 16-bit potentiometer value
  that was transmitted as two 8-bit bytes, and controls an leds bright accordingly.

  During transmission the 16-bit pot value was split into two parts:
      data[0] → low byte  (bits 0–7)
      data[1] → high byte (bits 8–15)

  These bytes were sent over RF and stored in the receiver buffer.

  To rebuild the original number:
      1. The high byte is shifted left by 8 bits to move it back into
         the upper half of the 16-bit value.
      2. The low byte is combined with the shifted high byte using
         a bitwise OR operation.

  buffer[0] = size/length
  buffer[1] = low byte
  buffer[2] = high byte
  buffer[3] = checksum

  This reconstructs the original potentiometer reading so it can be
  used to control the LED brightness.
  */

  uint16_t pot_value = buffer[1]; //| (buffer[2] << 8);         // recombining 2 8-bits bytes
  //uint8_t scaledPotValue = map(pot_value, 0, 1023, 0, 255);  // scaling for PWM.



  printf("Pot Value = %d\n", pot_value);  // optional messages
  printf("Scaled Pot Value = %d\n", pot_value);

  analogWrite(LED_PIN, pot_value);
}

void checksum(){
  uint8_t calculatedChecksum = buffer[0] + buffer[1] ;

  printf("Calculated checksum = %u\n", calculatedChecksum);
  printf("Received checksum   = %u\n", buffer[2]);

  if (buffer[2] == calculatedChecksum){
    printf("no error\n");
    ledController();
  } else {
    printf("Checksum error - TX ignored\n");
  }
}
