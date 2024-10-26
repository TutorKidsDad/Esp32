<?php
    include_once('inc.inc');
    
    // Check connection
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }
    date_default_timezone_set("Asia/Karachi");
    $val = $_GET['temp'];
    $humd = $_GET['humidity'];
    
    
    
    $date = date('Y-m-d');
    $time = date('H:i:s');
    $sql = "INSERT INTO temps(temp,humidity,dated,time) VALUES ($val,$humd,'$date','$time');";
    
    if ($conn->query($sql) === TRUE) {
        echo "Saved Successfully!";
    } else {
        echo "Error:" . $sql . "<br>" . $conn->error;
    }
    
    $conn->close();
?>
