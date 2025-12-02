#include <ESP32Servo.h>

Servo myservo;

int angle = 0;          // position actuelle
bool monte = true;      // direction du mouvement
unsigned long lastMove = 0;
const int stepDelay = 150;   // temps entre chaque pas
const int stepAngle = 10;    // pas en degrés

void setup() {
  Serial.begin(9600);

  // Initialisation du servo
  myservo.setPeriodHertz(50);       // fréquence standard
  myservo.attach(12, 800, 2200);    // pin GPIO 12 + limites
}

void loop() {

  if (millis() - lastMove >= stepDelay) {

    if (monte) {
      angle += stepAngle;
      if (angle >= 180) {
        angle = 180;
        monte = false;
      }
    } else {
      angle -= stepAngle;
      if (angle <= 0) {
        angle = 0;
        monte = true;
      }
    }

    myservo.write(angle);
    Serial.println(angle);

    lastMove = millis();
  }
}
