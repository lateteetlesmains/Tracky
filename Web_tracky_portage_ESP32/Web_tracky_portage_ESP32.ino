#include <WiFi.h>
#include <ESP32Servo.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_NeoPixel.h>

#include <FS_MX1508.h>

#define ssid "🏎Tracky🏎"
#define password ""

// Définition des pins utilisés pour les NeoPixels et le buzzer
#define NEOPIXEL_PIN 1 // GPIO01, aussi connu comme TX

#define NUMPIXELS 2

#define Pin_Klaxon D3

// Définition des broches pour les moteurs avant
const int motorAvant1Pin1 = D4;
const int motorAvant1Pin2 = D3;
const int motorAvant2Pin1 = D2;
const int motorAvant2Pin2 = D1;

// Définition des broches pour les moteurs arrière
const int motorArriere1Pin1 = D7;
const int motorArriere1Pin2 = D6;
const int motorArriere2Pin1 = D5;
const int motorArriere2Pin2 = D0;

MX1508 motorAvantGauche(motorAvant1Pin1, motorAvant1Pin2); // default SLOW_DECAY (resolution 8 bits, frequency 1000Hz)
MX1508 motorAvantdroite(motorAvant2Pin1, motorAvant2Pin2); // default SLOW_DECAY (resolution 8 bits, frequency 1000Hz)
MX1508 motorArriereGauche(motorArriere1Pin1, motorArriere1Pin2); // default SLOW_DECAY (resolution 8 bits, frequency 1000Hz)
MX1508 motorArrieredroite(motorArriere2Pin1, motorArriere2Pin2); // default SLOW_DECAY (resolution 8 bits, frequency 1000Hz)

// Création de l'objet pixels pour contrôler les NeoPixels
//Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//void initNeoPixel() {
//  pixels.begin(); // Initialisation de la bande de NeoPixels
//  pinMode(Pin_Klaxon, OUTPUT); // Définit le pin du buzzer comme sortie
//}

void init_Moteur() {
    motorAvantGauche.motorBrake();
    motorAvantdroite.motorBrake();
    motorArriereGauche.motorBrake();
    motorArrieredroite.motorBrake();
}

void setupWiFi() {
   // Connect to Wi-Fi
  WiFi.softAP(ssid, password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

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

//void changeColor() {
//  for (int i = 0; i < NUMPIXELS; i++) {
//    pixels.setPixelColor(i, pixels.Color(random(255), random(255), random(255)));
//  }
//  pixels.show();
//}

//void setHeadlightsColor(int r, int g, int b) {
//  pixels.setPixelColor(0, pixels.Color(r, g, b));
//  pixels.setPixelColor(1, pixels.Color(r, g, b));
//  pixels.show();
//}

void stopTout() {
   motorAvantGauche.motorBrake();
    motorAvantdroite.motorBrake();
    motorArriereGauche.motorBrake();
    motorArrieredroite.motorBrake();
}

void avancer() {
//  for (int pwm = 0; pwm <= 200; pwm++) { // ramp up forward.
//    motorAvantGauche.motorGo(pwm);
//    motorAvantdroite.motorGo(-pwm);
//    motorArriereGauche.motorGo(pwm);
//    motorArrieredroite.motorGo(-pwm);
//    delay(10);
//  }
motorArriereGauche.motorGo(255);
}

void reculer() {
  for (int pwm = 0; pwm <= 200; pwm++) { // ramp up forward.
    motorAvantGauche.motorGo(-pwm);
    motorAvantdroite.motorGo(pwm);
    motorArriereGauche.motorGo(-pwm);
    motorArrieredroite.motorGo(pwm);
    delay(10);
  }
}

void tournerGauche() {
 for (int pwm = 0; pwm <= 200; pwm++) { // ramp up forward.
    motorAvantGauche.motorGo(pwm);
    motorAvantdroite.motorGo(pwm);
    motorArriereGauche.motorGo(-pwm);
    motorArrieredroite.motorGo(-pwm);
    delay(10);
  }
}

void tournerDroite() {
  for (int pwm = 0; pwm <= 200; pwm++) { // ramp up forward.
    motorAvantGauche.motorGo(-pwm);
    motorAvantdroite.motorGo(-pwm);
    motorArriereGauche.motorGo(pwm);
    motorArrieredroite.motorGo(pwm);
    delay(10);
  }
}


  void setupWebServer() {
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/jquery.min.js", "application/javascript");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/script.js", "application/javascript");
  });

  server.on("/testbuzzer", HTTP_GET, [](AsyncWebServerRequest *request){
    TestBuzzer();
    Serial.println("Requête reçue : /testbuzzer");
    request->send(200, "text/plain", "buzzer OK");
});

 
 // Gérer les requêtes pour chaque bouton de contrôle moteur
server.on("/forward", HTTP_GET, [](AsyncWebServerRequest *request) {
  Serial.println("Requête reçue : Avancer");
  avancer();
  // Implémentez la logique de contrôle du moteur 1 pour avancer ici
  request->send(200, "text/plain", "Avancer");
});

server.on("/backward", HTTP_GET, [](AsyncWebServerRequest *request) {
  Serial.println("Requête reçue : Reculer");
  reculer();
  // Implémentez la logique de contrôle du moteur 1 pour reculer ici
  request->send(200, "text/plain", "Recule");
});

server.on("/left", HTTP_GET, [](AsyncWebServerRequest *request) {
  Serial.println("Requête reçue : Gauche");
  tournerGauche();
  // Implémentez la logique de contrôle du moteur 3 pour avancer ici
  request->send(200, "text/plain", "Gauche");
});

server.on("/right", HTTP_GET, [](AsyncWebServerRequest *request) {
  Serial.println("Requête reçue : Droite");
  tournerDroite();
  // Implémentez la logique de contrôle du moteur 3 pour avancer ici
  request->send(200, "text/plain", "Droite");
});

server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request) {
  Serial.println("Requête reçue : stop");
  stopTout();
  // Implémentez la logique de contrôle du moteur 3 pour avancer ici
  request->send(200, "text/plain", "stop");
});


  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404, "text/plain", "Not found");
  });

  // Start server
  server.begin();
}
void setup() {
  Serial.begin(115200);
  // Start SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  setupWiFi();
  setupWebServer();
}
void loop() {
  //server.handleClient();
}
