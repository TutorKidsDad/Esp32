<?php
 include_once('inc.inc');
 
 // Check connection
 if ($conn->connect_error) {
 die("Connection failed: " . $conn->connect_error);
 }
 date_default_timezone_set("Asia/Karachi");
 $device_name = $_GET['device_name'];
 
 $sql = "SELECT device_status FROM devices_status WHERE device_name='$device_name'";
 $result = $conn->query($sql);
 if ($result->num_rows > 0) {
 while($row = $result->fetch_assoc())
 {
 echo $row['device_status'];
 }
 } else {
 echo "Error:" . $sql . "<br>" . $conn->error;
 }
 
 $conn->close();
?>
