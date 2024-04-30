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




void loop() {

  while (SerialBT) {
    digitalWrite(LED_BUILTIN,SerialBT.available()||Serial.available());
    delay(100);
    if(SerialBT.available()){
      
      String input = SerialBT.readString()+"°°°";
      SerialBT.println("1: "+split(input,'°',0));
      SerialBT.println("2: "+split(input,'°',1));
      SerialBT.println("3: "+split(input,'°',2));
      
      if(split(input,'°',0).equals("cmd")){
        SerialBT.println("executing  "+split(input,'°',1)+"  in cmd");
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.write('r');
        Keyboard.releaseAll();
        delay(50);
        Keyboard.print("cmd /c"+split(input,'°',1));
        delay(10);
        Keyboard.press(KEY_KP_ENTER);
        delay(10);
        Keyboard.releaseAll();
        SerialBT.println("done");
      }
      else if(split(input,'°',0).equals("key")){
        SerialBT.println("writing  "+split(input,'°',1));
        Keyboard.print(split(input,'°',1));
        if(split(input,'°',2) == "enter"){
          Keyboard.press(KEY_KP_ENTER);
          delay(10);
          SerialBT.println("entered enter key to enter");
        }
        Keyboard.releaseAll();
        SerialBT.println("done");
      }
      else if(split(input,'°',0).equals("winr")){
        SerialBT.println("executing  "+split(input,'°',1)+"  in win+r");
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.write('r');
        Keyboard.releaseAll();
        delay(50);
        Keyboard.print(split(input,'°',1));
        delay(10);
        Keyboard.press(KEY_KP_ENTER);
        delay(10);
        Keyboard.releaseAll();
        SerialBT.println("done");
      }else{
        SerialBT.println("not a valid command");
      }
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
