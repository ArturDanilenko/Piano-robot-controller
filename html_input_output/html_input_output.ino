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
  server.listenOnLocalhost();
  server.begin();  
}

void loop() { 
  int LED_choice;
  BridgeClient client = server.accept();  
  client.println("client accepted");
  digitalWrite(led, HIGH);
  delay(150);
  client.println("delay 1 passed led should be low");
  digitalWrite(led, LOW);
  delay(150);
  client.println("delay 2 passed led should be low");
  if (client) { 
    String command = client.readStringUntil('/'); //Read in the string up to the frist
    command.trim();        //kill whitespace
    if (command == "temperature") {  //Check to see if the first part of the URL command     
      LED_choice=client.parseInt();
      //client.print("<br>Arduino says LED on.");
      client.println(LED_choice);
      client.println(hits);
      if (LED_choice == 1) { // the URL "/arduino/temperature/1" was sent 
          client.print("<br>Arduino says LED on."); //Turn the LED on by making the 
          digitalWrite(led, HIGH);
      }
      if (LED_choice == 0) { // the URL "/arduino/temperature/0" was sent
        client.print("<br>Arduino says LED off.");
        digitalWrite(led, LOW); //Turn the LED off by making the voltage LOW
      }
    }
    client.println("commands executed");
     // Close connection and free resources.
    client.stop();
    client.flush();//discard any bytes that have been written to client but not 
    //yet read.
    hits++; //increment the "hits" counter by 1.
  }    
  client.println("if statement passed");
} 
