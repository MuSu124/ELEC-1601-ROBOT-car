#include <Servo.h>
Servo myservoL;
Servo myservoR;
// Variables that will need to be configured
int Leftturntime = 1125; // time needed for robot to rotate 90 degrees left
int Rightturntime = 1105; // time needed for robot to rotate 90 degrees right
//int Leftturntime = 1000; // time needed for robot to rotate 90 degrees left
//int Rightturntime = 1017; // time needed for robot to rotate 90 degrees right
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
delay(2500);
adjustor();
Serial.print("Robot Starting");
}

void loop()
{ 
distR = irDistance(rightirLedPin, rightirReceiverPin);
distL = irDistance(leftirLedPin, leftirReceiverPin);
valL = irDetect(leftirLedPin, leftirReceiverPin, leftfrequency);
valM = irDetect(midirLedPin, midirReceiverPin, middlefrequency);
distM = irDistance(midirLedPin, midirReceiverPin);
valR = irDetect(rightirLedPin, rightirReceiverPin, rightfrequency);

if (distL < 7  && distR < 7 ){
  if (distM < 7){
    stop();
  }
  }

if (distM < 7){
  distM = irDistance(midirLedPin, midirReceiverPin);
  if (distL < 7 && distR < 7 ){
    stop();
  }
  else if (distR == 7){
    if (distM <= 3){
      reverse();
      delay(500);
      turnLeft();
    }
    else{
      turnLeft();
    }
  }
  else if (distL == 7){
      if (distM <= 3){
      reverse();
      delay(500);
      turnRight();
    }
    else{
      turnRight();
    }
  }
  else{
    stop();
  }

distM = irDistance(midirLedPin, midirReceiverPin);
}
if (distM == 7){
  goForward();
    if (distL < 7 && distR < 7){
      adjustor();
    }
    else{
      adjustor2();
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
myservoL.writeMicroseconds(TurnLeft);
myservoR.writeMicroseconds(Stop);
delay(Leftturntime);
stop();
}
void turnRight(){
myservoL.writeMicroseconds(Stop);
myservoR.writeMicroseconds(TurnRight);
delay(Rightturntime);
stop();
}
void goForward(){
//Serial.println("Going Forward");
myservoL.writeMicroseconds(LeftForward);
myservoR.writeMicroseconds(RightForward);
//delay(forwardTime);
//stop();
//Serial.println("Forward Completed");
}
void reverse(){
Serial.println("Reversing");
myservoL.writeMicroseconds(LeftReverse);
myservoR.writeMicroseconds(RightReverse);

Serial.println("Reversing Completed");
}
void stop(){
//Serial.println("Stopping");
myservoL.writeMicroseconds(StopL);
myservoR.writeMicroseconds(StopR);
//delay(5);
//Serial.println("Resuming");
}
void adjustor(){
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
void adjustor2(){
  distR = irDistance(rightirLedPin, rightirReceiverPin);
  distL = irDistance(leftirLedPin, leftirReceiverPin);
  distM = irDistance(midirLedPin, midirReceiverPin);
  if (distL == 7 && distR == 7){
    goForward();
  }
  else if (distL == 7){
    if (distR < 3){
    myservoR.writeMicroseconds(1410);
    myservoL.writeMicroseconds(1530);
    }
    else if (distR > 3){
    myservoL.writeMicroseconds(1590);
    myservoR.writeMicroseconds(1460);
    }
  }
  else if (distR == 7){
    if (distL < 3){
    myservoL.writeMicroseconds(1590);
    myservoR.writeMicroseconds(1460);
    }
    else if (distL > 3){
    myservoR.writeMicroseconds(1410);
    myservoL.writeMicroseconds(1530);
    }
  }
  else{
    if (distR < 2){
    myservoR.writeMicroseconds(1410);
    myservoL.writeMicroseconds(1530);
    }
    else if (distL < 2){
    myservoL.writeMicroseconds(1590);
    myservoR.writeMicroseconds(1460);
    }
  }
}
