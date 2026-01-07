/*
  OPB815WZ (4 fils) -> ESP32
  - Rouge  (Anode)     -> 3V3 via résistance 220Ω
  - Noir   (Cathode)   -> GND
  - Blanc  (Collector) -> GPIO27 (pull-up interne)
  - Vert   (Emitter)   -> GND

  ATTENTION: Le phototransistor a besoin d'une résistance pull-up!
*/

const int SENSOR_PIN = 27;
const int LED_PIN = 2;

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(SENSOR_PIN, INPUT);  // Pas INPUT_PULLUP si résistance externe
  pinMode(SENSOR_PIN, INPUT_PULLUP);
}

void loop() {
  int state = digitalRead(SENSOR_PIN);

  // Affichage en temps réel
  Serial.print("État: ");
  if (state == HIGH) {
    Serial.println("LIBRE (HIGH)");
    digitalWrite(LED_PIN, LOW);
  } else {
    Serial.println("COUPÉ (LOW)");
    digitalWrite(LED_PIN, HIGH);
  }

 
  delay(100);  // Rafraîchissement 10x/sec
}
