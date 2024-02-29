#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <Adafruit_NeoPixel.h>

// Définition des pins utilisés pour les NeoPixels et le buzzer
#define NEOPIXEL_PIN 1// GPIO01, aussi connu comme TX

#define NUMPIXELS    2

#define Pin_Klaxon      D8

// Définition des broches pour les moteurs avant
const int motorAvant1Pin1 = D7;
const int motorAvant1Pin2 = D6;
const int motorAvant2Pin1 = D5;
const int motorAvant2Pin2 = D0;

// Définition des broches pour les moteurs arrière
const int motorArriere1Pin1 = D4;
const int motorArriere1Pin2 = D3;
const int motorArriere2Pin1 = D2;
const int motorArriere2Pin2 = D1;

// Création de l'objet pixels pour contrôler les NeoPixels
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Création de l'objet server pour le serveur web
ESP8266WebServer server(80);
void initNeoPixel() {
  pixels.begin(); // Initialisation de la bande de NeoPixels
  pinMode(Pin_Klaxon, OUTPUT); // Définit le pin du buzzer comme sortie
  
}


void init_Moteur()
{
  pinMode(motorAvant1Pin1, OUTPUT);
  pinMode(motorAvant1Pin2, OUTPUT);
  pinMode(motorAvant2Pin1, OUTPUT);
  pinMode(motorAvant2Pin2, OUTPUT);
  
  pinMode(motorArriere1Pin1, OUTPUT);
  pinMode(motorArriere1Pin2, OUTPUT);
  pinMode(motorArriere2Pin1, OUTPUT);
  pinMode(motorArriere2Pin2, OUTPUT);
  
}
void setupWiFi() {
  WiFi.softAP("🏎️Tracky_Jules🏎️"); // Crée un point d'accès WiFi avec un SSID émotif
  Serial.print("Point d'accès démarré, IP : ");
  Serial.println(WiFi.softAPIP()); // Affiche l'adresse IP du point d'accès
}

void TestBuzzer() {
  pinMode(Pin_Klaxon,OUTPUT);
  
  int beeps[] = {1933, 2156, 1863, 1505, 1816, 1933, 1729, 2291};
  int buzzVols[] = {144, 180, 216, 252, 252, 252, 252, 216, 180, 144};

  int i = 10;
  while (i > 0) {
    tone(Pin_Klaxon,1050, buzzVols[i]);
    delayMicroseconds(20);
    tone(Pin_Klaxon,1050, buzzVols[i] / 8);
    delayMicroseconds(40);
    i--;
  }

  delay(35);

  i = 0;
  while (i < 8) {
    int v = 0;
    while (v < 250) { // 12.5 mS fade up time
      tone(Pin_Klaxon,beeps[i], v);
      v += 10;
      delayMicroseconds(2);
    }
    delay(20);
    v = 250;
    while (v > 0) { // 12.5 mS fade down time
      tone(Pin_Klaxon,beeps[i], v);
      v -= 10;
      delayMicroseconds(5);
    }
    noTone(Pin_Klaxon);
    delay(35);
    i++;
  }

  int f = 2466;
  while (f < 2825) {
    tone(Pin_Klaxon,f, 255);
    f += 3;
    delay(1);
  }
  f = 2825;
  int v = 255;
  while (f > 2000) {
    tone(Pin_Klaxon,f, v);
    f -= 6;
    v -= 1;
    delay(1);
  }
  noTone(Pin_Klaxon);
  delay(35);

  i = 10;
  while (i > 0) {
    tone(Pin_Klaxon,1050, buzzVols[i]);
    delayMicroseconds(20);
    tone(Pin_Klaxon,1050, buzzVols[i] / 8);
    delayMicroseconds(40);
    i--;
  }
  noTone(Pin_Klaxon);
   pinMode(D0,INPUT);
}
  

void changeColor() {
  // Change la couleur de chaque pixel NeoPixel aléatoirement
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(random(0, 255), random(0, 255), random(0, 255)));
  }
  pixels.show(); // Met à jour les NeoPixels avec les nouvelles couleurs
}
void phareBleu() {
  // Change la couleur de chaque pixel NeoPixel aléatoirement
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 255));
  }
  pixels.show(); // Met à jour les NeoPixels avec les nouvelles couleurs
}

