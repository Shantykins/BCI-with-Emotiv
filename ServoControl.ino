/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo3, myservo5, myservo6, myservo9, myservo10 ;  // create servo object to control a servo

//int potpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

void setup()
{
 // while(!Serial) {}

//Serial.println("Is there anybody out there?");
  myservo3.attach(3);  // attaches the servo on pin 3 to the servo object
  myservo5.attach(5);  // attaches the servo on pin 5 to the servo object
  myservo6.attach(6);  // attaches the servo on pin 6 to the servo object
  myservo9.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo10.attach(10);// attaches the servo on pin 10 to the servo object
  pinMode(13, HIGH);

  val = 0;
  val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  myservo3.write(val);                  // sets the servo position according to the scaled value
  myservo5.write(val);
  myservo6.write( val);
  myservo9.write(val);
  myservo10.write(val);
  digitalWrite(13, HIGH);
  delay(1500); 
}

void loop() 
{

  val = 0;
   val = map(val, 0, 1023, 0, 180); 
  
  val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  myservo3.write(val);                  // sets the servo position according to the scaled value
  myservo5.write(val);
  myservo6.write( val);
  myservo9.write(val);
  myservo10.write(val);
  digitalWrite(13, HIGH);
  delay(1500);   

  val = 350;
  val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  myservo3.write(val);
  myservo5.write(val);
  myservo6.write(val);
  myservo9.write(val);
  myservo10.write(180); 
  digitalWrite(13, LOW);
  delay(1500);
 }
