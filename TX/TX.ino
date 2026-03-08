#include "manchester.h"


/*
  Manchester Transmitter example

  In this example transmitter will send 10 bytes array  per transmittion
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

#define TX_PIN  11          //pin where your transmitter is connected
#define GRN_LED  4          //pin for Green LED
#define YLO_LED  5          //pin for Yellow LED
#define datalength   7      //this is size of array
#define button  10          //pin where your active-low button is connected


uint8_t data[7] = {7, 'G', 'O', 'J', 'A', 'Y', 'S'};  // array of data to transmit

void setup()
{
  pinMode(GRN_LED , OUTPUT);
  digitalWrite(GRN_LED, LOW);
  pinMode(YLO_LED , OUTPUT);
  digitalWrite(YLO_LED, LOW);


  for (byte i = 0; i < 5; i++)
  {
    digitalWrite(5, HIGH);
    digitalWrite(4, HIGH);
    delay(25);
    digitalWrite(5, LOW);
    digitalWrite(4, LOW);

  }


  //man.workAround1MhzTinyCore(); //add this in order for transmitter to work with 1Mhz Attiny85/84
  man.setupTransmit(TX_PIN, MAN_300);


}



void loop()
{
  delay(800);
  while (digitalRead(10) == 1)		// do nothing while no button pressed
  {

    digitalWrite(GRN_LED, 1);      // GREEN LED on = system is ready to transmit
  }

  digitalWrite(GRN_LED, 0);    // GREEN LED off

  man.transmitArray(datalength, data);
  for (int i = 0; i < 5; i++)
  { digitalWrite(YLO_LED, 1);
    delay(150);
    digitalWrite(YLO_LED, 0);
  }
  delay(800);
}
