var map = L.map('map').setView([0, 0], 13);

        L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
            attribution: '&copy; OpenStreetMap contributors'
        }).addTo(map);

        // Load coordinates from the text file
        fetch('home/orangepi/projectweek2024/dag2/gps_data.txt')
            .then(response => response.text())
            .then(data => {
                // Process the coordinates data
                var coordinatesArray = processData(data);

                // Display markers on the map
                displayMarkers(coordinatesArray);
            })
            .catch(error => console.error('Error reading the file:', error));

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
        }