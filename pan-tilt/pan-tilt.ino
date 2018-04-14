/*
 * Read a joystick and send movement commands to a pair of Dynamixel servos
 * uses 74HC241 to mix the Rx and Tx serial connection to the AX-12A servos
 * 
 *  ** When running, DISCONNECT the Arduino from the PC, or it will randomly reset.
 */

#include "Arduino.h"
#include "AX12A.h"

// Pin assignments
int yPin = A0;
int xPin = A1;
int jButton = 2;  // joystick button
int mem1 = 3;  // memory recall 1
int mem2 = 4;  // memory recall 2


int yMax = 750;
int yMin = 250;
int xMax = 750;
int xMin = 250;

int xAxis;  // reading
int yAxis;

int xMove; // how many steps to jump
int yMove;

int xCenter = 0;
int yCenter = 0;
int xSlow = 20;  // diff from center for slow
int xFast = 370;  // diff from center for fast
int ySlow = 20;
int yFast = 350;

#define DirectionPin 	(10u)
#define BaudRate  		(1000000ul)
#define yID				(1u)
#define xID       (1u)

int initial_pos = 512;

int pos = initial_pos;

void setup()
{
	ax12a.begin(BaudRate, DirectionPin, &Serial);
  ax12a.moveSpeed(ID, pos, 50);
  delay(200);

  xCenter = analogRead(xPin);
  yCenter = analogRead(yPin);
  
//    Serial.begin(9600);
//  Serial.println("Hello world");
//  delay(2000);// Give reader a chance to see the output.
}

// determine the X & Y change to make for the servo positions
void calculateMove(){
  int xDiff = xAxis - xCenter;
  if (abs(xDiff) > xFast){
    xMove = 3;
  }
  else if (abs(xDiff) > xSlow){
    xMove = 1;
  }
  else {
    xMove = 0;
  }
  if (xDiff < 0){   // is it negative?
    xMove *= -1;   // if so, move the other direction
  }
  
  int yDiff = yAxis - yCenter;
  if (abs(yDiff) > yFast){
    yMove = 3;
  }
  else if (abs(yDiff) > ySlow){
    yMove = 1;
  }
  else {
    yMove = 0;
  }
  if (yDiff < 0){   // is it negative?
    yMove *= -1;   // if so, move the other direction
  }
}

void loop()
{

  xAxis = analogRead(xPin);
  yAxis = analogRead(yPin);
  calculateMove();

  if ((pos + yMove) < yMax and (pos + yMove) > yMin){
  	pos = pos + yMove;
	  ax12a.moveSpeed(yID, pos, 100);

  if ((pos + xMove) < xMax and (pos + xMove) > xMin){
    pos = pos + xMove;
 //   ax12a.moveSpeed(xID, pos, 100);
  }
  
//  Serial.print("x reading = ");
//  Serial.print(xAxis);
//  Serial.print("move x = ");
//  Serial.print(xMove);
//  Serial.print("   pos = ");
//  Serial.println(pos);
	delay(50);
}
