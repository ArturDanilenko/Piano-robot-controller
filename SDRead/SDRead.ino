#include <FileIO.h>
#include <Bridge.h>
#include<Console.h>
void setup() {
  // Initialize the Bridge and the Serial
  Bridge.begin();
 // Serial.begin(9600);
  FileSystem.begin();
  Console.begin();
  while (!Console); // wait for Serial port to connect.
  Console.println("Filesystem datalogger\n");
}


void loop() {
  File dataFile = FileSystem.open("/mnt/sda1/arduino/www/html_input_output/uploads/text.txt", FILE_READ);
  byte readbyte;
  if (dataFile) {
    Console.println("success opening text.txt");
    readbyte = dataFile.read();
    readbyte = readbyte - '0';
    while(readbyte != 9){
       if(readbyte != 252 ) {
        Console.println(readbyte);
       }
       readbyte = dataFile.read() - '0';
    }
    dataFile.close();
  }
  else {
    SerialUSB.println("error opening datalog.txt");
  }

  delay(15000);
}