void setHeadlightsColor(String leftColor, String rightColor) {
  // Convertit les couleurs hexadécimales en entiers pour les composantes RGB
  long leftRGB = strtol(&leftColor[1], NULL, 16); // Enlève le '#' et convertit
  long rightRGB = strtol(&rightColor[1], NULL, 16);

  // Extrait les composantes RGB
  int leftR = (leftRGB >> 16) & 0xFF;
  int leftG = (leftRGB >> 8) & 0xFF;
  int leftB = leftRGB & 0xFF;

  int rightR = (rightRGB >> 16) & 0xFF;
  int rightG = (rightRGB >> 8) & 0xFF;
  int rightB = rightRGB & 0xFF;

  // Applique les couleurs aux NeoPixels des phares
  pixels.setPixelColor(0, pixels.Color(leftR, leftG, leftB)); // Phare gauche
  pixels.setPixelColor(1, pixels.Color(rightR, rightG, rightB)); // Phare droit
  pixels.show();
}

void stopTout() {
  digitalWrite(motorAvant1Pin1, LOW);
  digitalWrite(motorAvant1Pin2, LOW);
  digitalWrite(motorAvant2Pin1, LOW);
  digitalWrite(motorAvant2Pin2, LOW);
  
  digitalWrite(motorArriere1Pin1, LOW);
  digitalWrite(motorArriere1Pin2, LOW);
  digitalWrite(motorArriere2Pin1, LOW);
  digitalWrite(motorArriere2Pin2, LOW);
}

// Fonction pour avancer
void avancer() {
  // Moteurs avant
  digitalWrite(motorAvant1Pin1, LOW);
  digitalWrite(motorAvant1Pin2, HIGH);
  digitalWrite(motorAvant2Pin1, LOW);
  digitalWrite(motorAvant2Pin2, HIGH);
  
  // Moteurs arrière
  digitalWrite(motorArriere1Pin1, HIGH);
  digitalWrite(motorArriere1Pin2, LOW);
  digitalWrite(motorArriere2Pin1, HIGH);
  digitalWrite(motorArriere2Pin2, LOW);
}

// Fonction pour reculer
void reculer() {
  // Moteurs avant
  digitalWrite(motorAvant1Pin1, HIGH);
  digitalWrite(motorAvant1Pin2, LOW);
  digitalWrite(motorAvant2Pin1, HIGH);
  digitalWrite(motorAvant2Pin2, LOW);
  
  // Moteurs arrière
  digitalWrite(motorArriere1Pin1, LOW);
  digitalWrite(motorArriere1Pin2, HIGH);
  digitalWrite(motorArriere2Pin1, LOW);
  digitalWrite(motorArriere2Pin2, HIGH);
}

// Fonction pour tourner à gauche
void tournerGauche() {
  // Moteurs avant tournent en arrière
  digitalWrite(motorAvant1Pin1, HIGH);
  digitalWrite(motorAvant1Pin2, LOW);
  digitalWrite(motorAvant2Pin1, HIGH);
  digitalWrite(motorAvant2Pin2, LOW);
  
  // Moteurs arrière tournent en avant
  digitalWrite(motorArriere1Pin1, HIGH);
  digitalWrite(motorArriere1Pin2, LOW);
  digitalWrite(motorArriere2Pin1, HIGH);
  digitalWrite(motorArriere2Pin2, LOW);
}

// Fonction pour tourner à droite
void tournerDroite() {
  // Moteurs avant tournent en avant
  digitalWrite(motorAvant1Pin1, LOW);
  digitalWrite(motorAvant1Pin2, HIGH);
  digitalWrite(motorAvant2Pin1, LOW);
  digitalWrite(motorAvant2Pin2, HIGH);
  
  // Moteurs arrière tournent en arrière
  digitalWrite(motorArriere1Pin1, LOW);
  digitalWrite(motorArriere1Pin2, HIGH);
  digitalWrite(motorArriere2Pin1, LOW);
  digitalWrite(motorArriere2Pin2, HIGH);
}


