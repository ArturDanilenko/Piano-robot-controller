<?php
// We need to use sessions, so you should always start sessions using the below code.
session_start();
// If the user is not logged in redirect to the login page...
if (!isset($_SESSION['loggedin'])) {
	header('Location: index.html');
	exit;
}
?>

<!DOCTYPE html>
<html>
	<head>
		<link href="stylesheet.css" rel="stylesheet" type="text/css">
	</head>
<body>
    <p>Click the button to turn on and off the LED.</p>
	
	<form id="LED_Selection" action="">
        <input type="radio" name="LEDCheck" value="off" id="off" />
        <label for="off">LED Off</label><br>
        <input type="radio" name="LEDCheck" value="on" id="on" />
        <label for="on">LED On</label><br>
    </form>
    
	<form action="uploadscript.php" method="POST" enctype="multipart/form-data">
		<input type="file" name="file">
		<button type = "submit" name="submit">UPLOAD</button>
    </form>
    
	<div class="slidecontainer">
		<input type="range" min="0" max="8" value="0" class="slider" id="myRange">
    </div>
	<p>Value: <span id="demo"></span></p>
    <br />
    
    <div id="sensor_content">Loading data from Arduino...</div>
    <div id="LED_content"></div>
    <div id="LED_content_test"></div>
    <script type="text/javascript" src="js/zepto.min.js"></script>
    <script type="text/javascript" src="js/jsHere.js"></script>	
    <script>
        var slider = document.getElementById("myRange");
        var output = document.getElementById("demo");
        output.innerHTML = slider.value;
        
        slider.oninput = function() {
        output.innerHTML = this.value;
        }
    </script>
         
	</body>

</html>

