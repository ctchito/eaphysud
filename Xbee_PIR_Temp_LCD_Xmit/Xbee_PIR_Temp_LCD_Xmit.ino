/* 
 * //////////////////////////////////////////////////
 * //making sense of the Parallax PIR sensor's output with Buzzer and Temperature sensing
 * //////////////////////////////////////////////////
 *
 */
#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into port 2 on the Arduino

/////////////////////////////
//VARS
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;        

//the time when the sensor outputs a low impulse
 unsigned long lowIn;         
unsigned long lowInTemp;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
unsigned long pause = 5000;  
unsigned long pauseTemp = 3000;

boolean lockLow = true;
boolean takeLowTime;  

const int ledPin2 = 13; // the pin that the LED is attached to
int pirPin = 9;    //the digital pin connected to the PIR sensor's output
int ledPin = 12;
boolean pin = 0;

#define ONE_WIRE_BUS 8
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);


/////////////////////////////
//SETUP
void setup(){
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(pirPin, LOW);
  digitalWrite(ledPin2, LOW);
  
  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    Serial.println("Dallas Temperature Init ... ");
  // Start up the library
  sensors.begin();
    delay(50);

    
  }

////////////////////////////
//LOOP
void loop(){
     // see if there's incoming serial data:
    serialEvent();
  
     if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
       if(lockLow){  
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;
         Serial.print('H');            
         Serial.println("---");
         Serial.print("motion at ");
         Serial.print(millis()/1000);
         Serial.println(" sec"); 
         delay(50);
         }         
         takeLowTime = true;
       }

     if(digitalRead(pirPin) == LOW){       
       digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state

       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true; 
           Serial.print('L');                       
           Serial.print(" motion ended at ");      //output
           Serial.print((millis() - pause)/1000);
           Serial.println(" sec");
           
           delay(50);
           }
       }//PirPin low

        if(millis() - lowInTemp > pauseTemp)
        {  
            lowInTemp = millis();
            //makes sure this block of code is only executed again after 
            // call sensors.requestTemperatures() to issue a global temperature 
            // request to all devices on the bus
            //Serial.print("Requesting temperatures...");
            sensors.requestTemperatures(); // Send the command to get temperatures
            //Serial.println("DONE");
            Serial.print("Temp: ");
            //Serial.print("Temperature for the device 1 (index 0) is: ");
            Serial.println(sensors.getTempCByIndex(0)); 
        }//Temp interval 

  }//loop

  /*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    //inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '*') {
      pin++;
      digitalWrite(ledPin2, pin);
    }
  }
}
