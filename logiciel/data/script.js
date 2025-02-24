var canDetectMove = true;
var force_old = 0;
var direction_old = "";

document.addEventListener('DOMContentLoaded', function() {
    // Liste des boutons et leurs actions associées
    const buttons = [
        { id: 'changeColor', endpoint: 'changecolor' },
        { id: 'testBuzzer', endpoint: 'testbuzzer' },
        { id: 'En_Avant', endpoint: 'forward' },
        { id: 'Gauche', endpoint: 'left' },
        { id: 'Droite', endpoint: 'right' },
        { id: 'En_Arriere', endpoint: 'backward' },
        { id: 'Stop', endpoint: 'stop' },
        { id: 'Fermeture_pince', endpoint: 'Ferme_Pince' },
        { id: 'Ouverture_pince', endpoint: 'Ouvre_Pince' }
    ];

    buttons.forEach(function(button) {
        addClickListener(button.id, button.endpoint);
    });

    // Vérification si l'élément existe avant d'ajouter un event listener
    var setHeadlightsColorButton = document.getElementById('setHeadlightsColor');
    if (setHeadlightsColorButton) {
        setHeadlightsColorButton.addEventListener('click', function() {
            var leftColor = document.getElementById('leftHeadlightColor').value;
            var rightColor = document.getElementById('rightHeadlightColor').value;
            sendColors(leftColor, rightColor);
        });
    }

    // Configuration du joystick
    var joystick = nipplejs.create({
        zone: document.getElementById('joystick-zone'),
        mode: 'static',
        position: { left: '50%', top: '50%' },
        color: 'red'
    });

    joystick.on('move', function (evt, data) {
        if (!canDetectMove) return;

        if (data.direction) {
            var direction = data.direction.angle;
            var force = Math.round(data.force * 255);
            force = Math.min(force, 255);

            // Éviter les envois inutiles (même direction & force que la dernière)
            if (force !== force_old || direction !== direction_old) {
                if (force > 90) {
                    sendJoystickCommand(direction, force);
                    force_old = force;
                    direction_old = direction;
                }
            }
        }
    });

    joystick.on('end', function () {
        console.log('Joystick relâché');
        sendStopCommandUntilAcknowledged();
        canDetectMove = false;
        force_old = 0;
        direction_old = "";
        setTimeout(() => canDetectMove = true, 500);
    });

    // Synchronisation des sliders
    var slider_1 = document.getElementById("servoSlider_1");
    var slider_2 = document.getElementById("servoSlider_2");

    if (slider_1 && slider_2) {
        slider_1.oninput = function() {
            slider_1.value = this.value;
            slider_2.value = this.value;
            document.getElementById("servoPos1").innerHTML = this.value;
            document.getElementById("servoPos2").innerHTML = this.value;
        };

        slider_2.oninput = function() {
            slider_1.value = this.value;
            slider_2.value = this.value;
            document.getElementById("servoPos1").innerHTML = this.value;
            document.getElementById("servoPos2").innerHTML = this.value;
        };

        slider_1.addEventListener("change", function() { servo_1(this.value); });
        slider_2.addEventListener("change", function() { servo_2(this.value); });
    }
});

// Configuration AJAX pour éviter les timeouts longs
$.ajaxSetup({ timeout: 1000 });

function servo_1(pos) {
    $.get("/position_servo_1?value=" + pos);
}
function servo_2(pos) {
    $.get("/position_servo_2?value=" + pos);
}

function addClickListener(buttonId, endpoint) {
    var button = document.getElementById(buttonId);
    if (button) {
        button.addEventListener('click', function() {
            sendCommand(endpoint);
        });
    }
}

let lastCommand = "";
function sendCommand(endpoint) {
    if (lastCommand !== endpoint) {
        lastCommand = endpoint;
        fetch(`/${endpoint}`)
        .then(response => response.text())
        .then(data => console.log('Réponse du serveur:', data))
        .catch(error => console.error('Erreur:', error));
    }
}

function sendColors(leftColor, rightColor) {
    fetch(`/setheadlightscolor?left=${encodeURIComponent(leftColor)}&right=${encodeURIComponent(rightColor)}`)
    .then(response => response.text())
    .then(data => console.log('Réponse du serveur:', data))
    .catch(error => console.error('Erreur:', error));
}

function sendJoystickCommand(direction, force) {
    fetch(`/joystick?direction=${encodeURIComponent(direction)}&force=${encodeURIComponent(force)}`)
    .then(response => response.text())
    .then(data => console.log('Réponse du serveur:', data))
    .catch(error => console.error('Erreur:', error));
}

function sendStopCommandUntilAcknowledged() {
    let intervalId = setInterval(() => {
        sendCommand('stop');
        sendJoystickCommand('stop', 0);
    }, 100);

    function checkServerAcknowledgement() {
        fetch('/stop-check')
        .then(response => response.text())
        .then(data => {
            if (data === 'stop') {
                clearInterval(intervalId);
                console.log('Commande stop confirmée par le serveur');
            } else {
                setTimeout(checkServerAcknowledgement, 200);
            }
        })
        .catch(error => console.error('Erreur:', error));
    }

    checkServerAcknowledgement();
}
