#include <WiFi.h>
#include <ESP32Servo.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


#include <FS_MX1508.h>

#define ssid "Black_Swan"
#define password ""

#define start_pwm 51
#define stop_pwm_tourne 153 

//#define pos_pince_ferme_1 70
//#define pos_pince_ferme_2 140
//
//#define pos_pince_ouvre_1 100
//#define pos_pince_ouvre_2 100

#define pos_pince_ferme_1 20
#define pos_pince_ferme_2 80

#define pos_pince_ouvre_1 80
#define pos_pince_ouvre_2 20

Servo servo_1;  // create servo object to control a servo
Servo servo_2;  // create servo object to control a servo

// GPIO the servo is attached to
int servoPin_1 = 27;
int servoPin_2 = 33

;

// Définition des broches pour les moteurs avant
const int motorAvantGauchePin1 = D4;  // Avant gauche
const int motorAvantGauchePin2 = D3;
const int motorAvantDroitePin1 = D2;   // Avant droite
const int motorAvantDroitePin2 = 25;

// Définition des broches pour les moteurs arrière
const int motorArriereGauchePin1 = D5;  // Arrière gauche (inversé)
const int motorArriereGauchePin2 = D0;
const int motorArriereDroitePin1 = D7 ;   // Arrière droite (inversé)
const int motorArriereDroitePin2 = D6 ;

// Création des objets moteurs avec FS_MX1508
MX1508 motorAvantGauche(motorAvantGauchePin1, motorAvantGauchePin2);
MX1508 motorAvantDroite(motorAvantDroitePin1, motorAvantDroitePin2);
MX1508 motorArriereGauche(motorArriereGauchePin1, motorArriereGauchePin2);
MX1508 motorArriereDroite(motorArriereDroitePin1, motorArriereDroitePin2);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void init_Moteur() {
  
    motorAvantGauche.motorBrake();
    motorAvantDroite.motorBrake();
    
    motorArriereGauche.motorBrake();
    motorArriereDroite.motorBrake();
}

void setupWiFi() {
   // Connect to Wi-Fi
  WiFi.softAP(ssid, password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

}

void stopTout() {
  motorAvantGauche.motorBrake();
  motorAvantDroite.motorBrake();
  motorArriereGauche.motorBrake();
  motorArriereDroite.motorBrake();
}

void Ferme_Pince()
{
      Serial.println("Servo_1:");
      Serial.println(pos_pince_ferme_1);
      servo_1.write(pos_pince_ferme_1);
      Serial.println("Servo_2:");
      Serial.println(pos_pince_ferme_2);
      servo_2.write(pos_pince_ferme_2);
}
void Ouvre_Pince()
{
      Serial.println("Servo_1:");
      Serial.println(pos_pince_ouvre_1);
      servo_1.write(pos_pince_ouvre_1);
      Serial.println("Servo_2:");
      Serial.println(pos_pince_ouvre_2);
      servo_2.write(pos_pince_ouvre_2);
}

   

void handleJoystick(String direction, int force) {
  if (direction == "up") {
    Serial.println("force:");
    Serial.println(force);
    motorAvantGauche.motorGo(force);
    motorArriereGauche.motorGo(force);
    motorAvantDroite.motorGo(force);
    motorArriereDroite.motorGo(force);
  } else if (direction == "down") {
    Serial.println("-force:");
    Serial.println(force);
     motorAvantGauche.motorGo(-force);
    motorArriereGauche.motorGo(-force);
    motorAvantDroite.motorGo(-force);
    motorArriereDroite.motorGo(-force);
  } else if (direction == "left") {
    Serial.println("force:");
    Serial.println(force);
    motorAvantGauche.motorGo(force);
    motorArriereGauche.motorGo(force);

    motorAvantDroite.motorGo(-force);
    motorArriereDroite.motorGo(-force);
  } else if (direction == "right") {
    Serial.println("force:");
    Serial.println(force);
    motorAvantGauche.motorGo(-force);
    motorArriereGauche.motorGo(-force);

    motorAvantDroite.motorGo(force);
    motorArriereDroite.motorGo(force);
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
  server.on("/nipplejs.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/nipplejs.min.js", "application/javascript");
  });
server.on("/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/jquery.min.js", "application/javascript");
  });
  
 server.on("/car.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/car.png", "image/png");
  });
server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request) {
  Serial.println("Requête reçue : stop");
  stopTout();
  // Implémentez la logique de contrôle du moteur 3 pour avancer ici
  request->send(200, "text/plain", "stop");
});
server.on("/stop-check", HTTP_GET, [](AsyncWebServerRequest *request) {
  Serial.println("Requête reçue : stop_check");
  stopTout();
  request->send(200, "text/plain", "stop");
});

server.on("/joystick", HTTP_GET, [](AsyncWebServerRequest *request){
    String direction = request->getParam("direction")->value();
    String force = request->getParam("force")->value();
    handleJoystick(direction, force.toInt());
    request->send(200, "text/plain", "Joystick command received");
  });
server.on("/position_servo_1", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("value")) {
      int pos = request->getParam("value")->value().toInt();
      Serial.println("Servo_1:");
      Serial.println(pos);
      servo_1.write(pos);
      request->send(200, "text/plain", "Position updated");
    } else {
      request->send(400, "text/plain", "No position specified");
    }
  });

  server.on("/position_servo_2", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("value")) {
      int pos = request->getParam("value")->value().toInt();
      Serial.println("Servo_2:");
      Serial.println(pos);
      servo_2.write(pos);
      request->send(200, "text/plain", "Position updated");
    } else {
      request->send(400, "text/plain", "No position specified");
    }
  });

  server.on("/Ferme_Pince", HTTP_GET, [](AsyncWebServerRequest *request) {
  Serial.println("Requête reçue : Ferme_Pince");
  Ferme_Pince();
  // Implémentez la logique de contrôle du moteur 3 pour avancer ici
  request->send(200, "text/plain", "Ferme_Pince");
});
server.on("/Ouvre_Pince", HTTP_GET, [](AsyncWebServerRequest *request) {
  Serial.println("Requête reçue : Ouvre_Pince");
  Ouvre_Pince();
  // Implémentez la logique de contrôle du moteur 3 pour avancer ici
  request->send(200, "text/plain", "Ouvre_Pince");
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
  servo_1.attach(servoPin_1, 500, 2400); // Pulse width range in microseconds
  servo_2.attach(servoPin_2, 500, 2400); // Pulse width range in microseconds
  
      Ouvre_Pince();
        delay(2000);
 //     Ferme_pince();
        stopTout();
}
void loop() {
  //server.handleClient();
}
