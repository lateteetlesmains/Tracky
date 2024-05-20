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
        document.getElementById(button.id).classList.add('robo-button');
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
        color: '#00bcd4', // Bleu
        size: 150, // Taille du joystick
        multitouch: true
    });

    joystick.on('move', function (evt, data) {
        if (data.direction) {
            var direction = data.direction.angle;
            var force = Math.round(data.force * 255);
            force = Math.min(force, 255
