// A very simple Serial-over-BT app that reads input from the host and CAPITALIZES it.
// Released to the public domain by Earle F. Philhower, III, in February 2023

// Under Linux to connect to the PicoW
// 1. Pair to the "PicoW Serial XX:XX..." device using your favorite GUI, entering a PIN of "0000"
// 2. Execute "sudo rfcomm bind 0 00:00:00:00:00:00" to make a `/dev/rfcomm0" device, replacing the "00:00.." with the MAC as listed in the device name
// 3. Run "minicom -D /dev/rfcomm0" and type away
// 4. To remove the virtual serial port, execute "sudo rfcomm release rfcomm0"

// Under Windows to connect to the PicoW
// 1. Pair to the "PicoW Serial XX:XX..." device using the copntrol panel, ignoring any PIN it says to check for
// 2. At this point you will have a new COM: port.  You may need to use the Device Manager to find it's number.
// 3. Open up COMX: in your favorite terminal application and type away

// Under Mac to connect to the PicoW
// 1. Open System Preferences and go in the bluetooth section. You should find a bluetooth device called
//    PicoW Serial XX:XX:... Click Connect button.
// 2. A /dev/tty.PicoWSerialXXXX becomes available.
// 3. Connect to this device with your favorite terminal application.


#include <SerialBT.h>

#include "Keyboard.h"
void setup() {
  SerialBT.begin();//"olaf.maydell remote shell"
  Keyboard.begin(KeyboardLayout_de_DE);
  pinMode(LED_BUILTIN,OUTPUT);
}

//[[
// [F1-24]
//  [CTRL]
//  [SHIFT]
//  [ALT]
//  [ENTER]
//  [WIN]
// [TAB]
// [CAPS]
// [DEL]
// [INSERT]
// [ESC]
// [DELAY0-99999999]


boolean ParseText(String input){
  int index = 0;
  int cmdIndex = 0;
  boolean cmd = false;

  while(input.substring(index,index) != null){
    String currentChar = char(input.substring(index,index));
    if(currentChar.equals("[")&&!cmd){
      cmd = true; // start reding command
      index++;
    
    }else if(currentChar.equals("]")){
      cmd = false;//end cmd
      KBRelease();
      index++;
    }else if(cmd){
                  //read cmd and no num
      boolean searching = true;
      cmdIndex = index;
      int wdt = 0;
      while(searching){
        if(wdt > 2000){
          return false;
        }
        wdt++;
        String cmdParser = input.substring(index,cmdIndex); //get curret command part
        boolean nextNum = input.subString(cmdIndex+1,cmdIndex+1).isDigit(); //see if next is number
        done = true;
        if(nextNum){
          if(cmdParser.equals("F")){
            val = parseNumber(input,cmdIndex+1,index);

            //f keys
          }else if(cmdParser.equals("DELAY")){
            val = parseNumber(input,cmdIndex+1,index);
            delay(val);
            SerialBT.println("sleeping "+String(val)+" ms");
            //delay
          }else{
            done = false;
          }
        }else{
          if(cmdParser.equals("ENTER")){
            
            KBPress(KEY_KP_ENTER);
          }else if(cmdParser.equals("WIN")){
            
            KBPress(KEY_LEFT_GUI);
          }else if(cmdParser.equals("ALT")){
            
            KBPress(KEY_LEFT_ALT);
          }else if(cmdParser.equals("SHIFT")){
       
            KBPress(KEY_LEFT_SHIFT);
          }else if(cmdParser.equals("CTRL")){
            
            KBPress(KEY_LEFT_CTRL);
          }else if(cmdParser.equals("TAB")){
           
            KBPress(KEY_TAB);
          }else if(cmdParser.equals("CAPS")){
           
            KBPress(KEY_CAPS_LOCK);
          }else if(cmdParser.equals("DEL")){
         
            KBPress(KEY_DELETE);
          }else if(cmdParser.equals("INSERT")){
           
            KBPress(KEY_INSERT);
          }else if(cmdParser.equals("ESC")){
           
            KBPress(KEY_ESC);
          }else if(cmdParser.equals("[")){
            Keyboard.write("[");
          }else{
            done=false;
          }
        }
        if(done){
          searching = false;
        }
      }
      index = cmdIndex+1;
      cmd = false;
    }else{
      Keyboard.write(currentChar); //write character
      index++;
    }
  }
  return true;
}

void KBPress(int keyToPress){
  Keyboard.press(keyToPress);
}

void KBRelease(){
  Keyboard.releaseAll();
}

int parseNumber(String inString, int startPos, int& newIndex){
  int digits = 0;
  while(true){
    if(digits > 32){
      SerialBT.println("number too long");
      return 0;
    }
    if(!inString.substring(startPos,startPos+digits).isDigit()){
      newIndex = startPos+digits;
      if(digits == 0){
        SerialBT.println("not a number");
        return 1;
      }
      return toInt(inString.substring(startPos,startPos+digits-1));
    }
    digits++;
  } 
}




void loop() {
  while (SerialBT) {
    if(SerialBT.available()){ 
       ParseText(SerialBT.readString());
    }
  }
}


String split(String s, char parser, int index) {        //I STOLE THIS CODE
  String rs="";                                         //I STOLE THIS CODE
  int parserIndex = index;                              //I STOLE THIS CODE
  int parserCnt=0;                                      //I STOLE THIS CODE
  int rFromIndex=0, rToIndex=-1;                        //I STOLE THIS CODE
  while (index >= parserCnt) {                          //I STOLE THIS CODE
    rFromIndex = rToIndex+1;                            //I STOLE THIS CODE
    rToIndex = s.indexOf(parser,rFromIndex);            //I STOLE THIS CODE
    if (index == parserCnt) {                           //I STOLE THIS CODE
      if (rToIndex == 0 || rToIndex == -1) return "";   //I STOLE THIS CODE
      return s.substring(rFromIndex,rToIndex-1);        //I STOLE THIS CODE
    } else parserCnt++;                                 //I STOLE THIS CODE
  }                                                     //I STOLE THIS CODE
  return rs;                                            //I STOLE THIS CODE
}                                                       //I STOLE THIS CODE
