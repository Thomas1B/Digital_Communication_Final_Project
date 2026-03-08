#include <manchester.h>

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

#define RX_PIN 11
#define LED_PIN 13


#define BUFFER_SIZE 7
uint8_t buffer[BUFFER_SIZE];
uint8_t Rx_num=0;

void setup() {
  pinMode(LED_PIN, OUTPUT); 
  Serial.begin(1200); 
  digitalWrite(LED_PIN, 0);
  man.setupReceive(RX_PIN, MAN_300 );
  man.beginReceiveArray(BUFFER_SIZE, buffer);


}

void loop() {
	
	
  if (man.receiveComplete()) {
	  
	  Rx_num++;
	  Serial.println("Received... message # ");
	  Serial.print(Rx_num);
	  digitalWrite(LED_PIN, 1);
	  

    man.beginReceiveArray(BUFFER_SIZE, buffer);
    for(int i=0; i<BUFFER_SIZE; i++){
        Serial.print(char(buffer[i]));
    }
    Serial.println();
  }
  delay(500);
  digitalWrite(LED_PIN, 0);
  Serial.println("end");
}
