/***************************************************************************
 * RFIDuino Demo 3
 *
 * This demo uses the RFIDuino sheild, a Geekduino (or other Arduino Board)
 * and the RFIDuino library to scan for a tag. If the tag matches one of multiple
 * pre-set 'key' tag, then a green light turns on and a 'success' sound is played.
 * Pin 9 is then set to high for a short time. If a relay is connected to pin
 * 9, then the relay will activate for that time period. If the relay is connected
 * to a solenoid or other motorized locking mechanism, this creates a RFID unlock
 * system. 
 *
 * If the tag is not matched to the 'key' then a red light and 'error' sound will
 * play. The solenoid will remain inactive. 
 *
 * This demo supports multiple key tags. To add / remove tags, first change the 
 * #define NUMBER_OF_CARDS to match the number of key cards you plan to have. Now
 * add/remove the lines to the keyTag array. The code will take care of the rest.
 *
 * The RFIDuino library is compatible with both versions of the RFIDuino shield
 * (1.1 and 1.2), but the user must initialize the library correctly. See 
 * 'HARDWARE VERSION' instructions near the beginning of the code.
 *
 *
 * The RFIDuino Shield is designed to be used with 125khz EM4100 family tags. 
 * This includes any of the EM4102 tags sold by Trossen Robotics/ RobotGeek. 
 * The RFIDuino shield may not work with tags outside the EM4100 family
 *
 *
 * Links
 *    RFIDUino Getting Started Guide
 *      http://learn.robotgeek.com/getting-started/41-rfiduino/142-rfiduino-getting-started-guide.html
 *    RFIDUino Library Documentation
 *      http://learn.robotgeek.com/41-rfiduino/144-rfiduino-library-documentation.html
 *    RFIDUino Shield Product Page
 *      http://www.robotgeek.com/rfiduino
 *  
 *
 * User Output pins
 *    myRFIDuino.led1 - Red LED
 *    myRFIDuino.led2 - Green LED
 *    myRFIDuino.buzzer - Buzzer
 *
 ***************************************************************************/

#include <RFIDuino.h> //include the RFIDuino Library

/***************
* HARDWARE VERSION - MAKE SURE YOU HAVE SET THE CORRECT HARDWARE VERSION BELOW
* Version 1.1 has a 4-pin antenna port and no version marking
* Version 1.2 has a 2-pin antenna port and is marked 'Rev 1.2'
*****************/
RFIDuino myRFIDuino(1.1);     //initialize an RFIDuino object for hardware version 1.1
//RFIDuino myRFIDuino(1.2);       //initialize an RFIDuino object for hardware version 1.2

//Define the pins for our outputs
#define   RELAY 9               //the Buzzer on the RFIDuino shield
#define   OPEN_TIME 1000        //the time (in milliseconds) that the solenoid will be activated for
#define   NUMBER_OF_CARDS 5     //total numer of key cards that the system will respond to.   

byte tagData[5];                   //Holds the ID numbers from the tag

byte keyTag[NUMBER_OF_CARDS][5] ={
{2,13,30,0,180},  //Tag 1 //commenting each tag with a description can help you keep track of them
{1,12,59,62,48},  //Tag 2
{2,13,29,233,78}, //Tag 3
{2,13,29,208,140},//Tag 4
{2,13,29,221,53}, //Tag 5
};  

boolean verifyKey = false;         //true when a verified tag has been determined to match the key tag, false otherwise
int i;

void setup()
{
  //The RFIDUINO hardware pins and user outputs(Buzzer / LEDS) are all initialized via pinMode in the library

  //The relay must be manuualy initalized as an output
  pinMode(RELAY, OUTPUT);
}

void loop()
{    
   //scan for a tag - if a tag is sucesfully scanned, return a 'true' and proceed
  if(myRFIDuino.scanForTag(tagData) == true)
  {
    verifyKey = false;  //set verfiy key to false incase previous ones have made it true
    while(i<NUMBER_OF_CARDS && verifyKey == false)
    {
      verifyKey = myRFIDuino.compareTagData(tagData, keyTag[i]);//run the comparetagData to check the tag just read against the 'key' tag we defined.
      i++;            
    }

    if(verifyKey == true)//if a 'true' is returned by comparetagData, the current read is a key tag
    {                       
      digitalWrite(myRFIDuino.led2,HIGH);         //turn green LED on
      digitalWrite(RELAY,HIGH);                   //turn relay/solenoid on
      myRFIDuino.successSound();                  //Play a 'success' sound from the buzzer, 3 notes acsending 
      delay(OPEN_TIME);                           //the amount of time that the solenoid will remain open for
      digitalWrite(RELAY,LOW);                    //turn the relay/solenoid off
      digitalWrite(myRFIDuino.led2,LOW);          //turn the green LED off
    }
  
    else//otherwise the tag is not a key tag
    {        
      digitalWrite(myRFIDuino.led1,HIGH);         //turn red LED on
      myRFIDuino.errorSound();                    //Play a 'error' sound from the buzzer, 3 notes acsending 
      delay(OPEN_TIME);                           //wait /delay before turning the led off
      digitalWrite(myRFIDuino.led1,LOW);          //turn the red LED off        
    }


  }    
}//end loop()
 


  
  

