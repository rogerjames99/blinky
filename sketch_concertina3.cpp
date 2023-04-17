 /* Parts from concertina-2021-1 now added to try and get display, up, down, select switches and menus working together
now an update from sept 11f
*/


#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>
#include <pindef.h>

//#include <ADC.h>
//include <SD.h>
//#include <SerialFlash.h> // as above
//#include <EEPROM.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define SCR_WDTH_CH 21    //screen width in characters
// #define STATUS_LED 13
#define DIAGNOSTICS 1

//FUNCTION PROTOTYPES ********************************

void sendSplash(void);
void screenInit(void); 
int testSwitches(void);
void sendFail(void);
void testLEDs(void);

// ******************************************** GLOBALS *****************************
//****************************************************************************************************************
//****************************************** This is setup  Initialization    ************************************
//****************************************************************************************************************
int test;  
void setup() {        // sends 'Splash Screen' to serial
  int a, b;
  Serial.begin(115200);     // was 115200
  if(DIAGNOSTICS) Serial.println("This is setup 43");

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    if(DIAGNOSTICS) Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  setPins();
  // Clear the buffer
  display.clearDisplay();

  digitalWrite(SCAN_BUTTON, HIGH);
  digitalWrite(SCAN_DIP1, HIGH);
  digitalWrite(SCAN_DIP2, HIGH);

  digitalWrite(OUT1, HIGH);    // turn off all LEDs at start
  digitalWrite(OUT2, HIGH);
  digitalWrite(OUT3, HIGH);
  digitalWrite(OUT4, LOW);
  digitalWrite(OUT5, LOW);
  digitalWrite(OUT6, LOW);
  digitalWrite(OUT7, LOW);
  digitalWrite(OUT8, LOW);
  digitalWrite(OUT9, LOW);
  digitalWrite(LID_TURN_ON, LOW);
  
  sendSplash();      // Welcome screen
  delay(2000);        // Goes to loop() next .....
  test = 1;
} 
  

//****************************************** This is loop  *******************************************

void loop() // regularly checking/polling all changes that control sound 
{
  int a, b, f, repeat;
  if(DIAGNOSTICS) Serial.print("This is loop 81\n");
  repeat = 1;
  while(repeat){
    switch(test){
      case 1:
        for(a = 1; a < 6 ; a++){
          f = 500;
          sendSplash();
          digitalWrite(SCAN_BUTTON, LOW);    
          display.print("Press Switch "); 
          display.print(a); 
          display.print(" Now"); 
          display.display();
          b = 0;
          while(f--){
            if(a == 1){if(digitalRead(SW1) == 0) b = 1;}  
            if(a == 2){if(digitalRead(SW2) == 0) b = 1;}  
            if(a == 3){if(digitalRead(SW3) == 0) b = 1;}  
            if(a == 4){if(digitalRead(SW4) == 0) b = 1;}  
            if(a == 5){if(digitalRead(SW5) == 0) b = 1;}  
            delay(10);
            }
          if(b){ display.print("OK"); 
            display.display();
            }
          else{display.print("BD"); 
            display.display();
            }
          delay(200);
        } 
      }
    }
        /* 
        
        
        
        
        
        a = testSwitches();
        display.print(a); // just for now .....
        display.display();
        delay(200);
        if(DIAGNOSTICS) Serial.print("a = ");
        if(DIAGNOSTICS) Serial.println(a);
        if(a != 31){
          delay(300);
          sendFail();
          display.print("\nSWS");
          repeat = 0; 
          if(a > 15){
            a -= 16;
            display.print(" 5 ");
            display.display();
            delay(300);
          } 
          if(a > 7){
            a -= 8;
            b = 8;
            display.print("4 ");
            display.display();
            delay(300);
          }   
          if(a > 3){
            a -= 4;
            b += 4;
            display.print("3 ");
            display.display();
            delay(300);
          } 
          if(a > 1){
            a -= 2;
            b += 2;
            display.print("2 ");
            display.display();
            delay(300);
          }   
          if(a)display.print("1"); 
          display.display(); 
          delay(300);   
          } 

        if(a == 31 || b == 14){
          display.println("\nSWs2,3&4=Bck,Fwd,Rpt");
          display.display();
          delay(300);
        }
        // test++;  // goto next test .....
        break;
      case 2:
        // sendSplash();
        display.println("Check LEDs 1 - 11");
        display.println("They must all flash");
        display.display();
        for(a = 0; a < 6; a++){
          delay(500); 
          testLEDs();
          }
        // test++;
        break;
      }
  }
  

  if(DIAGNOSTICS) delay(1000);


  // playing = 1;   //bypasses menu to test keyboard etc.
  // delay(100);   //Just a delay for dignostics
  */ 
}

// ************************************************ This is Send Splash ****************************************************** 
void sendSplash(void)
{
  if(DIAGNOSTICS) Serial.print("This is sendSplash 160\n");
  screenInit();
  display.println("Marchwood Diagnostic\n");
  /*display.println(" "); 
  //display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text 
  display.println(" ");
  //display.setTextColor(SSD1306_WHITE); // Draw white text  
  display.println(" ");   // 21 chars!/*/
  display.display();
  //delay(500);
}

