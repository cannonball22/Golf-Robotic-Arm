#include <Servo.h> //servo motors library
#include <NewPing.h> //ultrasonic sensor library
#include <math.h> // math library
int dt = 750; //delay by 0.75 sec
// declaring servo pins on SSC-32U
// Pin 0:base, pin 1:shoulder, pin 2:elbow
// pin 3:wrist, pin 4:gripper, pin 5: wrist rotate
const int trigpin = 10;
const int echopin = 11;
int ir_output = 13;
int ir_input = 3;
int LED = 9;
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
//int range = 40; // ultrasonic range 
// need to check how to calculate the range from the ball to the ultrasonic sensor
//NewPing sonar (10, 11, range); // (trigger pin, echo pin, range) // declaring object

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600); //baud rate
  pinMode(ir_output,OUTPUT); // IR sensor output
  pinMode(ir_input,INPUT);   // IR sensor input
  pinMode(trigpin,OUTPUT);   // ultrasonic output
  pinMode(echopin,INPUT);    // ultrasonic input
  pinMode(LED,OUTPUT);       // LED pin 
}
int mapping(int value,int pin){
  int position_in_sequence = map(value,minVal[pin],maxVal[pin],minFreq,maxFreq);           //map(value, fromLow, fromHigh, toLow, toHigh)
  return position_in_sequence; 
}
void sendMoves(int pin, int position, int speed){
  //A typical line of code for the SSC-32 servo controller would be: #15 P1500 T500
  //# represents the pin number on the SSC-32 that the servo is connected to
  //P represents the position that the servo motor should rotate to
  //T represents the time that the servo should take to rotate to the desired position
  // Serial.println("# Servo_pin Puls Time for excute motion");
  int new_position = mapping(position,pin);
  Serial.print("#");
  Serial.print(pin);
  Serial.print(" P");
  Serial.print(new_position);
  Serial.print(" T");  
  Serial.print(speed);
}

void moveServo(int base, int shoulder, int elbow, int wrist, int gripper, int wrist_rotate, int speed){
  sendMoves (base_pin, base, speed);
  sendMoves (shoulder_pin, shoulder, speed);
  sendMoves (elbow_pin, elbow, speed);
  sendMoves (wrist_pin, wrist, speed);
  sendMoves (gripper_pin, gripper, speed);
  sendMoves (wrist_rotate_pin, wrist_rotate, speed);
}
void move_after_hitting(){
  moveServo(0, 0, -5, 35, 0.75, 0, 2500);      //Clearance --
  delay (dt);
  moveServo(-81, -20, -5, -40, 0.75, 0, 2500); //Above ball in hole
  delay (5000);
  moveServo(-81, -28.5, -5, -40, 0.75, 0, 2500); //At ball in hole
  delay (5000);
  moveServo(-81, -28.5, -5, -40, 1, 0, 750);     //Gripping ball on level ground
  delay (2500);
  moveServo(-81, -20, -5, -40, 1, 0, 750);       //Hole Clearance
  delay (2500);
  moveServo(0, -22, 8, -43.38, 1, 0, 2500);       //Holding ball 30mm above tee
  delay (5000);
  moveServo(0, -24.4, 8, -43.38, 0.75, 0, 2500);       //Dropping Ball 30mm above tee --
  delay (2000);
  moveServo(0, -10, 8, -43.38, 0.75, 0, 1750);       //Lifting above tee --
  delay (3500);
}
void ultrasonic(){
  while (1){
  // to measure the distance using ultrasonic sensor we need to do the calculation 
  // Distance = (Time * SpeedOfSound)/2
  //speed of sound 343 m/s
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);
  duration = pulseIn(echopin,HIGH);
  distance = duration*0.0343/2; // speed of sound in usec
    if (distance >= 29 && distance <=31){ // distance between sensor and ball
      //continue movement 
      move_after_hitting ();
      return;
    }else{
      digitalWrite(LED, HIGH);
      delay (dt);
      digitalWrite(LED, LOW);
      delay (dt);
    }
  }
}
void IR(){
  if (digitalRead (3) == HIGH) {
    gripper = 1;
  }else{
    gripper = 0;
  }
}
void loop() { 
  // ssc-32 = pulse from 500 0 till 2500 180
  // Pin 0:base,//pin 1:shoulder, //pin 2:elbow
  // pin 3:wrist, //pin 4:gripper //pin 5: wrist rotate
  moveServo(81, 0, -5, 30, 0.75, 0, 2500);      //Position club // double check the value of gripper --
  delay (2750);
  moveServo(81, -23, -5, 35, 0.75, 0, 2500);    //Lower Arm
  delay (2250);
  moveServo(81, -23, -5, 35, 1, 0, 3500);       //Grabbing club // double check the value of gripper
  delay (3000);
  moveServo(81, 0, -5, 35, 1, 0, 2500);         //Lifting club
  delay (dt);
  moveServo(81, 0, -5, 30, 1, 0, 2500);         //Ready to swing
  delay (dt);
  moveServo(-20, 0, 8.38, 30, 1, 0, 700);       //Swing club
  delay (3000);
  moveServo(81, -23, -5, 35, 1, 0, 2500);       //Replacing club
  delay (3000);
  moveServo(81, 10, -5, 30, 1, 0, 2500);       //Lifting for clearance
  delay (dt);
  ultrasonic ();
}
