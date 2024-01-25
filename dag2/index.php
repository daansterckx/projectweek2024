<?php
$data = file_get_contents('gps_data.txt');
list($latitude, $longitude) = explode(',', $data);
?>
<p>Latitude: <?= $latitude ?></p>
<p>Longitude: <?= $longitude ?></p>