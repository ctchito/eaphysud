
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
//LiquidCrystal(rs =46, rw= , enable= 47, d4= , d5, d6, d7)
//LiquidCrystal(rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(32, 30, 28, 26, 24, 22);


const int ledPin = 12; // the pin that the BUZZER is attached to
int incomingByte;      // a variable to read incoming serial data into

String content = "";
char character;


void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  
  // Print a message to the LCD.
  lcd.print("hello home...");
  
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  
  if (Serial.available() > 0) {
    //lcd.clear();
    //while(Serial.available()>0) 
    {
         character = Serial.read();
          // read the oldest byte in the serial buffer:
          incomingByte = character;
          lcd.write(incomingByte);
          // if it's a capital H (ASCII 72), turn on the LED:
          if (incomingByte == 'H') {
            digitalWrite(ledPin, HIGH);
            Serial.print("*");
          } 
          // if it's an L (ASCII 76) turn off the LED:
          if (incomingByte == 'L') {
            digitalWrite(ledPin, LOW);
          }       
         content.concat(character);
    }
    Serial.print("#");
    Serial.print(content);
    content="";
    //lcd.print(content);
    Serial.println("#");
    Serial.flush();
    
  }//if serial.aviable >0
  
}






/**
// Buffer to store incoming commands from serial port
String inData;

void setup() {
    Serial.begin(9600);
    Serial.println("Waiting for Raspberry Pi to send a signal...\n");
}

void loop() {
    while (Serial.available() > 0)
    {
        char recieved = Serial.read();
        inData += recieved; 

        // Process message when new line character is recieved
        if (recieved == '\n')
        {
            Serial.print("Arduino Received: ");
            Serial.print(inData);

            inData = ""; // Clear recieved buffer
        }
    }
}


**/
