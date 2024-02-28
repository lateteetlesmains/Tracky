// Ajout de l'écouteur d'événement pour setHeadlightsColor
document.addEventListener('DOMContentLoaded', function() {
    // Configuration initiale pour les boutons de commande
    const buttons = [
        { id: 'changeColor', endpoint: 'changecolor' },
        { id: 'testBuzzer', endpoint: 'testbuzzer' },
        { id: 'motor1-forward', endpoint: 'motor1-forward' },
        { id: 'motor1-backward', endpoint: 'motor1-backward' },
        { id: 'motor2-forward', endpoint: 'motor2-forward' },
        { id: 'motor2-backward', endpoint: 'motor2-backward' },
        { id: 'motor3-forward', endpoint: 'motor3-forward' },
        { id: 'motor3-backward', endpoint: 'motor3-backward' },
        { id: 'motor4-forward', endpoint: 'motor4-forward' },
        { id: 'motor4-backward', endpoint: 'motor4-backward' },
        // Ajouter d'autres boutons ici selon le même schéma
    ];

    // Ajoute des écouteurs pour tous les boutons configurés
    buttons.forEach(function(button) {
        addClickListener(button.id, button.endpoint);
    });

    // Nouvel écouteur d'événement pour setHeadlightsColor
    document.getElementById('setHeadlightsColor').addEventListener('click', function() {
        var leftColor = document.getElementById('leftHeadlightColor').value;
        var rightColor = document.getElementById('rightHeadlightColor').value;
        sendColors(leftColor, rightColor);
    });
});

// Fonction pour envoyer les couleurs des phares au serveur
function sendColors(leftColor, rightColor) {
    fetch('/setheadlightscolor?left=' + leftColor + '&right=' + rightColor)
    .then(response => response.text())
    .then(data => console.log(data))
    .catch(error => console.error('Erreur:', error));
}
