#include "pitches.h"
#include "StringSplitter.h"

bool DEBUG = true;


// PIN configuration - contants
//  ----- Light bloc ----- 
//   * WorkLight Pin Ids : 2 -> 16
//   * ActionLigh Pin Ids : 17 -> 21
// -----  Push bloc ----- 
//   * WorkPush Pin Ids : 22 -> 36
//   * ActionPush Pin Ids : 37 -> 41

// Light bloc definition
int pinFirstWorkLight = 2;
int pinLastWorkLight = 16;
int pinFirstActionLight = 17;
int pinLastActionLight = 21;

// Push bloc definition
int pinFirstWorkPush = 22;
int PinLastWorkPush = 36;
int pinFirstActionPush = 37;
int pinLastActionPush = 41;

// Speaker bloc definition
int pinSpeaker = 42;

// Contant related to the PIN Id of the button that was pushed
int pinPushedID;

// Array giving the light that must be highlight from a button pushed
// The first 21 element of the array are empty because the first action button start at PIN 22
//  - Index -> PIN ID of the element
//  - Value -> list of PIN ID that will be hightlited
String push2Light[42] = {
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "17_20",
  "17_18_19",
  "17_18_19_20_21",
  "17_18",
  "17_18",
  "17_18_19_20_21",
  "17_18_19_20",
  "18_19",
  "18_19_20",
  "17_20_21",
  "20_21",
  "17_18_19_20_21",
  "17_18_19_20_21",
  "17_18_19_20_21",
  "17_18_19_20_21",
  "2_3_4_5_6_7_8_11_13_14_15_16",
  "3_4_5_6_7_8_9_10_13_14_15_16",
  "3_4_7_8_9_10_13_0_15_16",
  "2_4_7_8_10_11_12_13_14_15_16",
  "4_7_11_12_13_14_15_16"
};

/*
// Music
// D4 E4 C4 C3 G3
ring(NOTE_D4);
ring(NOTE_E4);     
ring(NOTE_C4); 
ring(NOTE_C3);
ring(NOTE_G3);
*/

// Duration of each notes (higher value -> slower notes)
int speed=90;

// Ring note
void ring(int note) {
  tone(pinSpeaker, note,720*.95); //
  delay(720);
  noTone(pinSpeaker);//  
}

// Fonction d'initialisation des PIN de type WorkerLight (1 à 16)
void initInputLight(){
  int i=0;
  String strOut = "[OUTPUT] Pin : ";
  for (i = pinFirstWorkLight; i <= pinLastActionLight; i++){
    pinMode(i, OUTPUT);

    if(DEBUG) Serial.println(strOut + i);
  }
}

// Fonction d'initialisation des PIN de type WorkerButton (17 à 31)
void initInputButton(){
  int i;
  String strOut = "[INPUT] Pin ";

  for(i = pinFirstWorkPush; i <= pinLastActionPush; i++){
    pinMode(i, INPUT);
    if(DEBUG) Serial.println(strOut + i);
  }
}

// Fonction pour récupérer l'identifiant de la première PIN appuyée
int getButtonPushed(){
  int buttonId = 0;
  int i;
  String strOut = "[PUSHED] Pin : ";
  for(i = pinFirstWorkPush; i <= pinLastActionPush + 1; i++){
    if(digitalRead(i) == LOW){
      buttonId = i;
      break;
    }
  }
  if(buttonId != pinLastActionPush + 1 && buttonId != 0 && DEBUG) Serial.println(strOut + buttonId);
  return buttonId;
}

// Fonction pour repositionner les boutons (push) dans leur état initial
void resetButtonState(){
  int i;
  String strOut = "[RESET-PUSHED] Pin : ";
  for(i = pinFirstWorkPush; i <= pinLastActionPush; i++){
    digitalWrite(i, HIGH);
    //if(DEBUG) Serial.println(strOut + i);
  }
}

// Fonction d'initialisation de la lumière des bouttons (Worker )
void initLightsOn(){
  int i;
  String strOut = "[ON] Pins : ";
  String pinList = "";
  String separator = "";

  for(i = pinFirstWorkLight;i <= pinLastActionLight;i++){
    if(i != 2) separator = ", ";
    digitalWrite(i, HIGH);
    pinList = pinList + separator + i;
  }
  //if(DEBUG) Serial.println(strOut + pinList);
}

// Fonction d'initialisation de la lumière des bouttons (Worker )
void initLightsOff(){
  int i;
  String strOut = "[OFF] Pins : ";
  String pinList = "";
  String separator = "";

  for(i = pinFirstWorkLight;i <= pinLastActionLight;i++){
    if(i != 2) separator = ", ";
    digitalWrite(i, LOW);
    pinList = pinList + separator + i;
  }
  //if(DEBUG) Serial.println(strOut + pinList);
}


void setup() {
  Serial.begin(9600);

  initInputLight();
  initInputButton();
}

void loop() {
  
  // Put all lights on
  initLightsOff();

  // Getting the Pin ID related to the pushed button
  pinPushedID = getButtonPushed();
  
  // If a button is pushed
  if(pinPushedID != pinLastActionPush + 1 && pinPushedID != 0){   
    // Light on the light attached to the pushed button
    // -20 is the correspondance between the PIN link to the pushed button and his light
    // For exemple : If BIM Manager button is pushed (PIN ID 22) the PIN light 2 will be turned off
    digitalWrite(pinPushedID-20, HIGH);

    String strOut = "[PUSH2LIGHT] Value :";
    if(DEBUG) Serial.println(strOut + push2Light[pinPushedID]);

    // Split methods to get each related light button
    StringSplitter *splitter = new StringSplitter(push2Light[pinPushedID], '_',15);
    int itemCount = splitter->getItemCount();

    // Loop on each element of the splitter object
    for(int i = 0; i < itemCount; i++){
      
      // Highlight the button relate to the current element (subString)
      digitalWrite(splitter->getItemAtIndex(i).toInt(), HIGH);

      // Ring the tone relate to the Action element linked
      if(splitter->getItemAtIndex(i) == "17") ring(NOTE_D4);
      if(splitter->getItemAtIndex(i) == "18") ring(NOTE_E4);
      if(splitter->getItemAtIndex(i) == "19") ring(NOTE_C4);
      if(splitter->getItemAtIndex(i) == "20") ring(NOTE_C3);
      if(splitter->getItemAtIndex(i) == "21") ring(NOTE_G3);
    }

    // Delay between each new try
    delay(500);
  }  
  
  // Reset all pushed button state
  resetButtonState();
}
