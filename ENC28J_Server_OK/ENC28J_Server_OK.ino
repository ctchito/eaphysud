/**
 * CS is Pin 10
 * 
 */
#include <UIPEthernet.h> // Used for Ethernet

#define Alarm 4
#define LED1 5
#define LED2 6
#define tmp 4 //defines analog pin 4 as the temperature sensor pin
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
  pinMode(Alarm,OUTPUT); //setting digital pin4 as an output pin for temperature alarm
  pinMode(LED1,OUTPUT);//setting pin5 as an output pin
  pinMode(LED2,OUTPUT); //setting pin6 as an output pin
  pinMode(7,INPUT); //setting pin7 as input to verify if Lamp1 is actually on
  pinMode(8,INPUT); //setting pin8 as input to verify if Lamp2 is actually on
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

   

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  String LED1Status;
  String LED2Status;
  if (client) {
    
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
            client.println("Content-Type:text/html");
            client.println("connection: close");
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            /*
            client.println("<head>");
            client.println("<title>");
            client.println("Internet-Based Control of Systems");
            client.println("</title>");
           
            client.println("<h1 align=\"center\">");
            client.println("<font width=\"10\" size=\"10\">");
            client.println("INTERNET BASED CONTROL OF SYSTEMS");
            client.println("</font></h1>");
          
            client.println("</head>");
            client.println("<hr />");
            client.println("<body bgcolor=\"#B0F9F1\" font size=\"1em\" face=\"Arial\">");
            
            client.println("<FORM METHOD=POST action=0>");
            client.println("<font size=\"2\" face=\"Arial\">");
            client.println("</font>");
            client.println("<h2 align=\"center\">");
            client.println("<strong>");
            client.println("Commands");
            client.println("</strong>");
            
            
            client.println("<p style=\"float:right;\"> <a href=\"/?lamp2on\"> <input type=submit name=1 value=\"Switch ON Lamp2\"> </a>");
            client.println("</p>");
            client.println("<p style=\"float:right;\"> <a href=\"/?lamp2off\"> <input type=submit name=1 value=\"Switch OFF Lamp2\"> </a>");
            client.println("</p>");
             client.println("<p style=\"float:left;\"> <a href=\"/?lamp1on\"> <input type=submit name=1 value=\"Switch ON Lamp1\"> </a>");
            client.println("</p>");
            client.println("<p style=\"float:left;\"> <a href=\"/?lamp1off\"> <input type=submit name=1 value=\"Switch OFF Lamp1\"> </a>");
            client.println("</p></h2>");
            */
            client.println("<p style=\"clear:both;\"></p><br />");
            
            client.println("<hr />");
            client.println("<meta http-equiv=\"refresh\" content=\"3\">");
            client.println("<h3>");
            client.println("<div align=\"center\">");
            client.println("<strong>");
            client.println("Results");
            client.println("</strong>");
            client.println("</div>");

            client.println("<p>");
            client.println("<font  size=\"2\">");     
             client.println("</font>");
                if(readString.indexOf("?lamp1on") >0)//checks for on
                 {
              
                    digitalWrite(LED1, HIGH); // set pin 5 high
                    
                 }
            
               else{
                   if(readString.indexOf("?lamp1off") >0)//checks for off
                    {
                     digitalWrite(LED1, LOW); // set pin 5 low
                     
 
                     }
                  }
                  
            client.println("</p>");

            client.println("<p>");
            client.println("<font  size=\"2\">");
           // client.println("LAMP2:");
            client.println("</font>");
                   if(readString.indexOf("?lamp2on") >0)//checks for on
                   {
                      digitalWrite(LED2, HIGH); // set pin 6 high

                    }
                  else{
                      if(readString.indexOf("?lamp2off") >0)//checks for off
                      {
                          digitalWrite(LED2, LOW); // set pin 6 low

                       }
           
                      }
            client.println("</p>");

            client.println("<br />");
          if(digitalRead(LED1)) // set pin 6 high
             {
                      LED1Status="ON";
                       
             }else{LED1Status="OFF";}
             
                       client.println("LAMP1:");      
                       client.println("</font>");
                       client.println("<font size=\"3\" color=\"#004000\">");
                       client.println(LED1Status);
                       client.println("</font>");

          client.println("<br />");
          
          if(digitalRead(LED2)) // set pin 6 high
             {
                      LED2Status="ON";
                       
             }else{LED2Status="OFF";}
             
                       client.println("LAMP2:");      
                       client.println("</font>");
                       client.println("<font size=\"3\" color=\"#004000\">");
                       client.println(LED2Status);
                       client.println("</font>");
                       
           
             client.println("<br />");
            client.println("<font size=\"3\">");
            client.println("TEMPERATURE:");
            client.println("</font>");
            int temp=(analogRead(tmp)*0.48828125); 
            client.println("<font color=\"#004000\" size=\"3\">");
            client.println(temp);
            client.println("C");
            if(temp>=29){
              digitalWrite(Alarm,HIGH); //to activate buzzer alarm
              client.println("Alarm is on");
            }
            
            client.println("</font>");
            
            
///////////////////////////////////////////////section to test if pins are actually high or low
            client.println("<hr />");
             
          client.println("<P align=\"center\"><b>Pin Values </b></p>"); 
          // output the value of digital pins 0 to 9 onto the web page
           int pinStatus = analogRead(tmp); //reads anolog pin status
            client.print("Analog pin ");
            client.println(tmp);
            client.print(" is   <b>");
            client.print(pinStatus);
            client.println("</b><br />");
          for (int digitalChannel = 4; digitalChannel < 9; digitalChannel++) 
          {
            boolean pinStatus = digitalRead(digitalChannel);
            client.print("Digital pin ");
            client.print(digitalChannel);
            client.print(" is   <b>");
            client.print(pinStatus);
            client.println("</b><br />");       
          }
          
           
            client.println("<hr />");
            client.println("</body>");
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
     readString=""; //clearing string for next read
  }
}

