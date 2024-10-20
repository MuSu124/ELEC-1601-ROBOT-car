#include <Servo.h>
Servo myservoL;
Servo myservoR;
// Variables that will need to be configured
int Leftturntime = 840; // time needed for robot to rotate 90 degrees left
int Rightturntime = 1007; // time needed for robot to rotate 90 degrees right
int forwardTime = 500; // time needed for robot to move 1 maze unit, maze wall is 20cm, maze corner is 7cm
int StopL = 1495; // 0 Wheel speed for left wheel
int StopR = 1500; // 0 Wheel speed for right wheel
const long leftfrequency = 38000;// These three frequencies should mean robot
const long middlefrequency = 40000;// detects walls from the same distance
const long rightfrequency = 39000;// currently set to 8.2cm
// Constant Variables
const int LeftForward = 1600;
const int RightForward = 1400;
const int LeftReverse = 1400;
const int RightReverse = 1600;
const int TurnRight = 1350;
const int TurnLeft = 1650;
const int Stop = 1500;
const int midirLedPin=6, midirReceiverPin=7;
const int midredLedPin = A1;
const int leftirLedPin=10, leftirReceiverPin=11;
const int leftredLedPin = A2;
const int rightirLedPin=2, rightirReceiverPin=3;
const int rightredLedPin = A0;
// Data Recording Variables
int valL = 0;
int valR = 0;
int valM = 0;
int distM = 0;
int distL = 0;
int distR = 0;
// Advanced part variables for mazeEnd function
int commands[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int comindex = 0; // 0 = do nothing, 1 = Right turn, 2 = Left turn, 3 = go forward
int curcom = 0; // for iteration through list

void setup(){
  Serial.begin(9600);
  myservoL.attach(13);
  myservoR.attach(12);
  pinMode(midirReceiverPin, INPUT);            // IR receiver pin is an input
  pinMode(midirLedPin, OUTPUT);                // IR LED pin is an ouput
  pinMode(midredLedPin, OUTPUT);               // Red LED pin is an output
  pinMode(leftirReceiverPin, INPUT);            // IR receiver pin is an input
  pinMode(leftirLedPin, OUTPUT);                // IR LED pin is an ouput
  pinMode(leftredLedPin, OUTPUT);               // Red LED pin is an output
  pinMode(rightirReceiverPin, INPUT);            // IR receiver pin is an input
  pinMode(rightirLedPin, OUTPUT);                // IR LED pin is an ouput
  pinMode(rightredLedPin, OUTPUT);               // Red LED pin is an output
  Serial.begin(9600);  
  stop();
  delay(1000);
  //Serial.print("Robot Starting");
}

void loop()
{ 
  valL = irDetect(leftirLedPin, leftirReceiverPin, leftfrequency);
  valM = irDetect(midirLedPin, midirReceiverPin, middlefrequency);
  distM = irDistance(midirLedPin, midirReceiverPin);
  valR = irDetect(rightirLedPin, rightirReceiverPin, rightfrequency);
  distR = irDistance(rightirLedPin, rightirReceiverPin);
  distL = irDistance(leftirLedPin, leftirReceiverPin);
  distM = irDistance(midirLedPin, midirReceiverPin);
  Serial.print(distM);
  if (distM < 7){
    if(distL >= 5 || distR >= 5){
      if (distL > distR){
        turnRight();
        myservoL.writeMicroseconds(LeftForward);
        myservoR.writeMicroseconds(RightForward);
      }
      else if (distL < distR){
        turnLeft();
        myservoL.writeMicroseconds(LeftForward);
        myservoR.writeMicroseconds(RightForward);
      }
    }
  }
  else{
    if (valL == 0 && valR == 0){
      if (distM <3){
        stop();
      }
      else{
        goForward();
      }
    }
    else{
      myservoL.writeMicroseconds(LeftForward);
      myservoR.writeMicroseconds(RightForward);
    }
  }
}


int irDetect(int irLedPin, int irReceiverPin, long frequency){
  tone(irLedPin, frequency);                 // Turn on the IR LED square wave
  delay(1);                                  // Wait 1 ms
  noTone(irLedPin);                          // Turn off the IR LED
  int ir = digitalRead(irReceiverPin);      
  //Serial.println(ir);  // IR receiver -> ir variable
  delay(5);                                  // Down time before recheck
  return ir;                                 // Return 0 detect, 1 no detect
}
int irDistance(int irLedPin, int irReceiverPin){
   int distance = 0;
   for(long f = 36000; f <= 42000; f += 1000)
   {
      distance += irDetect(irLedPin, irReceiverPin, f);
   }
   return distance;
}
void turnLeft(){
  //Serial.println("Begining left turn");
  myservoL.writeMicroseconds(TurnLeft);
  myservoR.writeMicroseconds(Stop);
  delay(Leftturntime);
  stop();
  //goForward();
  //Serial.println("Left Turn Completed");
 
}
void turnRight(){
  //Serial.println("Begining right turn");
  myservoL.writeMicroseconds(Stop);
  myservoR.writeMicroseconds(TurnRight);
  delay(Rightturntime);
  stop();
  //goForward();
 // Serial.println("Right Turn Completed");
}
void goForward(){
  distR = irDistance(rightirLedPin, rightirReceiverPin);
  distL = irDistance(leftirLedPin, leftirReceiverPin);
  distM = irDistance(midirLedPin, midirReceiverPin);
  if (distL > distR){
    myservoR.writeMicroseconds(1410);
    myservoL.writeMicroseconds(1530);
  }
  else if (distL < distR){
    myservoL.writeMicroseconds(1590);
    myservoR.writeMicroseconds(1460);
  }
  else if (distL == distR){
    myservoL.writeMicroseconds(LeftForward);
    myservoR.writeMicroseconds(RightForward);
  }
}
void stop(){
  //Serial.println("Stopping");
  myservoL.writeMicroseconds(StopL);
  myservoR.writeMicroseconds(StopR);
  //delay(5);
  //Serial.println("Resuming");
}
