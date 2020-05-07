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
        <p>The following control panel allows you to control the Arduino Yun board which is the logic driver for a piano playing robot</p>
        <br/>
        <p>Few things to note: <br>
        There is already an initial text file with coordinates uploaded onto the SD card of the the ATHEROS chip
        ,hence if you do not upload your own coordinates, it will still work. <br>
        The buttos for the commands can be pressed at any time, but establishing server-client connection is resouse consuming, 
        so if you spam the buttons, one of the chips WILL CRASH. SO DONT SPAM IT.<br>
        Readfile MUST be executed when YUN is either in RESET state or WAIT state. <br>
        In the best case commands won't do anything, however the system is still being tested so unpredictable results are quite likely <br>
        Decode MUST be executed when YUN is in WAIT state. This is only possible if some file has already been read as you cant decode an empty stack.<br>
        Reading commands are connection heavy, so give them time and absolutely do not spam them.
        Uploading can be done anytime as its handled by the server and requires no connection between Linux and Arduino side
        Speed setter will only work if button is pressed. 
        </p>
        <br/>
        <p>Click 'on' to enable speed reading</p>
        
        <form id="LED_Selection" action="">
            <input type="radio" name="LEDCheck" value="off" id="off" />
            <label for="off">LED Off</label><br>
            <input type="radio" name="LEDCheck" value="on" id="on" />
            <label for="on">LED On</label><br>
        </form>
        
        <form action="uploadscript.php" method="POST" enctype="multipart/form-data">
            <input type="file" name="file">
            <button type = "submit" name="submit">UPLOAD</button>
        </form><br/>
        
        <form id="my-form" action ="">
            <button type="submit" id="readfile">Readfile</button>
            <button type="submit" id="decode">Decode</button>
            <button type="submit" id="play">Play</button>
            <button type="submit" id="read">Read</button>
            <button type="submit" id="reset">Reset</button>
            <button type="submit" id="readstack">Readstack values</button>
        </form>
        
        <br/>
        <div class="slidecontainer">
            <input type="range" min="0" max="8" value="0" class="slider" id="myRange">
        </div>
        <p>Value: <span id="demo"></span></p>
        <br/>
        <div id="command"></div>
        <div id="LED_content"></div>
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
        <script type = "text/javascript" src="js/commands.js"></script>

    </body>

</html>

