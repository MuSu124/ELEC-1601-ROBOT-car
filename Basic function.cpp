#include <Servo.h>
Servo myservoL;
Servo myservoR;
// Variables that will need to be configured
int Leftturntime = 1050; // time needed for robot to rotate 90 degrees left
int Rightturntime = 1250; // time needed for robot to rotate 90 degrees right
int forwardTime = 500; // time needed for robot to move 1 maze unit, maze wall is 20cm, maze corner is 7cm
int StopL = 1495; // 0 Wheel speed for left wheel
int StopR = 1500; // 0 Wheel speed for right wheel
const long leftfrequency = 38000;// These three frequencies should mean robot
const long middlefrequency = 40000;// detects walls from the same distance
const long rightfrequency = 39000;// currently set to 8.2cm
// Constant Variables
const int LeftForward = 1600;
const int LeftReverse = 1400;
const int RightForward = 1400;
const int RightReverse = 1600;
const int TurnRight = 1450;
const int TurnLeft = 1550;
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
  delay(5000);
  //adjustor();
  Serial.print("Robot Starting");
}

void loop()
{ Serial.println(valM);
  delay(100);
  valL = irDetect(leftirLedPin, leftirReceiverPin, leftfrequency);
  valM = irDetect(midirLedPin, midirReceiverPin, middlefrequency);
  distM = irDistance(midirLedPin, midirReceiverPin);
  valR = irDetect(rightirLedPin, rightirReceiverPin, rightfrequency);
  //Serial.print(distM);
  if (valM == 0){
    if (valL == 0){
      turnRight();
      
      //commands[comindex] = 1;
      //comindex+=1;
    }
    else if (valR == 0){
      turnLeft();
      //commands[comindex] = 2;
      //comindex+=1;

    }
    else if (valL == 0 && valR == 0){
      stop();
      //TurnAround()
    }
    /*else{
      distM = irDistance(midirLedPin, midirReceiverPin);
      if ( distM > 3){
        goForward();
        commands[comindex] = 3;
        comindex+=1;
      }
    }*/
  }
  else {
    goForward();
  }

  }
int irDetect(int irLedPin, int irReceiverPin, long frequency){
  tone(irLedPin, frequency);                 // Turn on the IR LED square wave
  delay(1);                                  // Wait 1 ms
  noTone(irLedPin);                          // Turn off the IR LED
  int ir = digitalRead(irReceiverPin);      
  //Serial.println(ir);  // IR receiver -> ir variable
  delay(50);                                  // Down time before recheck
  return ir;                                 // Return 0 detect, 1 no detect
}
int irDistance(int irLedPin, int irReceiverPin){
   int distance = 0;
   for(long f = 38000; f <= 42000; f += 1000)
   {
      distance += irDetect(irLedPin, irReceiverPin, f);
   }
   return distance;
}
void turnLeft(){
  Serial.println("Begining left turn");
  myservoL.writeMicroseconds(TurnLeft);
  myservoR.writeMicroseconds(TurnLeft);
  delay(Leftturntime);
  stop();
  goForward();
  Serial.println("Left Turn Completed");
 
}
void turnRight(){
  Serial.println("Begining right turn");
  myservoL.writeMicroseconds(TurnRight);
  myservoR.writeMicroseconds(TurnRight);
  delay(Rightturntime);
  stop();
  goForward();
  Serial.println("Right Turn Completed");
}
void goForward(){
  Serial.println("Going Forward");
  myservoL.writeMicroseconds(LeftForward);
  myservoR.writeMicroseconds(RightForward);
  delay(forwardTime);
  //stop();
  Serial.println("Forward Completed");
}
void reverse(){
  Serial.println("Reversing");
  myservoL.writeMicroseconds(LeftReverse);
  myservoR.writeMicroseconds(RightReverse);
  delay(3000);
  Serial.println("Reversing Completed");
}
void stop(){
  Serial.println("Stopping");
  myservoL.writeMicroseconds(StopL);
  myservoR.writeMicroseconds(StopR);
  delay(5);
  Serial.println("Resuming");
}
void adjustor(){
  distR = irDistance(rightirLedPin, rightirReceiverPin);
  Serial.println("Left:");
  Serial.println(distL);
  distL = irDistance(leftirLedPin, leftirReceiverPin);
  Serial.println("Right:");
  Serial.println(distR);

  while (distL < distR){
    myservoR.writeMicroseconds(1510);
    distL = irDistance(leftirLedPin, leftirReceiverPin);
    distR = irDistance(rightirLedPin, rightirReceiverPin);
    if (distL == distR){
      stop();
      break;
    }
  }
  while (distL > distR){
    myservoL.writeMicroseconds(1480);
    distL = irDistance(leftirLedPin, leftirReceiverPin);
    distR = irDistance(rightirLedPin, rightirReceiverPin);
    if (distL == distR){
      stop();
      break;
    }
  }
}
void testspeed(){
  int sl = 1500;
  for(int s = 1500; s <= 1700; s += 25)
   {
     Serial.println(s);
     Serial.println(sl);
      myservoR.writeMicroseconds(s);
      myservoL.writeMicroseconds(sl);
      delay(1000);
      stop();
      delay(5000);
      sl -= 25;

   };
}
void mazeEnd(){
  myservoL.writeMicroseconds(TurnRight);
  myservoR.writeMicroseconds(TurnRight);
  delay(Rightturntime*8);
  stop();
  for (int i = 0; i < 25; i = i + 1) {
  curcom = commands[i];
    if (curcom == 0){
      continue;
    }
    else if (curcom == 1){
      turnRight();
      goForward();
    }
    else if (curcom == 2){
      turnLeft();
      goForward();
    }
    else if (curcom == 3){
      goForward();
    }
   
}
}
