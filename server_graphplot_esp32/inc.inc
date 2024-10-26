<?php
    $servername = "localhost";
    $username = "onlinetp_espdata";
    $password = "TPS@462016";
    $dbname = "onlinetp_espcontrl";
    

    // Create connection
     $conn	=	mysqli_connect($servername, $username,$password);
    $connection = mysqli_select_db($conn,$dbname);
?>
