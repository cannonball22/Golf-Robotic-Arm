#include <Servo.h> //servo motors library
#include <NewPing.h> //ultrasonic sensor library
#include <math.h> // math library
int dt = 1500; //delay by 1.5 sec
// declaring servo pins on SSC-32U
// Pin 0:base, pin 1:shoulder, pin 2:elbow
// pin 3:wrist, pin 4:gripper, pin 5: wrist rotate
const int trigpin = 10;
const int echopin = 11;
float duration;
float distance;
int gripper;
int base_pin = 0;
int shoulder_pin = 1;
int elbow_pin = 2;
int wrist_pin = 3;
int gripper_pin = 4;
int wrist_rotate_pin = 5;
int minFreq = 500; //minimum frequency
int maxFreq = 2500; //maximum frequency
int minVal[6] = {-90,-90,-150,-90,0,-90}; // minimum angles for each servo
int maxVal[6] = {90,70,90,90,1,90}; // maximum angles for each servo

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600); //baud rate
  pinMode(13,OUTPUT); // IR sensor output
  pinMode(3,INPUT); // IR sensor input
  pinMode(trigpin,OUTPUT);
  pinMode(echopin,INPUT);
}
int mapping(int value,int pin){
  int position_in_sequence = map(value,minVal[pin],maxVal[pin],minFreq,maxFreq); //map(value, fromLow, fromHigh, toLow, toHigh)
  return position_in_sequence;
}
void sendMoves(int pin, int position, int speed){
  //A typical line of code for the SSC-32 servo controller would be: #15 P1500 T500
  //# represents the pin number on the SSC-32 that the servo is connected to
  //P represents the position that the servo motor should rotate to
  //T represents the time that the servo should take to rotate to the desired position
  // Serial.println("# Servo_pin Puls Time for excute motion");
  // Example: Serial.println("#0 P500 T1500")
  int new_position = mapping(position,pin);
  Serial.print("#");
  Serial.print(pin);
  Serial.print(" P");
  Serial.print(new_position);
  Serial.print(" T");
  Serial.println(speed);
}
void moveServo(int base, int shoulder, int elbow, int wrist, int gripper, int wrist_rotate, int speed){
  sendMoves (base_pin, base, speed);
  sendMoves (shoulder_pin, shoulder, speed);
  sendMoves (elbow_pin, elbow, speed);
  sendMoves (wrist_pin, wrist, speed);
  sendMoves (gripper_pin, gripper, speed);
  sendMoves (wrist_rotate_pin, wrist_rotate, speed);
}
void loop() {
  // put your main code here, to run repeatedly:
  moveServo(81, -26, 10.75, 18.2, 0, 0, 1500);      //Grabbing club // double check the value of gripper

  moveServo(81, -26, 20, 18.2, 0, 0, 1500);         //Lifting club
  
  moveServo(81, -21, 8.38, 30, 0, 0, 1500);         //Ready to swing

  moveServo(-81, -21, 8.38, 30, 0, 0, 750);        //Swing club
  
  moveServo(81, -26, 10.75, 18.2, 0.7, 0, 1500);    //Replacing club
  
  moveServo(-81, -28.5, 16.6, -80.8, 0.7, 0, 1500); //Above ball in hole
  
  moveServo(-81, -30, 9.5, -54.04, 0, 0, 1500);     //Gripping ball on level ground
  
  moveServo(0, -24.4, 8, -43.38, 0, 0, 1500);       //Holding ball 30mm above tee
  
}
