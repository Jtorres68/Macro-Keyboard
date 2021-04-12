# Macro-Keyboard

Libraries needed:
  - Keyboard
  - U8glib
  - Adafruit BusIO
  - Adafruit GFX
  - Adafruit SSD1306
  
To install the Keyboard, U8glib, and Adafruit BusIO library:
  - On the Arduino IDE, go to Tools > Manage Libraries or press Ctrl+Shift+I
  ![Manage Lib](https://github.com/Jtorres68/Macro-Keyboard/blob/master/pic/ManageLib.png)
  - From there, search for the 3 libraries and select Install
  ![Example install](https://github.com/Jtorres68/Macro-Keyboard/blob/master/pic/exampleLib.png)
  
To install Adafruit GFX and Adafruit SSD1306:
  - Attached are two zip libraries, download the 2 zip folders and then in the Arduino IDE, go to Sketch > Include Library > Add .ZIP Library and select the downloaded libraries to open
  ![Example Zip install](https://github.com/Jtorres68/Macro-Keyboard/blob/master/pic/AddZipLib.png)
  
## Button Functionality
  - To explain the basics:
  ```
     Keyboard.press()           //presses and holds the key written inside
     Keyboard.print()           //sends or "prints" a string or a character to the computer 
     Keyboard.println()         //sends a string or character to the computer, followed by a newline and return
     Keyboard.release()         //releases the specified key
     Keyboard.releaseAll()      //releases all keys held  
     delay()                    //pauses the program for the amount of time in ms specified
   ```

   For more info, please visit https://www.arduino.cc/reference/en/language/functions/usb/keyboard/
   
   For more Modifiers, please visit https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/