// ***************************************************************************** Screen Init ************************* 
void screenInit(void)     // sent before, and is part of every display call
{
  if(DIAGNOSTICS) Serial.print("This is screenInit 175\n");
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
}

// ***************************************************************************** Button Update *********************** 
// *****************************************************************************  Second level Octave Menu Processing  *********** 

int testSwitches(void){
  int a, b, c, d, e, f = 1000, count;
  if(DIAGNOSTICS) Serial.print("This is testSwitches 186\n");
  //display.println("Testing Switches");
  //display.display();
  a = b = c = d = e = 0;

  while(f--){
    if(digitalRead(SW1) == 0) a = 1;  // SW1 is handled differently?
    if(digitalRead(SW2) == 0) b = 1;
    if(digitalRead(SW3) == 0) c = 1;
    if(digitalRead(SW4) == 0) d = 1;
    if(digitalRead(SW5) == 0) e = 1;    
    delay(10);
    }
  count = 0;
  if(a)count = 1;
  if(b)count += 2;
  if(c)count += 4;
  if(d)count += 8;
  if(e)count += 16;
  return(count);
}

// ***************************************************************************** Button Update *********************** 
// *****************************************************************************  Second level Octave Menu Processing  *********** 
// /* LEDs are part of ALL 9 OUT signals with the LED number being one 
// digit greater i.e. Out 1 is LED2. LED1 is part of the power supply and
// LED 11 is on In 7!!*/
void testLEDs(void)
{
  if(DIAGNOSTICS) Serial.print("This is testLEDs 216\n");
  digitalWrite(OUT1, LOW);    
  delay(500);
  digitalWrite(OUT1, HIGH);    // turn off LED
  delay(500);

  digitalWrite(OUT2, LOW);    
  delay(500);
  digitalWrite(OUT2, HIGH);    // turn off LED
  delay(500);  

  digitalWrite(OUT3, LOW);    
  delay(500);
  digitalWrite(OUT3, HIGH);    // turn off LED
  delay(500);

  digitalWrite(OUT4, HIGH);    
  delay(500);
  digitalWrite(OUT4, LOW);    // turn off LED
  delay(500);

  digitalWrite(OUT5, HIGH);    // turn off LED
  delay(500);
  digitalWrite(OUT5, LOW);    // turn off LED
  delay(500);

  digitalWrite(OUT6, HIGH);    
  delay(500);
  digitalWrite(OUT6, LOW);    // turn off LED
  delay(500);

  digitalWrite(OUT7, HIGH);    
  delay(500);
  digitalWrite(OUT7, LOW);    // turn off LED
  delay(500);

  digitalWrite(OUT8, HIGH);     
  delay(500);
  digitalWrite(OUT8, LOW);    // turn off LED
  delay(500);

  digitalWrite(OUT9, HIGH);     
  delay(500);
  digitalWrite(OUT9, LOW);    // turn off LED
  delay(500);

  digitalWrite(LID_TURN_ON, HIGH);    
  delay(500);
  digitalWrite(LID_TURN_ON, LOW);    // turn off LED1
  delay(500);

}

// ***************************************************************************** Button Update *********************** 
// *****************************************************************************  Second level Octave Menu Processing  *********** 

void sendFail(void) 
{
  if(DIAGNOSTICS) Serial.print("This is sendFail 274\n");
  sendSplash();
  display.println("\nTest Failed\n");
  display.display();
  delay(1000);
  sendSplash();
  test--;
}

/*
sendSplash();
        display.print("Test Switches now"); /// sb +ln
        display.display();
        digitalWrite(SCAN_BUTTON, LOW);    // initialy low
        a = testSwitches();
        display.print(a); // just for now .....
        display.display();
        delay(200);
        if(DIAGNOSTICS) Serial.print("a = ");
        if(DIAGNOSTICS) Serial.println(a);
        if(a != 31){
          delay(300);
          sendFail();
          display.print("\nSWS");
          repeat = 0; 
          if(a > 15){
            a -= 16;
            display.print(" 5 ");
            display.display();
            delay(300);
          } 
          if(a > 7){
            a -= 8;
            b = 8;
            display.print("4 ");
            display.display();
            delay(300);
          }   
          if(a > 3){
            a -= 4;
            b += 4;
            display.print("3 ");
            display.display();
            delay(300);
          } 
          if(a > 1){
            a -= 2;
            b += 2;
            display.print("2 ");
            display.display();
            delay(300);
          }   
          if(a)display.print("1"); 
          display.display(); 
          delay(300);   
          } 

        if(a == 31 || b == 14){
          display.println("\nSWs2,3&4=Bck,Fwd,Rpt");
          display.display();
          delay(300);
        }
        // test++;  // goto next test .....
        break;

*/
