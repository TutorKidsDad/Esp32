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

// Keep this API Key value to be compatible with the ESP32 code provided in the project page. 
// If you change this value, the ESP32 sketch needs to match
$api_key_value = "tPmAT5Ab3j7F9";

$api_key = $sensor = $location = $value1 = $value2 = $value3 = "";

// Check if data is received via HTTP POST method and that the content-type is JSON
if ($_SERVER["REQUEST_METHOD"] == "POST" && strpos($_SERVER["CONTENT_TYPE"], "application/json") !== false) {
    // Read the raw POST data
    $json = file_get_contents('php://input');
    // Decode JSON data into an associative array
    $data = json_decode($json, true);
    
    if ($data) {
        // Get values from the decoded JSON data
        $api_key = test_input($data["api_key"]);
        
        if ($api_key == $api_key_value) {
            $sensor = test_input($data["sensor"]);
            $location = test_input($data["location"]);
            $value1 = test_input($data["value1"]);
            $value2 = test_input($data["value2"]);
            $value3 = test_input($data["value3"]);
            
            // Create connection
            $conn = new mysqli($servername, $username, $password, $dbname);
            // Check connection
            if ($conn->connect_error) {
                die("Connection failed: " . $conn->connect_error);
            } 
            
            $sql = "INSERT INTO SensorData (sensor, location, value1, value2, value3)
            VALUES ('$sensor', '$location', '$value1', '$value2', '$value3')";
            
            if ($conn->query($sql) === TRUE) {
                echo "New record created successfully";
            } 
            else {
                echo "Error: " . $sql . "<br>" . $conn->error;
            }
        
            $conn->close();
        }
        else {
            echo "Wrong API Key provided.";
        }
    }
    else {
        echo "Invalid JSON data.";
    }
}
else {
    echo "No data posted with HTTP POST or incorrect content-type.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
?>

