#include <WiFi.h>
#include <ESP32Servo.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

Servo myservo;  // create servo object to control a servo

// GPIO the servo is attached to
int servoPin_1 = 23;
int servoPin_2 = 17;

// Network credentials
const char* ssid     = "servo";
const char* password = "";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  servoPin_1.attach(servoPin_1, 500, 2400); // Pulse width range in microseconds
  servoPin_2.attach(servoPin_2, 500, 2400); // Pulse width range in microseconds

  // Start SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.softAP(ssid, password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

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

  server.on("/position_servo_1", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("value")) {
      int pos = request->getParam("value")->value().toInt();
      Serial.print(pos);
      servoPin_1.write(pos);
      request->send(200, "text/plain", "Position updated");
    } else {
      request->send(400, "text/plain", "No position specified");
    }
  });

  server.on("/position_servo_2", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("value")) {
      int pos = request->getParam("value")->value().toInt();
      Serial.print(pos);
      servoPin_2.write(pos);
      request->send(200, "text/plain", "Position updated");
    } else {
      request->send(400, "text/plain", "No position specified");
    }
  });


  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404, "text/plain", "Not found");
  });

  // Start server
  server.begin();
}

void loop() {
  // Empty loop as AsyncWebServer handles clients asynchronously
}
