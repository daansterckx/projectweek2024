<!DOCTYPE html>
<html>
<body>
<p id="latitude">Latitude: </p>
<p id="longitude">Longitude: </p>

<script>
function fetchData() {
    fetch('gps_data.txt')
        .then(response => response.text())
        .then(data => {
            const [latitude, longitude] = data.split(',');
            document.getElementById('latitude').textContent = `Latitude: ${latitude}`;
            document.getElementById('longitude').textContent = `Longitude: ${longitude}`;
        });
}

// Fetch data every second
setInterval(fetchData, 1000);
</script>

</body>
</html>