// Example 55.3

#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8);
int x,y;
String textForSMS;

void setup()
{
  SIM900.begin(19200);
  SIM900power();  
  delay(20000);  // give time to log on to network. 
  randomSeed(analogRead(0));
}

void SIM900power()
// software equivalent of pressing the GSM shield "power" button
{
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(7000);
}

void sendSMS(String message)
{
  SIM900.print("AT+CMGF=1\r");                     // AT command to send SMS message
  delay(100);
  SIM900.println("AT + CMGS = \"+237699018486\"");  // recipient's mobile number, in international format
  delay(100);
  SIM900.println(message);                         // message to send
  delay(100);
  SIM900.println((char)26);                        // End AT command with a ^Z, ASCII code 26
  delay(100); 
  SIM900.println();
  delay(5000);                                     // give module time to send SMS
  SIM900power();                                   // turn off module
}

void loop()
{
  x = random(0,255);
  y = random(0,255);
  textForSMS = "Your random numbers are ";
  textForSMS.concat(x);
  textForSMS = textForSMS + " and ";
  textForSMS.concat(y);
  textForSMS = textForSMS + ". Enjoy!";  
  sendSMS(textForSMS);
  do {} while (1);
}
