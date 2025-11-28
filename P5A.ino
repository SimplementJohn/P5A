#include <ArduinoJson.h>   // JSON
#include <WiFi.h>          // WiFi ESP32
#include <HTTPClient.h>    // HTTP client

// PARAMÈTRES LOCALISATION (Montpellier)
String latitudeMontpellier  = "43.3640";
String longitudeMontpellier = "3.5236";

// CLÉ API APIVERVE
String apiKey = "f5768f22-16db-413c-8aea-daef5b41193b";

// Variables pour stocker les valeurs du soleil
String elevationSoleil = "";
String azimutSoleil    = "";
/*
// JSON de test statique (pour debug sans API)
const char* jsonAPI = R"JSON(
{
    "positions": [
        {
            "name": "Soleil",
            "ra": "16h 13min 53s",
            "dec": "-21°11'54\"",
            "az": "149°51'14\"",
            "alt": "19°23'24\""
        },
        {
            "name": "Lune",
            "ra": "21h 52min 45s",
            "dec": "-15°27'20\"",
            "az": "84°03'35\"",
            "alt": "-28°47'15\""
        },
        {
            "name": "Mercure",
            "ra": "15h 17min 39s",
            "dec": "-15°49'45\"",
            "az": "161°52'41\"",
            "alt": "28°40'27\""
        },
        {
            "name": "Venus",
            "ra": "15h 34min 12s",
            "dec": "-18°22'07\"",
            "az": "158°19'33\"",
            "alt": "25°12'00\""
        },
        {
            "name": "Mars",
            "ra": "17h 01min 39s",
            "dec": "-23°23'46\"",
            "az": "140°35'37\"",
            "alt": "12°29'09\""
        },
        {
            "name": "Jupiter",
            "ra": "7h 46min 48s",
            "dec": "21°19'18\"",
            "az": "289°46'45\"",
            "alt": "10°13'43\""
        },
        {
            "name": "Saturne",
            "ra": "23h 46min 00s",
            "dec": "-4°06'29\"",
            "az": "49°44'24\"",
            "alt": "-39°04'58\""
        },
        {
            "name": "Uranus",
            "ra": "3h 48min 12s",
            "dec": "19°46'44\"",
            "az": "335°20'19\"",
            "alt": "-22°51'29\""
        },
        {
            "name": "Neptune",
            "ra": "0h 00min 00s",
            "dec": "-1°28'33\"",
            "az": "44°08'21\"",
            "alt": "-38°42'02\""
        },
        {
            "name": "Pluton",
            "ra": "20h 19min 57s",
            "dec": "-23°24'54\"",
            "az": "106°07'10\"",
            "alt": "-17°19'59\""
        }
    ],
    "location": {
        "latitude": 43.6108,
        "longitude": 3.8767,
        "elevation": 0,
        "timezone": 1
    },
    "time_info": {
        "calculated_for_utc": "2025-11-27 09:30:00 UTC",
        "local_time_display": "2025-11-27 10:30:00 (UTC+1)",
        "universal_time_ut": "9h 30min 00s",
        "universal_time_decimal": 9.5,
        "julian_day": 2461006.895833,
        "day_number_j2000": 9463.4,
        "greenwich_sidereal_time": "13h 56min 14s",
        "local_sidereal_time": "14h 11min 44s",
        "gst_decimal": 13.9372,
        "lst_decimal": 14.1957
    }
}
)JSON";
*/


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
  // Construction de l'URL avec tous les paramètres nécessaires
  String url = "https://api.le-systeme-solaire.net/rest/positions?lon=" +
               longitudeMontpellier + "&lat=" + latitudeMontpellier +
               "&elev=0&datetime=2025-11-28T08:00:00&zone=1";

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
