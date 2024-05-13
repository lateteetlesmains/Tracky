#include <WiFi.h>
#include <WebServer.h>
#include <FS.h>
#include <Adafruit_NeoPixel.h>

#define Nom_SSID "️Tracky_Terence️"

// Définition des pins utilisés pour les NeoPixels et le buzzer
#define NEOPIXEL_PIN 1 // GPIO01, aussi connu comme TX

#define NUMPIXELS 2

#define Pin_Klaxon D3

// Définition des broches pour les moteurs avant
const int motorAvant1Pin1 = D7;
const int motorAvant1Pin2 = D6;
const int motorAvant2Pin1 = D5;
const int motorAvant2Pin2 = D0;

// Définition des broches pour les moteurs arrière
const int motorArriere1Pin1 = D8;
const int motorArriere1Pin2 = D4;
const int motorArriere2Pin1 = D1;
const int motorArriere2Pin2 = D2;

// Création de l'objet pixels pour contrôler les NeoPixels
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Création de l'objet server pour le serveur web
WebServer server(80);

void initNeoPixel() {
  pixels.begin(); // Initialisation de la bande de NeoPixels
  pinMode(Pin_Klaxon, OUTPUT); // Définit le pin du buzzer comme sortie
}

void init_Moteur() {
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
  WiFi.softAP(Nom_SSID); // Crée un point d'accès WiFi avec un SSID émotif
  Serial.print("Point d'accès démarré, IP : ");
  Serial.println(WiFi.softAPIP()); // Affiche l'adresse IP du point d'accès
}

void TestBuzzer() {
  pinMode(Pin_Klaxon, OUTPUT);

  int beeps[] = {1933, 2156, 1863, 1505, 1816, 1933, 1729, 2291};
  int buzzVols[] = {144, 180, 216, 252, 252, 252, 252, 216, 180, 144};

  int i = 10;
  while (i > 0) {
    tone(Pin_Klaxon, 1050, buzzVols[i]);
    delayMicroseconds(20);
    tone(Pin_Klaxon, 1050, buzzVols[i] / 8);
    delayMicroseconds(40);
    i--;
  }

  delay(35);

  i = 0;
  while (i < 8) {
    int v = 0;
    while (v < 250) { // 12.5 mS fade up time
      tone(Pin_Klaxon, beeps[i], v);
      v += 10;
      delayMicroseconds(2);
    }
    delay(20);
    v = 250;
    while (v > 0) { // 12.5 mS fade down time
      tone(Pin_Klaxon, beeps[i], v);
      v -= 10;
      delayMicroseconds(5);
    }
    noTone(Pin_Klaxon);
    delay(35);
    i++;
  }

  int f = 2466;
  while (f < 2825) {
    tone(Pin_Klaxon, f, 255);
    f += 3;
    delay(1);
  }
  f = 2825;
  while (f > 2466) {
    tone(Pin_Klaxon, f, 255);
    f -= 3;
    delay(1);
  }
  noTone(Pin_Klaxon);
}

void changeColor() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(random(255), random(255), random(255)));
  }
  pixels.show();
}

void setHeadlightsColor(int r, int g, int b) {
  pixels.setPixelColor(0, pixels.Color(r, g, b));
  pixels.setPixelColor(1, pixels.Color(r, g, b));
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

void avancer() {
  digitalWrite(motorAvant1Pin1, HIGH);
  digitalWrite(motorAvant1Pin2, LOW);
  digitalWrite(motorAvant2Pin1, HIGH);
  digitalWrite(motorAvant2Pin2, LOW);

  digitalWrite(motorArriere1Pin1, LOW);
  digitalWrite(motorArriere1Pin2, LOW);
  digitalWrite(motorArriere2Pin1, LOW);
  digitalWrite(motorArriere2Pin2, LOW);
}

void reculer() {
  digitalWrite(motorAvant1Pin1, LOW);
  digitalWrite(motorAvant1Pin2, LOW);
  digitalWrite(motorAvant2Pin1, LOW);
  digitalWrite(motorAvant2Pin2, LOW);

  digitalWrite(motorArriere1Pin1, HIGH);
  digitalWrite(motorArriere1Pin2, LOW);
  digitalWrite(motorArriere2Pin1, HIGH);
  digitalWrite(motorArriere2Pin2, LOW);
}

void tournerGauche() {
  digitalWrite(motorAvant1Pin1, HIGH);
  digitalWrite(motorAvant1Pin2, LOW);
  digitalWrite(motorAvant2Pin1, LOW);
  digitalWrite(motorAvant2Pin2, LOW);

  digitalWrite(motorArriere1Pin1, LOW);
  digitalWrite(motorArriere1Pin2, LOW);
  digitalWrite(motorArriere2Pin1, HIGH);
  digitalWrite(motorArriere2Pin2, LOW);
}

void tournerDroite() {
  digitalWrite(motorAvant1Pin1, LOW);
  digitalWrite(motorAvant1Pin2, LOW);
  digitalWrite(motorAvant2Pin1, HIGH);
  digitalWrite(motorAvant2Pin2, LOW);

  digitalWrite(motorArriere1Pin1, HIGH);
  digitalWrite(motorArriere1Pin2, LOW);
  digitalWrite(motorArriere2Pin1, LOW);
  digitalWrite(motorArriere2Pin2, LOW);
}


  void setupWebServer() {
  server.on("/", []() {
    server.sendHeader("Content-Type", "text/html");
    server.send(200, "<!DOCTYPE html><html><head><title>Tracky Terence</title></head><body><h1>Contrôle de la voiture</h1><p><a href=\"/avancer\">Avancer</a></p><p><a href=\"/reculer\">Reculer</a></p><p><a href=\"/gauche\">Tourner à gauche</a></p><p><a href=\"/droite\">Tourner à droite</a></p><p><a href=\"/stop\">Arrêter</a></p><p><a href=\"/changecolor\">Changer de couleur</a></p><p><a href=\"/testbuzzer\">Tester le buzzer</a></p><p><a href=\"/setheadlightscolor?r=255&g=0&b=0\">Mettre les phares rouges</a></p><p><a href=\"/setheadlightscolor?r=0&g=255&b=0\">Mettre les phares verts</a></p><p><a href=\"/setheadlightscolor?r=0&g=0&b=255\">Mettre les phares bleus</a></p></body></html>"); // Closing quotation mark added here
  });

  server.on("/avancer", avancer);
  server.on("/reculer", reculer);
  server.on("/gauche", tournerGauche);
  server.on("/droite", tournerDroite);
  server.on("/stop", stopTout);
  server.on("/changecolor", changeColor);
  server.on("/testbuzzer", TestBuzzer);
  server.on("/setheadlightscolor", []() {
    int r = server.arg("r").toInt();
    int g = server.arg("g").toInt();
    int b = server.arg("b").toInt();
    setHeadlightsColor(r, g, b);
    server.send(200, "Couleur des phares modifiée");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
