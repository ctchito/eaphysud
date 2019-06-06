// Programa : Display LCD 16x2 e modulo I2C
// Autor : Arduino e Cia

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);
 
void setup()
{
 lcd.begin (16,2);
 lcd.setBacklight(HIGH);
 lcd.setCursor(0,0);
 lcd.print("WELCOME Arduino!!");
}
 
void loop()
{
  // print the number of seconds since reset:
  lcd.setCursor(0,0);
  lcd.print(millis()/1000);
  lcd.setCursor(0,1);
  lcd.print("LCD CTCHITO I2C");
  lcd.setCursor(0, 1);
   
  //lcd.setBacklight(LOW);
  delay(500);
}
