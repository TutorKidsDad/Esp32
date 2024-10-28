<!DOCTYPE html>
<html>
<head>
  <title>Sensor Data</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      background-color: #f4f4f4;
      margin: 0;
      padding: 20px;
    }
    table {
      width: 80%;
      margin: 20px auto;
      border-collapse: collapse;
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
      background-color: #fff;
    }
    th, td {
      padding: 12px 15px;
      border: 1px solid #ddd;
    }
    th {
      background-color: #003249;
      color: #fff;
    }
    tr:nth-child(even) {
      background-color: #f2f2f2;
    }
  </style>
</head>
<body>

<?php
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-php/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

$servername = "localhost";

// REPLACE with your Database name
$dbname = "REPLACE_WITH_YOUR_DATABASE_NAME";
// REPLACE with Database user
$username = "REPLACE_WITH_YOUR_USERNAME";
// REPLACE with Database user password
$password = "REPLACE_WITH_YOUR_PASSWORD";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT id, sensor, location, value1, value2, value3, reading_time FROM SensorData ORDER BY id DESC";

echo '<table cellspacing="5" cellpadding="5">
      <tr> 
        <th>ID</th> 
        <th>Sensor</th> 
        <th>Location</th> 
        <th>Value 1</th> 
        <th>Value 2</th>
        <th>Value 3</th> 
        <th>Timestamp</th> 
      </tr>';
 
if ($result = $conn->query($sql)) {
    while ($row = $result->fetch_assoc()) {
        $row_id = htmlspecialchars($row["id"]);
        $row_sensor = htmlspecialchars($row["sensor"]);
        $row_location = htmlspecialchars($row["location"]);
        $row_value1 = htmlspecialchars($row["value1"]);
        $row_value2 = htmlspecialchars($row["value2"]); 
        $row_value3 = htmlspecialchars($row["value3"]); 
        $row_reading_time = htmlspecialchars($row["reading_time"]);

        echo '<tr> 
                <td>' . $row_id . '</td> 
                <td>' . $row_sensor . '</td> 
                <td>' . $row_location . '</td> 
                <td>' . $row_value1 . '</td> 
                <td>' . $row_value2 . '</td>
                <td>' . $row_value3 . '</td> 
                <td>' . $row_reading_time . '</td> 
              </tr>';
    }
    $result->free();
} else {
    echo "<tr><td colspan='7'>No data found</td></tr>";
}

$conn->close();
?> 
</table>
</body>
</html>

