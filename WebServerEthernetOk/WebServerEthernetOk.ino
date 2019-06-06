/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 */

#include <SPI.h>
#include <Ethernet.h>

// for DHT22 sensor
#include "DHT.h"
DHT dht;
#define DHTPIN 2
//#define DHTTYPE DHT22
#define LED1 3

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 17);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(8081);
String readString;

void setup() {
  pinMode(LED1, OUTPUT); //pin selected to control LED 
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

   Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F) \t Analog Inputs");
  //dht.begin();
   dht.setup(DHTPIN); // data pin 2
 // Open serial communications and wait for port to open:

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        //read char by char HTTP request
         if (readString.length() < 100) {
        
         //store characters to string
         readString += c;
         //Serial.print(c); 

         }        
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");

          delay(dht.getMinimumSamplingPeriod());
          // get data from DHT22 sensor
          //float h = dht.readHumidity();
          //float t = dht.readTemperature();
           float h = dht.getHumidity();
           float t = dht.getTemperature();

          
          Serial.print(dht.getStatusString());
          Serial.print("\t h=");
          Serial.print(h, 1);
          Serial.print("\t\t t=");
          Serial.print(t, 1);
          Serial.print("cel\t\t t=");
          Serial.println(dht.toFahrenheit(t), 1);
          Serial.print("F");

          // from here we can enter our own HTML code to create the web page
          client.print("<head><title>Home Automation</title></head>");
          client.println("<body bgcolor='aqua'><h1>Home Automation System</h1>"); 
          
          client.println("<hr />");
          client.println("<h2>Temperature and Humidity </h2>");  
          client.println("<p>Temperature - <b>");
          client.print(t);
          client.print(" </b>degrees Celsius <br/>");
          client.print("Humidity -  <b>");
          client.print(h);
          client.print(" </b>percent</p>");
          
          client.println("<hr />");
          client.println("<h2>Analog Values </h2>"); 
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is   <b>");
            client.print(sensorReading);
            client.println(" </b><br />");
          }
          client.println("<hr />");
          client.println("<h2>Digital Values </h2>"); 
          // output the value of digital pins 0 to 9 onto the web page
          for (int digitalChannel = 0; digitalChannel < 10; digitalChannel++) 
          {
            boolean pinStatus = digitalRead(digitalChannel);
            client.print("digital pin ");
            client.print(digitalChannel);
            client.print(" is   <b>");
            client.print(pinStatus);
            client.println("</b><br />");       
          }  

           client.println("<hr />");
           client.println("<br />");
          
           client.println("<a href=\"/?lighton\"\">Turn On Light</a>");
           client.println("<br /><br />");
           client.println("<a href=\"/?lightoff\"\">Turn Off Light</a><br />");

           client.println("<hr />");
           client.println("<a href=\"\">Refresh</a><br />"); 


          client.println("</BODY>"); 
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");

        ///////////////////// control arduino pin
     if(readString.indexOf("?lighton") >0)//checks for on
     {
     digitalWrite(LED1, HIGH); // set pin 6 high
     Serial.println("LED On");
     }
     else{
     if(readString.indexOf("?lightoff") >0)//checks for off
     {
     digitalWrite(LED1, LOW); // set pin 6 low
     Serial.println("LED Off");
     }
     }
     readString=""; //clearing string for next read
  }
}

