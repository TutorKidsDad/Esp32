<!DOCTYPE html>
<html lang="en">
    <head>
		<meta charset="UTF-8" />
        <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1"> 
		<meta name="viewport" content="width=device-width, initial-scale=1.0"> 
        <title>Button Switches with Checkboxes and CSS3 Fanciness</title>
 
        <link rel="stylesheet" type="text/css" href="css/style.css" />
<style type="text/css">
body{
background: #614385;  /* fallback for old browsers */
background: -webkit-linear-gradient(to right, #516395, #614385);  /* Chrome 10-25, Safari 5.1-6 */
background: linear-gradient(to right, #516395, #614385); /* W3C, IE 10+/ Edge, Firefox 16+, Chrome 26+, Opera 12+, Safari 7+ */

}
</style>
    </head>
    <body style="set_background">
    
    <?php
    include_once('inc.inc');
    
    if(isset($_GET['device_status']))
    {
       $status  =   $_GET['device_status'];
       if($status==1)
       {
        $update_status  =   0;
       }
       elseif($status==0)
       {
        $update_status  =   1;
       }
       $query   =   "UPDATE devices_status SET device_status='$update_status' WHERE device_name='home_sensor'";
       $conn->query($query);
    }

    
    $get_status_sql            =   "SELECT device_status FROM devices_status WHERE device_name='home_sensor'";
    $result         =   $conn->query($get_status_sql);
    
    if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc())
    {
        $current_status = $row['device_status'];
        
        if($current_status == 1)
        {
            $checked    =   'checked';
        }
        else
        {
            $checked    =   '';
        }}}
    
    ?>
    
        <div class="container">
		
			<section class="main">
				<h1 style="text-align: center; padding: 30px; font-size: 35px; font-family: fantasy;">Controll Sensor Data with PHP and MySQL</h1>
				<div class="switch demo3">
					<input onClick="location.href='<?php echo $_SERVER['PHP_SELF']; ?>?device_status=<?php echo $current_status; ?>'" type="checkbox" value="<?php echo $current_status;?>" <?php echo $checked;?> />
                    <label><i></i></label>
                </div>
                <h2 style="text-align: center;">Created by ICE786 Technologies Team </h2>
				
			</section>
			
        </div>

    </body>
</html>
