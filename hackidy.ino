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

// [[
// [F1-24]
// [CTRL]
// [SHIFT]
// [ALT]
// [ENTER]
// [WIN]
// [TAB]
// [CAPS]
// [DEL]
// [INSERT]
// [ESC]
// [SLEEP0-99999999]


boolean ParseText(String input){
  int index = 0;
  int cmdIndex = 0;
  boolean cmd = false;
  int len = input.length();
  SerialBT.println(input);
  //SerialBT.print(len);
  while(index < len){
    String currentChar = String(input.charAt(index));
    //SerialBT.print(currentChar);
    if(currentChar.equals("[")&&!cmd){
      cmd = true; // start reding command
      index++;
    
    }else if(currentChar.equals("]")){
      cmd = false;//end cmd
      //KBRelease();
      Keyboard.releaseAll();
      //SerialBT.println("releasing all");
      index++;
    }else if(cmd){
                  //read cmd and no num
      boolean searching = true;
      cmdIndex = index;
      int wdt = 0;
      while(searching){
        if(wdt > 2000){
          //SerialBT.println("cmd parser timed out");
          return false;
        }
        wdt++;
        String cmdParser = input.substring(index,cmdIndex); //get curret command part
        //SerialBT.println("checking "+cmdParser);
        boolean nextNum = isDigit(input.charAt(cmdIndex+1)); //see if next is number
        boolean done = true;
        if(nextNum){
          if(cmdParser.equals("F")){
            int val = parseNumber(input,cmdIndex,index);
            SerialBT.println("F"+String(val));
            //f keys
          }else if(cmdParser.equals("SLEEP")){
            int val = parseNumber(input,cmdIndex,index);
            cmdIndex += String(val).length();
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
            Keyboard.write('[');
          }else{
            done=false;
          }
        }
        cmdIndex++;
        if(done){
          searching = false;
        }
      }
      index = cmdIndex-1;
      cmd = false;
    }else{
      Keyboard.print(currentChar); //write character
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
  int digits = -1;
  while(true){
    if(digits > 32){
      SerialBT.println("number too long");
      return 0;
    }
    if(!isDigit(inString.charAt(startPos+digits+1))){
      //newIndex = newIndex+digits+1;
      if(digits == -1){
        SerialBT.println("not a number");
        return 1;
      }
      return inString.substring(startPos,startPos+digits+1).toInt();
    }
    digits++;
  } 
}




void loop() {
  while (SerialBT) {
    if(SerialBT.available()){
       String BTInput = "";
       int timeout = 0;
       while(timeout < 100000){
        if(SerialBT.available()){
          BTInput = BTInput+char(SerialBT.read());
          timeout = 0;
        }
        timeout++;
       }
       ParseText(BTInput.substring(0,BTInput.length()-2));
    }
  }
}

