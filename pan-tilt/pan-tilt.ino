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


int yMax = 700;
int yMin = 300;
int xMax = 4096;
int xMin = 0;

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
#define yID				(2u)
#define xID       (3u)

int initial_xPos = 2048;
int initial_yPos = 512;

int xPos = initial_xPos;
int yPos = initial_yPos;


void setup()
{
	ax12a.begin(BaudRate, DirectionPin, &Serial);
  ax12a.moveSpeed(yID, yPos, 50);
  ax12a.moveSpeed(xID, xPos, 50);
  delay(200);
  ax12a.setPunch(yID,512);
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
    xMove = 6;
  }
  else if (abs(xDiff) > xSlow){
    xMove = 2;
  }
  else {
    xMove = 0;
  }
  if (xDiff < 0){   // is it negative?
    xMove *= -1;   // if so, move the other direction
  }
  
  int yDiff = yAxis - yCenter;
  if (abs(yDiff) > yFast){
    yMove = 2;
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

  if ((yPos + yMove) < yMax and (yPos + yMove) > yMin){
  	yPos = yPos + yMove;
	  ax12a.moveSpeed(yID, yPos, 500);
    delay(2);
  }
  if ((xPos + xMove) < xMax and (xPos + xMove) > xMin){
    xPos = xPos + xMove;
    ax12a.moveSpeed(xID, xPos, 500);
    delay(2);
  }
  
//  Serial.print("x reading = ");
//  Serial.print(xAxis);
//  Serial.print("move x = ");
//  Serial.print(xMove);
//  Serial.print("   pos = ");
//  Serial.println(pos);
	delay(50);
}
