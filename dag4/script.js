var map = L.map('map').setView([0, 0], 20);
var markersLayer = L.layerGroup().addTo(map); // Create a layer group for markers
var circle; // Variable to hold the circle representing the alert zone
var drawnItems = new L.FeatureGroup().addTo(map); // Create a layer group for drawn items

L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: '&copy; OpenStreetMap contributors'
}).addTo(map);

// Initialize the draw control
var drawControl = new L.Control.Draw({
    edit: {
        featureGroup: drawnItems
    },
    draw: {
        polygon: true,
        circle: false,
        rectangle: false,
        marker: false,
        polyline: false
    }
});
map.addControl(drawControl);

// Handle draw created event
map.on(L.Draw.Event.CREATED, function (event) {
    var layer = event.layer;
    drawnItems.addLayer(layer);
});

// Handle draw edited event
map.on(L.Draw.Event.EDITED, function (event) {
    var layers = event.layers;
    // Do something with the edited layers
});

function loadfile() {
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

var alertedMarkers = new Set();

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
        var marker = L.marker(coord).addTo(markersLayer);

        // Check if the marker is outside the drawn area
        if (!isMarkerInsideDrawableArea(marker) && !alertedMarkers.has(marker)) {
            alert('Marker is outside the drawable zone!');
            alertedMarkers.add(marker); // Add the marker to the set to prevent repeated alerts
        }
    });

    // Check if the marker position crosses the border of the circle or the drawn area
    coordinatesArray.forEach(coord => {
        if ((circle && map.distance(coord, circle.getLatLng()) > circle.getRadius()) ||
            (!isMarkerInsideDrawableArea(marker))) {
            // If marker crosses the border, trigger an alert
            alert('Kind is in een verbodenzone!');
        }
    });

    // Set the map view to the last coordinates
    var lastCoord = coordinatesArray[coordinatesArray.length - 1];
    map.setView(lastCoord, 20);

    // Update the coordinates below the map
    document.getElementById('coordinates').textContent = 'Latitude: ' + lastCoord[0] + ', Longitude: ' + lastCoord[1];
}

function isMarkerInsideDrawableArea(marker) {
    // Get the marker coordinates
    var markerCoord = marker.getLatLng();

    // Check if the marker is inside the drawn area
    var isInside = drawnItems.getBounds().contains(markerCoord);
    
    return isInside;
}

function setRadius() {
    var radius = parseFloat(document.getElementById('radiusInput').value);
    if (!isNaN(radius)) {
        // Define the center of the alert zone
        var center = map.getCenter();

        // Remove existing circle if any
        if (circle) {
            map.removeLayer(circle);
        }

        // Add a new circle representing the alert zone
        circle = L.circle(center, {
            color: 'red',
            fillColor: '#f03',
            fillOpacity: 0.2,
            radius: radius
        }).addTo(map);
    } else {
        alert('Please enter a valid radius.');
    }
}

function removeRadius() {
    // Remove existing circle if any
    if (circle) {
        map.removeLayer(circle);
        circle = null; // Reset de referentie naar de cirkel
    }

    // Remove existing drawn items
    drawnItems.clearLayers();
}

function checkAlarmState() {
    fetch('button_state.txt')
        .then(response => response.text())
        .then(data => {
            if (data.trim() === '0') {
                alert('Kind is in nood!');
            }
        })
        .catch(error => console.error('Error reading the file:', error));
}

setInterval(loadfile, 2000);
setInterval(checkAlarmState, 500);

function login() {
    // Replace this with your actual authentication logic
    var username = document.getElementById('username').value;
    var password = document.getElementById('password').value;

    // Dummy authentication (for demonstration purposes)
    if (username === 'demo' && password === 'demo') {
        // On successful login, hide the login form
        document.getElementById('loginForm').style.display = 'none';
        // Make the map visible
        document.getElementById('mapLogin').style.display = 'block';
        map.invalidateSize();
    } else {
        alert('Invalid username or password. Please try again.');
    }
}

document.getElementById('buzzButton').addEventListener('click', function() {
    // Stuur een HTTP-verzoek naar de ESP32 wanneer de knop wordt ingedrukt
    fetch('http://192.168.137.17/buzzer', { method: 'POST' })
        .then(response => {
            if (response.ok) {
                alert('Kinderen worden opgeroepen!');
            } else {
                alert('Er is een fout opgetreden met het oproepen!');
            }
        })
        .catch(error => {
            console.error('Error:', error);
            alert('Er is een onbekende fout opgetreden!');
        });
});
