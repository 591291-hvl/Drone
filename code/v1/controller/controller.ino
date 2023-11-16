//Controller

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


//Radio
RF24 radio(7, 8); // CE, CSN

const byte address[6] = "10001";
int number[4] = {};

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


void setup() {
  Serial.begin(9600);
  Serial.println("Start");

  //Radio
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

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
  mapXRCalibrate = map(xPositionRCalibrate, 0, 1023, -512, 512);
  mapYRCalibrate = map(yPositionRCalibrate, 0, 1023, -512, 512);
  mapXLCalibrate = map(xPositionLCalibrate, 0, 1023, 512, -512);
  mapYLCalibrate = map(yPositionLCalibrate, 0, 1023, -512, 512);
  Serial.println(mapXRCalibrate);
  Serial.println(mapYRCalibrate);
  Serial.println(mapXLCalibrate);
  Serial.println(mapYLCalibrate);
  Serial.println("============");
  

}

void loop() {

  //JoystickR
  xPositionR = analogRead(VRxR);
  yPositionR = analogRead(VRyR);
  SW_stateR = digitalRead(SWR);
  mapXR = map(xPositionR, 0, 1023, -512, 512) - mapXRCalibrate;
  mapYR = map(yPositionR, 0, 1023, -512, 512) - mapYRCalibrate;
  
  Serial.print("XR: ");
  Serial.print(mapXR);
  Serial.print(" | YR: ");
  Serial.print(mapYR);
  Serial.print(" | ButtonR: ");
  Serial.println(SW_stateR);

  //JoystickL
  xPositionL = analogRead(VRxL);
  yPositionL = analogRead(VRyL);
  SW_stateL = digitalRead(SWL);
  mapXL = map(xPositionL, 0, 1023, 512, -512) - mapXLCalibrate;
  mapYL = map(yPositionL, 0, 1023, -512, 512) - mapYLCalibrate;
  
  Serial.print("XL: ");
  Serial.print(mapXL);
  Serial.print(" | YL: ");
  Serial.print(mapYL);
  Serial.print(" | ButtonL: ");
  Serial.println(SW_stateL);
  
  //Radio
  //const char text[] =  {String(mapXL).c_str()};
  number[0] = mapXR;
  number[1] = mapYR;
  number[2] = mapXL;
  number[3] = mapYL;
  
  String str = (String(mapXL));
  Serial.println(String(number[2]));
  radio.write(&number, sizeof(number));
  //Serial.println(radio.write(&mapXL, sizeof(mapXL)));
  delay(10);
}