void setupWebServer() {
  // Route pour la page d'accueil
  server.on("/", HTTP_GET, []() {
    serveFile("/index.html", "text/html");
  });
  
  // Route pour le fichier CSS
  server.on("/style.css", HTTP_GET, []() {
    serveFile("/style.css", "text/css");
  });
  
  // Route pour l'image PNG
  server.on("/car.png", HTTP_GET, []() {
    serveFile("/car.png", "image/png");
  });

  // Route pour le fichier JavaScript
  server.on("/script.js", HTTP_GET, []() {
    serveFile("/script.js", "application/javascript");
  });

  // Route pour le changement de couleur des NeoPixels
  server.on("/changecolor", HTTP_GET, []() {
    changeColor();
    Serial.println("Requête reçue : /changecolor");
    server.send(200, "text/plain", "Couleur ok");
  });

// Route pour le changement de couleur des NeoPixels
  server.on("/testbuzzer", HTTP_GET, []() {
    TestBuzzer();
    Serial.println("Requête reçue : /testbuzzer");
    server.send(200, "text/plain", "buzzer OK");
  });
 
  // Gérer les requêtes pour chaque bouton de contrôle moteur
  server.on("/forward", HTTP_GET, []() {
  Serial.println("Requête reçue : Avancer");
  avancer();
  // Implémentez la logique de contrôle du moteur 1 pour avancer ici
  server.send(200, "text/plain", "Avancer");
});

server.on("/backward", HTTP_GET, []() {
  Serial.println("Requête reçue : Reculer");
  reculer();
  // Implémentez la logique de contrôle du moteur 1 pour reculer ici
  server.send(200, "text/plain", "Recule");
});

server.on("/left", HTTP_GET, []() {
  Serial.println("Requête reçue : Gauche");
  tournerGauche();
  // Implémentez la logique de contrôle du moteur 3 pour avancer ici
  server.send(200, "text/plain", "Gauche");
});

server.on("/right", HTTP_GET, []() {
  Serial.println("Requête reçue : Droite");
  tournerDroite();
  // Implémentez la logique de contrôle du moteur 3 pour avancer ici
  server.send(200, "text/plain", "Droite");
});

server.on("/stop", HTTP_GET, []() {
  Serial.println("Requête reçue : stop");
  stopTout();
  // Implémentez la logique de contrôle du moteur 3 pour avancer ici
  server.send(200, "text/plain", "stop");
});

// Route pour le changement de couleur des phares
server.on("/setheadlightscolor", HTTP_GET, []() {
   Serial.println("Requête reçue : /setheadlightscolor");
  String leftColor = server.arg("left");
  Serial.println(leftColor);
  String rightColor = server.arg("right");
  Serial.println(rightColor);
  setHeadlightsColor(leftColor, rightColor);
  
  server.send(200, "text/plain", "Couleur des phares changée");
});



  // Gestionnaire pour les chemins non trouvés
  server.onNotFound(handleNotFound);

  server.begin(); // Démarre le serveur web
}

void serveFile(const String& path, const String& contentType) {
  File file = SPIFFS.open(path, "r"); // Ouvre le fichier demandé
  if (!file) {
    Serial.println("Erreur de chargement du fichier " + path); // Message d'erreur si le fichier n'est pas trouvé
    server.send(404, "text/plain", "404: Il manque une pièce dans le moteur :-)");
    return;
  }
  server.streamFile(file, contentType); // Envoie le fichier au client
  file.close(); // Ferme le fichier après l'avoir envoyé
}

void listFiles(char *dir) {
  Dir root = SPIFFS.openDir(dir); // Ouvre le répertoire demandé
  while (root.next()) { // Parcourt tous les fichiers du répertoire
    Serial.println(root.fileName()); // Affiche le nom de chaque fichier
  }
}

void handleNotFound() {
  Serial.println("Page non trouvée..."); // Message d'erreur pour les chemins non trouvés
  server.send(404, "text/plain", "404: Not found");
}
void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("Erreur SPIFFS..."); // Message d'erreur si SPIFFS ne démarre pas
    return;
  }
  listFiles("/"); // Liste tous les fichiers à la racine du SPIFFS
}
void setup() {
  Serial.begin(115200); // Commence la communication série
  while (!Serial); // Attend que la connexion série soit établie.
  init_Moteur();
  initNeoPixel(); // Initialisation des NeoPixels
  initSPIFFS(); // Initialisation du système de fichiers SPIFFS
  setupWiFi(); // Configuration du WiFi
  setupWebServer(); // Configuration du serveur web
  stopTout();
  Serial.println("Serveur démarré...");
  TestBuzzer();
  phareBleu();
}

void loop() {
  server.handleClient(); // Gestion des requêtes client
}
