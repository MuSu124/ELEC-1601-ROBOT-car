#include <Servo.h>
/*
#include<iostream>
#include<vector>
*/
using namespace std;



Servo myservoL;
Servo myservoR;
int valL = 0;
int valR = 0;
int valM = 0;
int distM = 0;
int distL = 0;
int distR = 0;
int analogPinL = A1;
int analogPinR = A0;
int LeftForward = 1600;
int LeftReverse = 1400;
int RightForward = 1400;
int RightReverse = 1600;
int TurnRight = 1450;
int TurnLeft = 1550;
int Stop = 1500;
int StopL = 1495;
int StopR = 1500;
int walls = 0;//how many walls detected
//bool direction = true; // true says go front, false says go back
//vector<int> wall2_selections = {}; //vector is a data structure similar to python list, it stores all 2 wall selections
/*
push_back: add an element to to back
back: invoke the last element
pop_back: delete last element

*/


const int midirLedPin=6, midirReceiverPin=7;
const int midredLedPin = A1;
const int leftirLedPin=10, leftirReceiverPin=11;
const int leftredLedPin = A2;
const int rightirLedPin=2, rightirReceiverPin=3;
const int rightredLedPin = A0;

const long leftfrequency = 38000;// 8.2cm
const long middlefrequency = 40000;// 8.2cm
const long rightfrequency = 39000;// 8.2cm

//why these three frequencies differ?

void setup(){
  Serial.begin(9600);

  //connect the wheel
  myservoL.attach(13);      
  myservoR.attach(12);

  //connect the ir light
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
  adjustor();
  Serial.print("Robot Starting"); 
}

void loop()
{
  delay(100);
  walls = 0;
  //1, check the walls
  valL = irDetect(leftirLedPin, leftirReceiverPin, leftfrequency);
  valM = irDetect(midirLedPin, midirReceiverPin, middlefrequency);
  distM = irDistance(midirLedPin, midirReceiverPin);
  valR = irDetect(rightirLedPin, rightirReceiverPin, rightfrequency);
  //int attempt=0; //how many times we have attempted at a selection point
  
  if ((valL+valM +valR==0)
  {walls = 3;}
  else if (valL+valM+valR==1)
  {walls = 2;}
   else if (((valL==0&&valM==1)||(valL==1&&valR==1))||(valM==1&&valR==1))
  {walls = 1;}
   else
  {walls = 0;}

    if (walls==3)
    {
        turnaround();
        // since car is driving back ,direction = false;

        //the car is now driving back to last selection point  (the different to drive in reverse function, the car turn around and drive forward)
    }
    else if (walls==2)
    {
        if(valM==1)
        {goForward();}
        if(valR==1)
        {turnRight();}
        if(valL==1)
        {turnLeft();}
        
    }
      /*
    else if (walls==1)
    {
        //meet the selection point



        About selection:
        Since we do not need to remeber the order of the selections( we always goes back the the last selection we make)
        The selection need to include 3 messages:
        Options, what did we choose, how many times we have encounter the selection?
        So that's how to read a selection:
        ex:<1,2>
        1,Options: 1 is right, 2 is mid, 3 is left
        2,what did we choose: it's the first element
        3, how many times: if the length is 2, means we already encounter this one time; if the length is 1, means we already encounter this two times.

        if (direction)
        {

            vector<int> selection = {};
            if (valM==0)
            {
                turnRight();
                forward();
              delay(1000);
                selection = {1,3};
            }
            if (valL==0)
            {
                turnRight();
              forward();
              delay(1000);
                selection = {1,2};
            }
            if (valR==0)
            {
                forward();
              delay(1000);
                selection = {2,3};
            }
            wall2_selections.push_back(selection);
        }
  

        else
        {//drive back to selection point


            direction = true;
            vector<int> selection = wall2_selections.back();//find the selection we want to do again, also the last selection
            int choice = selection[0];//find what did we do

            //1,reset the car base on what did we do   复位
            if (choice==1)
            {turnRight();}
            else if(choice==2)
            {turnaround()}
            else if(choice==3)
            {turnLeft();}
            //2, We want to kick the wrong option out of the selection variable
            selection.erase(selection.begin());


            if (selection.size()==1)
            {
                

                //3,take another choice which is not selected at first time
                choice = selection[0];
                if (choice==2)
                {
                    forward();//now the car physically locates at the selection point, although the direction is different
                    delay(1000);//enough time to leave the position
                }
                else if(choice==3)
                {
                    turnLeft();
                    forward();
                    delay(1000);
                }
                /*
                if (attempt==2)
                {
                    turnaround();
                    goForward();
                    delay(1000);// A delay is necessary here since at this point, car locats at a two point
                    direction = false;//we need to drive back to last selection again
                    attempt = 0;
                    wall2_selections.pop_back();
                }

            }
            else if (selection.size()==0)  this function is not used
            {
                //means we have already encounter this two times
                turnaround();
                goForward();
                delay(1000);// A delay is necessary here since at this point, car locats at a two point
                wall2_selections.pop_back();
            }
        }
    }
*/
    else(walls == 0)
    {goForward();}


/*

  if (valL == 1){
      turnRight();
      stop();
      goForward();
  }
  else if (valR == 1){
     turnLeft();
      stop();
     goForward();
  }
  else if (valL == 0 && valR == 0 && valM ==0){
    stop();
  }

  else{
    distM = irDistance(midirLedPin, midirReceiverPin);
    if ( distM > 3){
    goForward();
    }
  }
  */

}




//get wall deteceted
int irDetect(int irLedPin, int irReceiverPin, long frequency){
  tone(irLedPin, frequency);                 // Turn on the IR LED square wave
  delay(1);                                  // Wait 1 ms
  noTone(irLedPin);                          // Turn off the IR LED
  int ir = digitalRead(irReceiverPin);      
  //Serial.println(ir);  // IR receiver -> ir variable
  delay(50);                                  // Down time before recheck
  return ir;                                 // Return 0 detect, 1 no detect
}



//get distance against wall
int irDistance(int irLedPin, int irReceiverPin){
   int distance = 0;
   for(long f = 38000; f <= 42000; f += 1000)
   {
      distance += irDetect(irLedPin, irReceiverPin, f);
   }
   return distance;
}




//car behavior
//when car do the behavior, program should stop before it finished

//basic turning
void turnLeft(){
  Serial.println("Begining left turn");
  myservoL.writeMicroseconds(TurnLeft);
  myservoR.writeMicroseconds(TurnLeft);
  delay(1050);
  Serial.println("Left Turn Completed");
  
}
void turnRight(){
  Serial.println("Begining right turn");
  myservoL.writeMicroseconds(TurnRight);
  myservoR.writeMicroseconds(TurnRight);
  delay(1250);
  Serial.println("Right Turn Completed");
}
void goForward(){
  Serial.println("Going Forward");
  myservoL.writeMicroseconds(LeftForward);
  myservoR.writeMicroseconds(RightForward);
  Serial.println("Forward Completed"); 
}


//extension
void turnaround(){
    stop();
    turnRight();
    turnRight();
    direction = !direction;
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
  delay(50);
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
    myservoR.writeMicroseconds(1510);// Bill: Do we need a delay here?
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

