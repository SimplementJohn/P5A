#include <ESP32Servo.h>

Servo myservo_1;


const int pin_servo_1 = 12;
// const int pin_servo_1 = 14;


//Recommended servo pins include 2,4,12-19,21-23,25-27,32-33

void initServo(){
  
  myservo_1.setPeriodHertz(50);    // standard 50 hz servo
  myservo_1.attach(pin_servo_1,800,2200);

}
