var map = L.map('map').setView([0, 0], 20);
var markersLayer = L.layerGroup().addTo(map); // Create a layer group for markers

L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: '&copy; OpenStreetMap contributors'
}).addTo(map);

function loadfile(){
    // Load coordinates from the text file
    fetch('gps_data.txt')
        .then(response => response.text())
        .then(data => {
            // Clear the existing markers
            markersLayer.clearLayers();

            // Process the coordinates data
            processAndDisplayMarkers(data);

        })
        .catch(error => console.error('Error reading the file:', error));
}

function processAndDisplayMarkers(data) {
    // Split the data into lines
    var lines = data.trim().split('\n');

    // Process each line and extract coordinates
    var coordinatesArray = lines.map(line => {
        var [lat, lng] = line.split(',').map(coord => parseFloat(coord.trim()));
        return [lat, lng];
    });

    // Add markers to the map
    coordinatesArray.forEach(coord => {
        L.marker(coord).addTo(markersLayer);

    });

    // Set the map view to the last coordinates
    var lastCoord = coordinatesArray[coordinatesArray.length - 1];
    map.setView(lastCoord, 20);

    // Update the coordinates below the map
    document.getElementById('coordinates').textContent = 'Latitude: ' + lastCoord[0] + ', Longitude: ' + lastCoord[1];
}
setInterval(loadfile, 5000);
        
function login() {
    // Replace this with your actual authentication logic
    var username = document.getElementById('username').value;
    var password = document.getElementById('password').value;

    // Dummy authentication (for demonstration purposes)
    if (username === 'demo' && password === 'demo') {
        // On successful login, hide the login form
        document.getElementById('loginForm').style.display = 'none';
    } else {
        alert('Invalid username or password. Please try again.');
    }
}

// Function to toggle the buzzer state
function toggleBuzzer() {
    // Send a request to the ESP32 to toggle the buzzer
    fetch('toggle_buzzer', { method: 'POST' })
        .then(response => {
            if (response.ok) {
                console.log('Buzzer toggled successfully');
            } else {
                console.error('Failed to toggle buzzer');
            }
        })
        .catch(error => console.error('Error:', error));
}

// Add an event listener for the button click
document.getElementById('buzzButton').addEventListener('click', toggleBuzzer);

document.getElementById('controlButton').addEventListener('click', function() {
    // Stuur een HTTP-verzoek naar de ESP32 wanneer de knop wordt ingedrukt
    fetch('http://esp32_ip_address/buzzer', { method: 'POST' })
        .then(response => {
            if (response.ok) {
                alert('Buzzer activated successfully!');
            } else {
                alert('Failed to activate the buzzer.');
            }
        })
        .catch(error => {
            console.error('Error:', error);
            alert('Failed to activate the buzzer.');
        });
});
