/*
testWholeDisplay.ino
-test each segment in the display
-a simple example using Dean Reading's SevSeg library to light up all 4 digits plus the 4 decimal points on a 4 digit display
-the purpose of this example is to ensure you have the wires all hooked up right for every segment and digit, and to troubleshoot the display and wiring
 to ensure *every* segment and period lights up

By Gabriel Staples 
Website: http://www.ElectricRCAircraftGuy.com
My contact info is available by clicking the "Contact Me" tab at the top of my website.
Written: 1 Oct. 2015
Last Updated: 1 Oct. 2015
*/

/*
LICENSING:
-this *example file* only is modified from Dean Reading's original example, and is in the public domain.

Dean Reading's SevSeg library is as follows:
Copyright 2014 Dean Reading

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at 
http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <SevSeg.h>

SevSeg sevseg; //Instantiate a seven segment controller object

char label[6] = "letsgo";
byte hardwareConfig = N_TRANSISTORS; // See README.md for options
bool updating = false;

void setup()
{
  byte numDigits = 6;   
  byte digitPins[] = {19,18,17, 16, 15, 14,};
  byte segmentPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
  bool resistorsOnSegments = false; // Use 'true' if on digit pins
  byte hardwareConfig = N_TRANSISTORS; // See README.md for options
  
  //Note: 100 brightness simply corresponds to a delay of 2000us after lighting each segment. A brightness of 0 
                            //is a delay of 1us; it doesn't really affect brightness as much as it affects update rate (frequency).
                            //Therefore, for a 4-digit 7-segment + pd, COMMON_ANODE display, the max update rate for a "brightness" of 100 is 1/(2000us*8) = 62.5Hz.
                            //I am choosing a "brightness" of 10 because it increases the max update rate to approx. 1/(200us*8) = 625Hz.
                            //This is preferable, as it decreases aliasing when recording the display with a video camera....I think.

  // initialize serial:
  
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
//  label.reserve(200);
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);
  sevseg.setChars(label);
}


byte command;
int digit = 0;
int countup = 0;
int values [] = {1, 1, 1, 1};
int strikes[] = { A4, A5 };
int strike = 0;


void readData(){
  if (Serial.available()) {
    command = Serial.read();
    countup += command;
    sevseg.setNumber(command);
  //  return;
    if(command < 40){
      //beepTime = millis();
    }else if(command < 43){
      //strikeTime = millis();
    }

    if(command == 80){
      strike = 0;
      sevseg.setChars("letsgo");
      return;
      //digitalWrite(strikes[0], LOW);
      //digitalWrite(strikes[1], LOW);
    }else if(command == 81){
      strike = 1;
      sevseg.setChars("danger");
      return;
      //digitalWrite(strikes[0], HIGH);
      //digitalWrite(strikes[1], LOW);
    } else if(command == 82){
      strike = 2;
      sevseg.setChars("NOOOPE");
      return;
     // digitalWrite(strikes[0], HIGH);
      //digitalWrite(strikes[1], HIGH);
    }
    

    
    if(command < 20 && command >= 10){
      values[0] = command % 10;
    }else if(command < 30){
      values[1] = command % 10;
    }else if(command < 40){
      values[2] = command % 10;
    }else if(command < 50){
      values[3] = command % 10;
    }
    

    // 
    //sevseg.setNumber(command);
    //sevseg.setChars(label);

    //sevseg.setNumber(((values[0]) * 1000)+((values[1]+12) * 100)+(values[2] * 10)+(values[3] * 1));
    String labeltext =  String(strike)+" "+String(values[0])+String(values[1])+String(values[2])+String(values[3]);
    sevseg.setChars(labeltext.c_str());
    sevseg.refreshDisplay();
    //sevseg.refreshDisplay();
  }
}



void serialEventRun(void) {
  if (Serial.available()) serialEvent();
}


void serialEvent() {
  while (Serial.available()) {
    readData();
    /*
    //Serial.readBytes(label,128);
    int inChar = (int)Serial.read();
    sevseg.setNumber(inChar);
    sevseg.refreshDisplay();

  */
  }
    
  //}
}


void loop()
{
  //local vars
  readData();
  sevseg.refreshDisplay(); // Must run repeatedly; don't use blocking code (ex: delay()) in the loop() function or this won't work right
}
