$(function() {
	getSensorvalue();
});

function getSensorvalue() {
    //This function gets sensor values from Arduino AND sends out a request to turn LED on
	//$('#sensor_content').load('/arduino/temperature/'); //send a request for temperature/humidity data
	//$('#LED_content').load('/arduino/temperature/1');
	var checked_option_radio = $('input[name=LEDCheck]:checked','#LED_Selection').val();
	
	 if (checked_option_radio =='off') //if the user selected for LED to be off, send request to Arduino
	{
		//$("#LED_content_test").html("LED is turned off");
	 	$('#LED_content').load('/arduino/temperature/0');
	 }
	
	  if (checked_option_radio =='on') //if the user selected for LED to be on, send request to Arduino
	{
		//$("#LED_content").html("LED is turned on");
	 	$('#LED_content').load('/arduino/temperature/1');
	 }

	setTimeout("getSensorvalue()",1000); //Pole every one seconds.	
}

