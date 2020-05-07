$(function() {
	getSensorvalue();
});

var oldcommand = "";

function getSensorvalue() {
    //This function sends the speed to the arduino
	var checked_option_radio = $('input[name=LEDCheck]:checked','#LED_Selection').val();
	var slider = document.getElementById("myRange");
	command = '/arduino/setspeed/';
	command = command + slider.value.toString(); //parse the speed from the slider into the command to be sent to YUN
	if(oldcommand!=command){
		if (checked_option_radio =='off') //if the button is in off state, ignore the speed requests
		{
			command = '/arduino/setspeed/0';
			//$('#LED_content').load(command);
		}
		
		if (checked_option_radio =='on') //if the button is on send the chosen speed to YUN
		{
			$('#LED_content').load(command);
		}
		oldcommand = command;
	}
	setTimeout("getSensorvalue()",1000); //Pole every one seconds.	
}

