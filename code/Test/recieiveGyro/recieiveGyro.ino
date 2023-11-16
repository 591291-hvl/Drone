//Radio include start

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//Radio include end


//Motor include start

#include <Servo.h>

//Motor include end


//Radio variables start

#define CE_PIN   7
#define CSN_PIN 8

const byte thisSlaveAddress[5] = {'R','x','A','A','A'};

RF24 radio(CE_PIN, CSN_PIN);

float dataReceived[3] = {}; // this must match dataToSend in the TX
bool newData = false;

//Radio variables end


//Motor variables start

#define MIN_PULSE_LENGTH 1000 // Minimum pulse length in µs
#define MAX_PULSE_LENGTH 2000 // Maximum pulse length in µs
Servo motA, motB, motC, motD;
int number[4] = {};

int powerMain = 0;

int powerpx = 0;
int powernx = 0;
int powerpy = 0;
int powerny = 0;

//Motor variables end




void setup() {
  Serial.begin(9600);
  
  Radiosetup();
  Motorsetup();

  
}


void Motorsetup() {
  motA.attach(3, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
  motB.attach(5, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
  motC.attach(6, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
  motD.attach(10, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);

  motA.writeMicroseconds(MIN_PULSE_LENGTH);
  motB.writeMicroseconds(MIN_PULSE_LENGTH);
  motC.writeMicroseconds(MIN_PULSE_LENGTH);
  motD.writeMicroseconds(MIN_PULSE_LENGTH);
}

void Radiosetup() {

    

    Serial.println("SimpleRx Starting");
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(1, thisSlaveAddress);
    radio.startListening();
}

//=============

void loop() {
  Radioloop();
  Motorloop();
  
}

void Motorloop() {
  powerMain = map(dataReceived[2], 0, 512, 0, 500);
  
  if(dataReceived[0] < 0){ //x angle negative
    powerpx = 2*map(-dataReceived[0], 0, 200, 0, 500);// give power to front
  } else {
    powernx = 2*map(dataReceived[0], 0, 200, 0, 500);// give power to back
  }

  if(dataReceived[1] < 0) { 
    powerny = 2*map(-dataReceived[1], 0, 200, 0, 500);
  } else {
    powerpy = 2*map(dataReceived[1], 0, 200, 0, 500);
  }

  
  //A-50, B-140, C-137, D-140
  //Serial.println(String(powerpx) + " " + String(powernx));
  motA.writeMicroseconds(MIN_PULSE_LENGTH+50 -20 + powerMain + powerpx + powerpy);
  motB.writeMicroseconds(MIN_PULSE_LENGTH+140 -20 + powerMain + powerpx + powerny);
  motC.writeMicroseconds(MIN_PULSE_LENGTH+137 -20 + powerMain + powernx + powerny);
  motD.writeMicroseconds(MIN_PULSE_LENGTH+140 -20 + powerMain + powernx + powerpy);
  

}


void Radioloop() {
    getData();
    showData();
}

//==============

void getData() {
    if ( radio.available() ) {
        radio.read( &dataReceived, sizeof(dataReceived) );
        newData = true;
    }
}

void showData() {
    if (newData == true) {
        Serial.print("Data received: ");
        Serial.print("x angle: " + String(dataReceived[0]) + " ");
        Serial.print("y angle: " + String(dataReceived[1]) + " ");
        Serial.println("XL: " + String(dataReceived[2]) + " ");
        newData = false;
    }
}
