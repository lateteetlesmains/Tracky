var canDetectMove = true;
var force_old=0;

document.addEventListener('DOMContentLoaded', function() {
    // Configuration des boutons existants
    const buttons = [
        { id: 'changeColor', endpoint: 'changecolor' },
        { id: 'testBuzzer', endpoint: 'testbuzzer' },
        { id: 'En_Avant', endpoint: 'forward' },
        { id: 'Gauche', endpoint: 'left' },
        { id: 'Droite', endpoint: 'right' },
        { id: 'En_Arriere', endpoint: 'backward' },
        { id: 'Stop', endpoint: 'stop' }
    ];

    buttons.forEach(function(button) {
        addClickListener(button.id, button.endpoint);
    });

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
    joystick.on('added', function (evt, nipple) {
        // Modifier la taille des éléments après la création du joystick
        var back = nipple.ui[0].el.querySelector('.back');
        var front = nipple.ui[0].el.querySelector('.front');

        if (back && front) {
            back.style.width = '400px';
            back.style.height = '400px';
            back.style.marginLeft = '-100px';
            back.style.marginTop = '-100px';

            front.style.width = '100px';
            front.style.height = '100px';
            front.style.marginLeft = '-50px';
            front.style.marginTop = '-50px';
        }
    });
    joystick.on('move', function (evt, data) {
        if (!canDetectMove) {
            return; // Ignorer l'événement move si la détection est désactivée
        }
        if (data.direction) {
            var direction = data.direction.angle;
            // Convertir la force (qui est entre 0 et 1) en une valeur entière entre 0 et 255
            var force = Math.round(data.force * 255);
            
            // Limiter la force à 255 maximum
            force = Math.min(force, 255);
            console.log(force);
            if (force==force_old)
                {
                    return;
                }
            if (force > 90) {
                sendJoystickCommand(direction, force);
                force_old=force;
            }
            
        }
    });

    joystick.on('end', function () {
        console.log('Joystick relâché');
        sendStopCommandUntilAcknowledged();
        canDetectMove = false;
        force_old=0;
    setTimeout(function() {
        canDetectMove = true;
    }, 500); // Délai de 500 millisecondes
    });
});
var slider_1 = document.getElementById("servoSlider_1");
var slider_2 = document.getElementById("servoSlider_2");

var servoP1 = document.getElementById("servoPos1");
var servoP2 = document.getElementById("servoPos2");

servoP1.innerHTML = slider_1.value;
servoP2.innerHTML = slider_2.value;

slider_1.oninput = function() {
    slider_1.value = this.value;
    servoP1.innerHTML = this.value;
    slider_2.value = this.value;
    servoP2.innerHTML = this.value;
            
}

 slider_2.oninput = function() {
    slider_1.value = this.value;
    servoP1.innerHTML = this.value;
    slider_2.value = this.value;
    servoP2.innerHTML = this.value;
}

$.ajaxSetup({timeout: 1000});


function servo_1(pos) {
$.get("/position_servo_1?value=" + pos + "&", {Connection: close});
}
function servo_2(pos) {
$.get("/position_servo_2?value=" + pos + "&", {Connection: close});
}
slider_1.addEventListener("change", function() {
    servo_1(this.value);
});
slider_2.addEventListener("change", function() {
    servo_2(this.value);
});
function addClickListener(buttonId, endpoint) {
    var button = document.getElementById(buttonId);
    if (button) {
        button.addEventListener('click', function() {
            sendCommand(endpoint);
        });
    }
}

function sendCommand(endpoint) {
    fetch(`/${endpoint}`)
    .then(response => response.text())
    .then(data => console.log('Reponse du serveur:', data))
    .catch(error => console.error('Erreur:', error));
}

function sendColors(leftColor, rightColor) {
    fetch(`/setheadlightscolor?left=${encodeURIComponent(leftColor)}&right=${encodeURIComponent(rightColor)}`)
    .then(response => response.text())
    .then(data => console.log('Reponse du serveur:', data))
    .catch(error => console.error('Erreur:', error));
}

function sendJoystickCommand(direction, force) {
    fetch(`/joystick?direction=${encodeURIComponent(direction)}&force=${encodeURIComponent(force)}`)
    .then(response => response.text())
    .then(data => console.log('Reponse du serveur:', data))
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
