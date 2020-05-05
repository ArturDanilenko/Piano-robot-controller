$(function() {
	getSensorvalue();
});

//var oldcommand = "";

function getSensorvalue() {
    //This function gets sensor values from Arduino AND sends out a request to turn LED on
	//$('#sensor_content').load('/arduino/temperature/'); //send a request for temperature/humidity data
	//$('#LED_content').load('/arduino/temperature/1');
	var checked_option_radio = $('input[name=LEDCheck]:checked','#LED_Selection').val();
	var slider = document.getElementById("myRange");
	command = '/arduino/temperature/';
	command = command + slider.value.toString();
//	if(oldcommand!=command){
		if (checked_option_radio =='off') //if the user selected for LED to be off, send request to Arduino
		{
			//$("#LED_content_test").html("LED is turned off");
			command = '/arduino/temperature/0';
			//$('#LED_content').load(command);
		}
		
		if (checked_option_radio =='on') //if the user selected for LED to be on, send request to Arduino
		{
			//$("#LED_content").html("LED is turned on");
			//$('#LED_content').load(command);
		}
		//oldcommand = command;
//	}
	setTimeout("getSensorvalue()",1000); //Pole every one seconds.	
}

