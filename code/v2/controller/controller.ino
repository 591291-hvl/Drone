//Radio include start
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//Radio include end

//Joysticks variables start

//joystickR
int VRxR = A0;
int VRyR = A1;
int SWR = 4;

int xPositionR = 0;
int yPositionR = 0;
int SW_stateR = 0;
int mapXR = 0;
int mapYR = 0;

//joystickL
int VRxL = A2;
int VRyL = A3;
int SWL = 2;

int xPositionL = 0;
int yPositionL = 0;
int SW_stateL = 0;
int mapXL = 0;
int mapYL = 0;

//Calibrate
int xPositionRCalibrate = 0;
int yPositionRCalibrate = 0;
int xPositionLCalibrate = 0;
int yPositionLCalibrate = 0;

int mapXRCalibrate = 0;
int mapYRCalibrate = 0;
int mapXLCalibrate = 0;
int mapYLCalibrate = 0;


//Joysticks variables end


//Radio variables start

#define CE_PIN   7
#define CSN_PIN 8

const byte slaveAddress[5] = {'R','x','A','A','A'};


RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

float dataToSend[6] = {};


unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 100;


//Radio variables end


void setup() {
  Serial.begin(9600);                     //Remember to set this same baud rate to the serial monitor  

  JoystickSetup();
  RadioSetup();
}

void JoystickSetup() {
  //JoystickR
  pinMode(VRxR, INPUT);
  pinMode(VRyR, INPUT);
  pinMode(SWR, INPUT_PULLUP);

  //JoystickL
  pinMode(VRxL, INPUT);
  pinMode(VRyL, INPUT);
  pinMode(SWL, INPUT_PULLUP);

  //Calibrate
  xPositionRCalibrate = analogRead(VRxR);
  yPositionRCalibrate = analogRead(VRyR);
  xPositionLCalibrate = analogRead(VRxL);
  yPositionLCalibrate = analogRead(VRyL);
  mapXRCalibrate = map(xPositionRCalibrate, 0, 1023, 512, -512);
  mapYRCalibrate = map(yPositionRCalibrate, 0, 1023, -512, 512);
  mapXLCalibrate = map(xPositionLCalibrate, 0, 1023, 512, -512);
  mapYLCalibrate = map(yPositionLCalibrate, 0, 1023, -512, 512);
  Serial.println(mapXRCalibrate);
  Serial.println(mapYRCalibrate);
  Serial.println(mapXLCalibrate);
  Serial.println(mapYLCalibrate);
  Serial.println("============");
}

void RadioSetup() {

    Serial.println("SimpleTx Starting");

    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.setRetries(5,15); // delay, count
    radio.openWritingPipe(slaveAddress);
}


void loop() {
  Joystickloop();
  Radioloop();
}

void Joystickloop() {
  //JoystickR
  xPositionR = analogRead(VRxR);
  yPositionR = analogRead(VRyR);
  SW_stateR = digitalRead(SWR);
  mapXR = map(xPositionR, 0, 1023, 512, -512) - mapXRCalibrate;
  mapYR = map(yPositionR, 0, 1023, -512, 512) - mapYRCalibrate;

  //JoystickL
  xPositionL = analogRead(VRxL);
  yPositionL = analogRead(VRyL);
  SW_stateL = digitalRead(SWL);
  mapXL = map(xPositionL, 0, 1023, 512, -512) - mapXLCalibrate;
  mapYL = map(yPositionL, 0, 1023, -512, 512) - mapYLCalibrate;

}

void Radioloop() {
    currentMillis = millis();
    if (currentMillis - prevMillis >= txIntervalMillis) {
        send();
        prevMillis = millis();
    }
}

//====================

void send() {

    bool rslt;
    rslt = radio.write( &dataToSend, sizeof(dataToSend) );
        // Always use sizeof() as it gives the size as the number of bytes.
        // For example if dataToSend was an int sizeof() would correctly return 2

    Serial.print("Data Sent: ");
    Serial.print("XL: " + String(dataToSend[0]) + " ");
    Serial.print("XR: " + String(dataToSend[1]) + " ");
    Serial.print("YL: " + String(dataToSend[2]) + " ");
    Serial.print("YR: " + String(dataToSend[3]) + " ");
    Serial.print("SWR: " + String(dataToSend[4]) + " ");
    Serial.print("SWL: " + String(dataToSend[5]) + " ");
    if (rslt) {
        Serial.println("  Acknowledge received");
        updateMessage();
    }
    else {
        Serial.println("  Tx failed");
    }
}

//================

void updateMessage() {
        // so you can see that new data is being sent
    dataToSend[0] = mapXL;
    dataToSend[1] = mapXR;
    dataToSend[2] = mapYL;
    dataToSend[3] = mapYR;
    dataToSend[4] = SW_stateR;
    dataToSend[5] = SW_stateL;
}
