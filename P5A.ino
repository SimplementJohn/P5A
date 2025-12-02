#include <ArduinoJson.h>   // JSON
#include <WiFi.h>          // WiFi ESP32
#include <HTTPClient.h>    // HTTP client

// PARAMÈTRES LOCALISATION (Montpellier)
String latitudeMontpellier  = "43.6002";   // latitude EPF
String longitudeMontpellier = "3.8716";    // longitude EPF
String elevation = "40";                   // Élévation en mètres EPF
String timezone = "1";                     // Fuseau horaire (UTC+1 pour la France)

// CLÉ API APIVERVE
String apiKey = "f5768f22-16db-413c-8aea-daef5b41193b";

// Variables pour stocker les valeurs du soleil
String elevationSoleil = "";
String azimutSoleil    = "";



// JSON reçu de l'API (vide au départ)
String jsonSUN = "";

// PARAMÈTRES WIFI
const char* ssid     = "iPhone de Alexis";
const char* password = "11223344";

// MONITEUR SÉRIE
int BAUDRATE = 9600;  // 115200 pour ESP32 (standard)

/////////////////////////////////////////////////////////////////////////////
// DECLARATIONS FONCTIONS
void maintainWiFi();
String getAzimuth(const char* json);
String getAltitude(const char* json);
String getSunJSON();

/////////////////////////////////////////////////////////////////////////////
// SETUP
void setup() {
  Serial.begin(BAUDRATE);
  delay(500);

  Serial.println();
  Serial.println("=== DEMARRAGE ESP32 SUNPOSITION ===");

  // Connexion WiFi au démarrage
  maintainWiFi();

  Serial.println("FIN DE BOUCLE SETUP");
  Serial.println();
}

/////////////////////////////////////////////////////////////////////////////
// LOOP
void loop() {

  // MAINTIEN / RECONNEXION WIFI
  maintainWiFi();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi OK → Exécution du programme !");

    // 1) Récupération du JSON depuis l'API
    getSunJSON();

    // 2) Vérification et affichage
    if (jsonSUN.length() > 0) {
      Serial.println("JSON reçu :");
      Serial.println(jsonSUN);
      Serial.println();

      // 3) Extraction des valeurs pour le Soleil
      elevationSoleil = getAltitude(jsonSUN.c_str());
      azimutSoleil    = getAzimuth(jsonSUN.c_str());

      // 4) Affichage
      Serial.print("ALTITUDE (Soleil) : ");
      Serial.println(elevationSoleil);
      Serial.print("AZIMUT (Soleil) : ");
      Serial.println(azimutSoleil);

    } else {
      Serial.println("Erreur : JSON vide, impossible d'extraire les valeurs.");
    }

    Serial.println("FIN DE BOUCLE LOOP");
    Serial.println();

  } else {
    Serial.println("Pas de WiFi → programme en pause.");
  }

  delay(5000); // ATTENTE 5 secondes avant prochaine requête
}

/////////////////////////////////////////////////////////////////////////////
// FONCTIONS
/////////////////////////////////////////////////////////////////////////////
// Connexion / maintien WiFi
void maintainWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    return; // déjà connecté
  }

  Serial.print("Connexion au WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  unsigned long startAttemptTime = millis();

  // On attend max 10 secondes
  while (WiFi.status() != WL_CONNECTED &&
         millis() - startAttemptTime < 10000) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connecté au WiFi, IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Échec connexion WiFi.");
  }
}

/////////////////////////////////////////////////////////////////////////////
// Extraction de l'azimut du Soleil depuis le JSON (format positions array)
String getAzimuth(const char* json) {
  StaticJsonDocument<4096> doc;
  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.print("Erreur JSON (azimuth): ");
    Serial.println(error.c_str());
    return "NAN";
  }

  // Parcourir le tableau positions pour trouver "Soleil"
  JsonArray positions = doc["positions"];
  for (JsonObject pos : positions) {
    const char* name = pos["name"];
    if (strcmp(name, "Soleil") == 0) {
      const char* az = pos["az"];
      return String(az);
    }
  }

  return "NAN";
}

/////////////////////////////////////////////////////////////////////////////
// Extraction de l'altitude du Soleil depuis le JSON (format positions array)
String getAltitude(const char* json) {
  StaticJsonDocument<4096> doc;
  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.print("Erreur JSON (altitude): ");
    Serial.println(error.c_str());
    return "NAN";
  }

  // Parcourir le tableau positions pour trouver "Soleil"
  JsonArray positions = doc["positions"];
  for (JsonObject pos : positions) {
    const char* name = pos["name"];
    if (strcmp(name, "Soleil") == 0) {
      const char* alt = pos["alt"];
      return String(alt);
    }
  }

  return "NAN";
}

/////////////////////////////////////////////////////////////////////////////
// Appel de la nouvelle API le-systeme-solaire.net - Retourne le JSON dans jsonSUN
String getSunJSON() {
  // Construction de l'URL avec tous les paramètres
  String url = "https://api.le-systeme-solaire.net/rest/positions?lon=" +
               longitudeMontpellier + "&lat=" + latitudeMontpellier +
               "&elev=" + elevation + "&datetime=2025-11-28T08:00:00&zone=" + timezone;

  Serial.print("Requête HTTP: ");
  Serial.println(url);

  HTTPClient http;
  http.begin(url);
  
  // Header Authorization avec Bearer token
  http.addHeader("Authorization", "Bearer 277cf5ac-ea8d-46ce-aba0-7e12e31bf076");

  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.print("Code HTTP : ");
    Serial.println(httpCode);

    if (httpCode == HTTP_CODE_OK) {
      jsonSUN = http.getString();  // Stocke directement dans jsonSUN
      http.end();
      
      Serial.println("JSON stocké dans jsonSUN avec succès");
      return jsonSUN;
    } else {
      Serial.println("Réponse non OK de l'API.");
      http.end();
      return "";
    }
  } else {
    Serial.print("Erreur HTTP : ");
    Serial.println(httpCode);
    http.end();
    return "";
  }
}
