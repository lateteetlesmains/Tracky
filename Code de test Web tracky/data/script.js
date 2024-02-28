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
        // Ajoutez d'autres boutons ici selon le même schéma si nécessaire
    ];

    // Définition de la fonction addClickListener
    function addClickListener(buttonId, endpoint) {
        document.getElementById(buttonId).addEventListener('click', function() {
            sendColors(endpoint);
        });
    }

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
    // Assurez-vous que cette URL correspond à l'endpoint défini dans votre serveur Arduino
    fetch(`/setheadlightscolor?left=${encodeURIComponent(leftColor)}&right=${encodeURIComponent(rightColor)}`)
    .then(response => response.text())
    .then(data => console.log('Réponse du serveur:', data))
    .catch(error => console.error('Erreur:', error));
}

// Note: La fonction sendColors utilisée dans addClickListener semble être destinée à envoyer des requêtes spécifiques.
// Si sendColors est uniquement destinée aux phares, ajustez l'utilisation ou définissez des fonctions spécifiques pour chaque action.
