var map = L.map('map').setView([0, 0], 13);

        L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
            attribution: '&copy; OpenStreetMap contributors'
        }).addTo(map);

        // Load coordinates from the text file
        function updateMap() {
            fetch('gps_data.txt')
                .then(response => response.text())
                .then(data => {
                    const [latitude, longitude] = data.split(',');
                    const newPosition = [latitude, longitude];
                    map.setView(newPosition, 13);
                    marker.setLatLng(newPosition);
                });
        }

        function processData(data) {
            // Split the data into lines
            var lines = data.trim().split('\n');

            // Process each line and extract coordinates
            var coordinatesArray = lines.map(line => {
                var [lat, lng] = line.split(',').map(coord => parseFloat(coord.trim()));
                return [lat, lng];
            });

            return coordinatesArray;
        }

        function displayMarkers(coordinatesArray) {
            // Add markers to the map
            coordinatesArray.forEach(coord => {
                L.marker(coord).addTo(map);
            });

            // Set the map view to the last coordinates
            var lastCoord = coordinatesArray[coordinatesArray.length - 1];
            map.setView(lastCoord, 13);

            // Update the coordinates below the map
            updateCoordinates(lastCoord[0], lastCoord[1]);
        }

        function updateCoordinates(lat, lng) {
            document.getElementById('coordinates').textContent = 'Latitude: ' + lat + ', Longitude: ' + lng;
        }
        setInterval(updateMap, 2000);

        function login() {
            // Replace this with your actual authentication logic
            var username = document.getElementById('username').value;
            var password = document.getElementById('password').value;

            // Dummy authentication (for demonstration purposes)
            if (username === 'demo' && password === 'demo') {
                // On successful login, hide the login form
                document.getElementById('loginForm').style.display = 'none';
                // Start updating the map periodically
                updateMapPeriodically();
            } else {
                alert('Invalid username or password. Please try again.');
            }
        }