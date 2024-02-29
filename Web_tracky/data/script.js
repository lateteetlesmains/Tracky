
function addClickListener(buttonId, endpoint) {
    document.getElementById(buttonId).addEventListener('click', function() {
        sendCommand(endpoint);
    });
}
document.addEventListener('DOMContentLoaded', function() {
    const buttons = [
        { id: 'changeColor', endpoint: 'changecolor' },
        { id: 'testBuzzer', endpoint: 'testbuzzer' },
        { id: 'En_Avant', endpoint: 'forward' },
        { id: 'Gauche', endpoint: 'left' },
        { id: 'Droite', endpoint: 'right' },
        { id: 'En_Arriere', endpoint: 'backward' },
        { id: 'Stop', endpoint: 'stop' }
        // Ajoutez d'autres boutons ici selon le même schéma si nécessaire
    ];

    buttons.forEach(function(button) {
        addClickListener(button.id, button.endpoint);
    });

    // Configuration spéciale pour le bouton des phares, si nécessaire
});
// Note: La fonction sendColors utilisée dans addClickListener semble être destinée à envoyer des requêtes spécifiques.
// Si sendColors est uniquement destinée aux phares, ajustez l'utilisation ou définissez des fonctions spécifiques pour chaque action.
function sendCommand(endpoint) {
    // Assurez-vous que cette URL correspond à l'endpoint défini dans votre serveur Arduino
    fetch(`/${endpoint}`)
    .then(response => response.text())
    .then(data => console.log('Reponse du serveur:', data))
    .catch(error => console.error('Erreur:', error));
}
  
  // Nouvel écouteur d'événement pour setHeadlightsColor
    document.getElementById('setHeadlightsColor').addEventListener('click', function() {
        var leftColor = document.getElementById('leftHeadlightColor').value;
        var rightColor = document.getElementById('rightHeadlightColor').value;
        sendColors(leftColor, rightColor);
    });


// Fonction pour envoyer les couleurs des phares au serveur
function sendColors(leftColor, rightColor) {
    // Assurez-vous que cette URL correspond à l'endpoint défini dans votre serveur Arduino
    fetch(`/setheadlightscolor?left=${encodeURIComponent(leftColor)}&right=${encodeURIComponent(rightColor)}`)
    .then(response => response.text())
    .then(data => console.log('Reponse du serveur:', data))
    .catch(error => console.error('Erreur:', error));
}




