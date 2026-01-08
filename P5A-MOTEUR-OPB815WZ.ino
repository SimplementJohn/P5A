/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//VARIABLES CAPTEUR OPB815WZ
/*
  OPB815WZ (4 fils) -> ESP32
  - Rouge  (Anode)     -> 3V3 via résistance 220Ω
  - Noir   (Cathode)   -> GND
  - Blanc  (Collector) -> GPIO27 (pull-up interne)
  - Vert   (Emitter)   -> GND

  ATTENTION: Le phototransistor a besoin d'une résistance pull-up!
  
  Fonctionnement:
  - Faisceau NON coupé -> Phototransistor passant -> GPIO27 = LOW
  - Faisceau COUPÉ     -> Phototransistor bloqué  -> GPIO27 = HIGH (pull-up)
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//VARIABLES MOTEURS
#define EN      4       // Pin Enable pour activer/désactiver les drivers moteurs (LOW = actif, HIGH = inactif)
#define DIR_A   16      // Pin Direction du moteur A (définit le sens de rotation)
#define STEP_A  17      // Pin Step du moteur A (chaque impulsion = 1 pas)

#define HORAIRE       HIGH    // Rotation dans le sens des aiguilles d'une montre
#define ANTIHORAIRE   LOW     // Rotation dans le sens inverse des aiguilles d'une montre

const int STEP_DELAY_US = 2000;         // Délai entre chaque pas en microsecondes (contrôle la vitesse)

//VARIABLES CAPTEUR
const int SENSOR_PIN = 27;      // Pin GPIO du capteur OPB815WZ (entrée phototransistor)
const int LED_PIN = 2;          // Pin GPIO de la LED indicatrice de détection

// Variable pour l'état du moteur
bool moteurEnMarche = true;     // Flag indiquant si le moteur doit tourner

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SETUP
void setup() {
  Serial.begin(115200);               // Initialisation de la communication série
  delay(1000);                        // Attente de 1 seconde pour stabilisation
  
  // Configuration des pins moteur
  pinMode(STEP_A, OUTPUT);            // Configure STEP_A en sortie
  pinMode(DIR_A, OUTPUT);             // Configure DIR_A en sortie
  pinMode(EN, OUTPUT);                // Configure EN en sortie
  digitalWrite(EN, HIGH);             // Désactive les drivers au démarrage
  
  // Configuration du capteur
  pinMode(SENSOR_PIN, INPUT_PULLUP);  // Configure le pin capteur en entrée avec pull-up interne
  pinMode(LED_PIN, OUTPUT);           // Configure le pin LED en sortie
  digitalWrite(LED_PIN, LOW);         // Éteint la LED au démarrage
  
  
  // Lecture initiale du capteur
  Serial.print("Etat initial du capteur (GPIO27): ");
  Serial.println(digitalRead(SENSOR_PIN) ? "HIGH (faisceau coupe)" : "LOW (faisceau OK)");
  Serial.println("");
  
  delay(2000);                        // Pause avant de démarrer
  
  // Activation des drivers moteurs
  Serial.println(">>> Activation des drivers moteurs...");
  digitalWrite(EN, LOW);              // Active les drivers (EN = LOW = actif)
  delay(500);                         // Attente pour stabilisation
  
  // Définir le sens de rotation
  digitalWrite(DIR_A, HORAIRE);       // Sens horaire
  Serial.println(">>> Demarrage du moteur A en sens HORAIRE");
  Serial.println("");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOOP
void loop() {
  // Lire l'état du capteur
  // HIGH = faisceau coupé (pull-up actif car phototransistor bloqué)
  // LOW = faisceau OK (phototransistor passant)
  bool faisceauCoupe = (digitalRead(SENSOR_PIN) == HIGH);
  
  if (faisceauCoupe) {
    // Faisceau coupé -> Arrêter le moteur
    if (moteurEnMarche) {
      moteurEnMarche = false;
      Serial.println("");
      Serial.println("!!! COUPURE DETECTEE !!!");
      Serial.println(">>> Moteur A ARRETE");
      digitalWrite(LED_PIN, HIGH);    // Allumer la LED
    }
  } else {
    // Faisceau OK -> Le moteur tourne
    if (!moteurEnMarche) {
      moteurEnMarche = true;
      Serial.println("");
      Serial.println(">>> FAISCEAU RETABLI !");
      Serial.println(">>> Moteur A REDEMARRE");
      digitalWrite(LED_PIN, LOW);     // Éteindre la LED
    }
    
    // Faire un pas moteur
    digitalWrite(STEP_A, HIGH);         // Impulsion HIGH sur le pin STEP
    delayMicroseconds(STEP_DELAY_US);   // Maintient l'impulsion
    digitalWrite(STEP_A, LOW);          // Fin de l'impulsion
    delayMicroseconds(STEP_DELAY_US);   // Délai avant le prochain pas
  }
}
