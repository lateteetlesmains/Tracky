#include <FS_MX1508.h>

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

const int motorSpeed = 150;  // Vitesse (0 - 255)

void stop_tout() {
  motorAvantGauche.motorBrake();
  motorAvantDroite.motorBrake();
  motorArriereGauche.motorBrake();
  motorArriereDroite.motorBrake();
}

void setup() {
  Serial.begin(9600);
  Serial.println("Commande des moteurs via la liaison série:");
  Serial.println("Utilisez la syntaxe : <numéro><direction>");
  Serial.println("Mapping :");
  Serial.println("  1 -> Avant Gauche");
  Serial.println("  2 -> Avant Droite");
  Serial.println("  3 -> Arrière Droite");
  Serial.println("  4 -> Arrière Gauche");
  Serial.println("Exemples :");
  Serial.println("  1+ -> Avant Gauche Forward");
  Serial.println("  1- -> Avant Gauche Backward");
  Serial.println("-----------------------------------------------------");
  stop_tout();
 
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Supprime les espaces inutiles

    if (command.length() < 2) {
      Serial.println("Commande invalide. Format attendu : <numéro><direction> (ex: 1+)");
      return;
    }

    char motorNum = command.charAt(0);
    char motorDir = command.charAt(1);

    switch (motorNum) {
      case '1':  // Avant Gauche (câblage normal)
        if (motorDir == '0') {
          stop_tout();
        } else if (motorDir == '+') {
          // Pour un moteur normal, avancer = valeur négative
          motorAvantGauche.motorGo(motorSpeed);
          Serial.println("Avant Gauche : Forward");
        } else if (motorDir == '-') {
          motorAvantGauche.motorGo(-motorSpeed);
          Serial.println("Avant Gauche : Backward");
        } else {
          Serial.println("Direction invalide pour le moteur 1.");
        }
        break;

      case '2':  // Avant Droite (câblage normal)
        if (motorDir == '0') {
          stop_tout();
        } else if (motorDir == '+') {
          motorAvantDroite.motorGo(motorSpeed);
          Serial.println("Avant Droite : Forward");
        } else if (motorDir == '-') {
          motorAvantDroite.motorGo(-motorSpeed);
          Serial.println("Avant Droite : Backward");
        } else {
          Serial.println("Direction invalide pour le moteur 2.");
        }
        break;

      case '3':  // Arrière Droite (inversé)
        if (motorDir == '0') {
          stop_tout();
        } else if (motorDir == '+') {
          motorArriereDroite.motorGo(motorSpeed);
          Serial.println("Arrière Droite : Forward (inversé)");
        } else if (motorDir == '-') {
          motorArriereDroite.motorGo(-motorSpeed);
          Serial.println("Arrière Droite : Backward (inversé)");
        } else {
          Serial.println("Direction invalide pour le moteur 3.");
        }
        break;

      case '4':  // Arrière Gauche (inversé)
        if (motorDir == '0') {
          stop_tout();
        } else if (motorDir == '+') {
          motorArriereGauche.motorGo(motorSpeed);
          Serial.println("Arrière Gauche : Forward (inversé)");
        } else if (motorDir == '-') {
          motorArriereGauche.motorGo(-motorSpeed);
          Serial.println("Arrière Gauche : Backward (inversé)");
        } else {
          Serial.println("Direction invalide pour le moteur 4.");
        }
        break;

      default:
        Serial.println("Numéro de moteur invalide. Choisissez entre 1 et 4.");
        break;
    }
  }
}
