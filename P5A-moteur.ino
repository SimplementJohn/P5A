#define EN      4
#define DIR_A   16
#define STEP_A  17

#define DIR_B   15
#define STEP_B  2

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  initMotors();
  
  Serial.println("=== Activation des drivers (EN = LOW) ===");
  digitalWrite(EN, LOW); // Active les drivers
  delay(500);
}

void loop() {
  // ======= TEST MOTEUR A =======
  Serial.println("\n>>> MOTEUR A - Sens horaire (DIR = HIGH)");
  digitalWrite(DIR_A, HIGH);
  ledcWrite(STEP_A, 128); // Démarre PWM à 1000Hz (défini dans init)
  delay(3000); // Tourne pendant 3 secondes
  
  Serial.println(">>> MOTEUR A - ARRET");
  ledcWrite(STEP_A, 0); // Arrêt du PWM
  delay(1000);
  
  Serial.println(">>> MOTEUR A - Sens anti-horaire (DIR = LOW)");
  digitalWrite(DIR_A, LOW);
  ledcWrite(STEP_A, 128); // Redémarre PWM
  delay(3000);
  
  Serial.println(">>> MOTEUR A - ARRET");
  ledcWrite(STEP_A, 0);
  delay(2000);
  
  // ======= TEST MOTEUR B =======
  Serial.println("\n>>> MOTEUR B - Sens horaire (DIR = HIGH)");
  digitalWrite(DIR_B, HIGH);
  ledcWrite(STEP_B, 128);
  delay(3000);
  
  Serial.println(">>> MOTEUR B - ARRET");
  ledcWrite(STEP_B, 0);
  delay(1000);
  
  Serial.println(">>> MOTEUR B - Sens anti-horaire (DIR = LOW)");
  digitalWrite(DIR_B, LOW);
  ledcWrite(STEP_B, 128);
  delay(3000);
  
  Serial.println(">>> MOTEUR B - ARRET");
  ledcWrite(STEP_B, 0);
  
  Serial.println("\n=== Cycle terminé - Pause 5 secondes ===\n");
  delay(5000);
}

void initMotors() {
  pinMode(EN, OUTPUT);
  ledcAttachChannel(STEP_A, 1000, 8, 0); // 1000Hz, résolution 8-bit, canal 0
  pinMode(DIR_A, OUTPUT);

  ledcAttachChannel(STEP_B, 1000, 8, 1); // 1000Hz, résolution 8-bit, canal 1
  pinMode(DIR_B, OUTPUT);

  digitalWrite(EN, HIGH); // Désactivé au démarrage
}
