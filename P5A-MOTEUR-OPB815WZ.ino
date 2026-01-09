/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//VARIABLES
#define EN      4
#define DIR_A   16
#define STEP_A  17

#define DIR_B   15
#define STEP_B  2

// Définition des sens de rotation
#define HORAIRE       HIGH
#define ANTIHORAIRE   LOW

#define DELAY_5SEC  5000
#define DELAY_3SEC  3000
#define DELAY_1SEC  1000

const int SENSOR_PIN = 27;
const int LED_PIN = 2;
bool sensActuel = HORAIRE;
bool test = 0;
bool testsens = ANTIHORAIRE;

// Nombre de pas pour 360° (1 tour complet)
const int STEPS_PER_REVOLUTION = 400; //200 de base // 400 pour doubler le nombre de pas (rapport de 1/2)
const int STEP_DELAY_US = 2000;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FONCTIONS (déclarations avant setup/loop pour éviter les erreurs)

void attente(int duree_ms) {
  delay(duree_ms);
}

void initMotors() {
  pinMode(STEP_A, OUTPUT);
  pinMode(DIR_A, OUTPUT);
  pinMode(STEP_B, OUTPUT);
  pinMode(DIR_B, OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN, HIGH);
}

void sensRotation(int dirPin, bool sens) {
  digitalWrite(dirPin, sens);
  
  if (sens == HORAIRE) {
    Serial.print("Sens: HORAIRE (PIN ");
  } else {
    Serial.print("Sens: ANTIHORAIRE (PIN ");
  }
  Serial.print(dirPin);
  Serial.println(")");
}

void rotate360(int stepPin, int steps) {
  for(int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(STEP_DELAY_US);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(STEP_DELAY_US);
  }
  Serial.print("Rotation complétée (");
  Serial.print(steps);
  Serial.println(" pas)");
}

void unSeulPas(int stepPin) {
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(STEP_DELAY_US);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(STEP_DELAY_US);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SETUP
void setup() {
  Serial.begin(9600);
  attente(DELAY_1SEC);
  
  initMotors();
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);
  attente(DELAY_1SEC);

  pinMode(SENSOR_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  attente(DELAY_1SEC);
  
  sensRotation(DIR_A, HORAIRE);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOOP
void loop() {
  
  if(digitalRead(SENSOR_PIN) == LOW) {
    // Capteur détecté : faire un pas
    digitalWrite(STEP_A, HIGH);
    delayMicroseconds(STEP_DELAY_US);
    digitalWrite(STEP_A, LOW);
    delayMicroseconds(STEP_DELAY_US);
  } else {
    // Capteur non détecté : faire 10 pas en sens horaire
    sensRotation(DIR_A, testsens);
    testsens = !testsens;
    for(int i = 0; i < 10; i++) {
      digitalWrite(STEP_A, HIGH);
      delayMicroseconds(STEP_DELAY_US);
      digitalWrite(STEP_A, LOW);
      delayMicroseconds(STEP_DELAY_US);
    }
    delay(1000);
  }
}
