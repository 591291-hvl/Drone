//Sender sends information to reciver
//Has joysticks

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


//Radio
RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

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


void setup() {
  Serial.begin(9600);

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

  //Motor
  pinMode(5, OUTPUT);
}

void loop() {

  //Motor
  analogWrite(5, abs(mapXR));
  
  //Radio
  const char text[] = "Fuck you man";
  Serial.println(text);
  radio.write(&text, sizeof(text));
  delay(1000);

  //JoystickR
  xPositionR = analogRead(VRxR);
  yPositionR = analogRead(VRyR);
  SW_stateR = digitalRead(SWR);
  mapXR = map(xPositionR, 0, 1023, -512, 512);
  mapYR = map(yPositionR, 0, 1023, -512, 512);
  
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
  mapXL = map(xPositionL, 0, 1023, -512, 512);
  mapYL = map(yPositionL, 0, 1023, -512, 512);
  
  Serial.print("XL: ");
  Serial.print(mapXL);
  Serial.print(" | YL: ");
  Serial.print(mapYL);
  Serial.print(" | ButtonL: ");
  Serial.println(SW_stateL);



  
}
