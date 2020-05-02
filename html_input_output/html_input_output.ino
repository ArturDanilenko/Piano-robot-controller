//Inlude the appropriate libraries
#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>

//WiFi setup:
// Listen on default port 5555, the webserver on the Yún
// will forward there all the HTTP requests for us.
BridgeServer server;
String startString;
long hits = 0;

//Setup LED pin
int led=13; //hardware connection: Yún digital pin 7 to LED to 220 Ohm resistor to Ground.
int val = 1;
void setup() {
  Bridge.begin();
 // Console.begin();
  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();
   
  // get the time that this sketch started:
   
  pinMode(led,OUTPUT); //initialize the digital pin (which controls the LED) as an output.
 // digitalWrite(led,1);
// while(!Console);
 
}

void loop() { 
  int LED_choice;
 //Console.print("qwq");
  BridgeClient client = server.accept();  
 // Console.println(client);
  delay(50);
//  digitalWrite(led,1);
//  delay(250);
//  digitalWrite(led,0);
 // delay(250);
  if (client) { 
    String command = client.readStringUntil('/'); //Read in the string up to the frist
    command.trim();        //kill whitespace
    if (command == "temperature") {  //Check to see if the first part of the URL command 
      client.print(" degrees C");      
      LED_choice=client.parseInt();
      if (LED_choice == 1) { // the URL "/arduino/temperature/1" was sent 
          client.print("<br>Arduino says LED on."); //Turn the LED on by making the 
          digitalWrite(led, HIGH);
      }
      if (LED_choice == 0) { // the URL "/arduino/temperature/0" was sent
        client.print("<br>Arduino says LED off.");
        digitalWrite(led, LOW); //Turn the LED off by making the voltage LOW
      }
    }
     
     // Close connection and free resources.
    client.stop();
    client.flush();//discard any bytes that have been written to client but not 
    //yet read.
    hits++; //increment the "hits" counter by 1.
  }    
  
} 
