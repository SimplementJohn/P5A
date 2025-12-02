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


// Nombre de pas pour 360° (1 tour complet)
const int STEPS_PER_REVOLUTION = 200;
const int STEP_DELAY_US = 2000;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SETUP
void setup() {
  Serial.begin(9600);
  delay(1000);
  
  initMotors();
  
  Serial.println("=== Activation des drivers (EN = LOW) ===");
  digitalWrite(EN, LOW);
  delay(500);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOOP

void loop() {
  // ======= TEST MOTEUR A - 360° =======
  Serial.println("\n>>> MOTEUR A - Rotation 360° sens HORAIRE");
  sensRotation(DIR_A, HORAIRE);
  rotate360(STEP_A, STEPS_PER_REVOLUTION);
  
  Serial.println(">>> MOTEUR A - Pause 2 secondes");
  attente(DELAY_5SEC);
  
  Serial.println(">>> MOTEUR A - Rotation 360° sens ANTIHORAIRE");
  sensRotation(DIR_A, ANTIHORAIRE);
  rotate360(STEP_A, STEPS_PER_REVOLUTION);
  
  attente(DELAY_1SEC);
  
  // ======= TEST MOTEUR B - 360° =======
  Serial.println("\n>>> MOTEUR B - Rotation 360° sens HORAIRE");
  sensRotation(DIR_B, HORAIRE);
  rotate360(STEP_B, STEPS_PER_REVOLUTION);
  
  Serial.println(">>> MOTEUR B - Pause 2 secondes");
  attente(DELAY_1SEC);
  
  Serial.println(">>> MOTEUR B - Rotation 360° sens ANTIHORAIRE");
  sensRotation(DIR_B, ANTIHORAIRE);
  rotate360(STEP_B, STEPS_PER_REVOLUTION);
  
  Serial.println("\n=== Cycle terminé - Pause 5 secondes ===\n");
  attente(DELAY_5SEC);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FONCTIONS

// Fonction pour définir le sens de rotation
void sensRotation(int dirPin, bool sens) {
  digitalWrite(dirPin, sens);
  
  // Affichage optionnel pour debug
  if (sens == HORAIRE) {
    Serial.print("Sens: HORAIRE (PIN ");
  } else {
    Serial.print("Sens: ANTIHORAIRE (PIN ");
  }
  Serial.print(dirPin);
  Serial.println(")");
}

// Fonction pour faire 360° (1 tour complet)
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

void initMotors() {       //Initialisation des moteurs en OUTPUT et la broche EN en HIGH pour que les moteurs puissent bouger
  pinMode(STEP_A, OUTPUT);
  pinMode(DIR_A, OUTPUT);
  pinMode(STEP_B, OUTPUT);
  pinMode(DIR_B, OUTPUT);
  digitalWrite(EN, HIGH); //Mise en OFF des moteurs 
}


void attente(int duree_ms) {
  delay(duree_ms);
}
