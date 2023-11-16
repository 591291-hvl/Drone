//Drone is reciver, takes information from controller
#include <MPU6050.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <Servo.h>
// Customize here pulse lengths as needed
#define MIN_PULSE_LENGTH 1000 // Minimum pulse length in µs
#define MAX_PULSE_LENGTH 2000 // Maximum pulse length in µs
Servo motA, motB, motC, motD;
int number[4] = {};
int power = 0;


RF24 radio(7, 8); // CE, CSN

const byte address[6] = "10001";

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  
  motA.attach(3, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
  motB.attach(5, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
  motC.attach(6, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
  motD.attach(10, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);

  //motA.writeMicroseconds(MIN_PULSE_LENGTH);
  //motB.writeMicroseconds(MIN_PULSE_LENGTH);
  //motC.writeMicroseconds(MIN_PULSE_LENGTH);
  //motD.writeMicroseconds(MIN_PULSE_LENGTH);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  //MPU
  //Serial.println("");

  
  if (radio.available()) {
    //Serial.println("here");
    radio.read(&number, sizeof(number));
    Serial.println(number[2]);
    //radio.read(&data, sizeof(data));
    //Serial.println(data);
    
    if( number[2] >= 20){
      //50, 137, 140, 140
      power = map(number[2], 0, 512, 0, 500);
      //Serial.println("go");
      //Serial.println(power);
      motA.writeMicroseconds(MIN_PULSE_LENGTH+power+50);
      motB.writeMicroseconds(MIN_PULSE_LENGTH+power+140);
      motC.writeMicroseconds(MIN_PULSE_LENGTH+power+137);
      motD.writeMicroseconds(MIN_PULSE_LENGTH+power+140);
    }else{
      //Serial.println("not go");
      motA.writeMicroseconds(MIN_PULSE_LENGTH);
      motB.writeMicroseconds(MIN_PULSE_LENGTH);
      motC.writeMicroseconds(MIN_PULSE_LENGTH);
      motD.writeMicroseconds(MIN_PULSE_LENGTH);
    }
  }

  
  
}
