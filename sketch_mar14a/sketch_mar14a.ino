/*
A simple macroboard designed with 3 different profiles using an Arduino Pro Micro.
When plugged in, an OLED display will display the first profile shown (Win Shortcuts) and each buttons associated macro.
To scroll through the profiles, the "SELECT" button has to be pressed. Each profile has their own set of macros and display.
Down below are comments that may specifically say "Start Change", "End Change" to make this easier on others if they would like to change what the macro buttons do.
If editing the button output, please state the new outcome in the OLED string that is to be displayed.

This was a surprise project made for a good friend :D -Joselin Torres
*/


#include "Keyboard.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Pin definitions
#define SELECT 16
#define BUTTON_PIN1 4
#define BUTTON_PIN2 5
#define BUTTON_PIN3 6
#define BUTTON_PIN4 7
#define BUTTON_PIN5 8
#define BUTTON_PIN6 9
#define BUTTON_PIN7 10
#define BUTTON_PIN8 14

// Special key definitions
#define KEY_PRNT_SCRN 0xCE  //Windows Print Screen

//variables used to check and enable input buttons
const uint8_t buttons[] = {SELECT, BUTTON_PIN1, BUTTON_PIN2, BUTTON_PIN3, BUTTON_PIN4, BUTTON_PIN5, BUTTON_PIN6, BUTTON_PIN7, BUTTON_PIN8};
int buttonSize = sizeof(buttons)/sizeof(buttons[0]);

//variables used to intiailize and determine which profile is currently enabled
char pagenum[] = {'0', '1', '2'};
int xprof = 0;
char currprof = '0';  //current profile

//set for multiple buttons
//only change if adding more buttons, they are variables used for debounce function
int buttonState[8];             // the current reading from the input pin
int lastButtonState[] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};   // the previous reading from the input pin
unsigned long lastDebounceTime[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};  // the last time the output pin was toggled
unsigned long debounceDelay[] = {50, 50, 50, 50, 50, 50, 50, 50, 50};    // the debounce time; increase if the output flickers


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  //DOUBLE CHECK THIS (fail safe)
  // Safety check. Ground pin #1 (RX) to cancel keyboard inputs.
  pinMode(1, INPUT_PULLUP);
  if(!digitalRead(1)){
    for(;;){}
  }
    
  // Set the buttons as an input
  //DO NOT CHANGE!!
  for(int i = 0; i < buttonSize; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }

  Serial.begin(9600);
  
  //for the oled display screen 
  //DO NOT CHANGE!!
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);

  //setting up text
  display.setTextSize(1);
  display.setTextColor(WHITE);

}

void loop() {
  //select a profile
  if (pressed(digitalRead(SELECT), 0) == LOW) {
    //reached the last profile, go back to beginning profile
    if (xprof == 2) {
      xprof = 0;
    } else {
      xprof++;
    }
    currprof = pagenum[xprof];
  }

  //operate the main doAction function (Windows shortcut)
  if (currprof == '0') {
    display.clearDisplay();
    //display title
    display.setCursor(SCREEN_WIDTH/6, 5);
    // Display static text
    display.println("Win Shortcuts");

    //display shortcuts
    /*IF EDITING A SHORTCUT IN THE MAIN FUNCTION, BE SURE TO CHANGE THE TEXT TO CORRESPOND TO THE EDIT*/

    //Start Change
    display.setCursor(0, 20); //sets the placement of the cursor in a row, column position to start typing
    display.println("1-Copy 2-Paste 3-Cut");
    display.setCursor(0, 30);
    display.println("4-Undo 5-Alt+Tab");
    display.setCursor(0, 40);
    display.println("6-Prop. 7-Exit 8-Lock");
    //End Change
    
    display.display(); //displays text written above
    
    //time for buttons
    //DO NOT CHANGE!!
    for(int i = 0; i < buttonSize; i++) {
      if (pressed(digitalRead(buttons[i]), i) == LOW) {
        doActionMain(buttons[i]);
      }
    }

  //operate the Applicaton doAction function (Fusion, Cura, etc.)
  } else if (currprof == '1') {
    display.clearDisplay();
    //display title
    display.setCursor(SCREEN_WIDTH/6, 5);
    // Display static text
    display.println("Applications");

    //display shortcuts
    /*IF EDITING A SHORTCUT IN THE MAIN FUNCTION, BE SURE TO CHANGE THE TEXT TO CORRESPOND TO THE EDIT*/

    //Start Change
    display.setCursor(0, 20);
    display.println("1-Fus360 2-Cura");
    display.setCursor(0, 30);
    display.println("3-Spyder 4-CForge");
    display.setCursor(0, 40);
    display.println("5-GW2 6-Note++"); 
    display.setCursor(0, 50);
    display.println("7-Arduino 8-paint.net");
    //End Change
    
    display.display(); 
    
    //time for buttons
    //DO NOT CHANGE!!
    for(int i = 0; i < buttonSize; i++) {
      if (pressed(digitalRead(buttons[i]), i) == LOW) {
        doActionApp(buttons[i]);
      }
    }

  //operate the hotkeys Shift + F1-12
  } else if (currprof == '2') {
    display.clearDisplay();
    //display title
    display.setCursor(SCREEN_WIDTH/6, 5);
    // Display static text
    display.println("Program Macros");

    //display shortcuts
    /*IF EDITING A SHORTCUT IN THE MAIN FUNCTION, BE SURE TO CHANGE THE TEXT TO CORRESPOND TO THE EDIT*/

    //Start Change
    display.setCursor(0, 20);
    display.println("1-F13 2-F14 3-F15");
    display.setCursor(0, 30);
    display.println("4-F16 5-F17 6-F18");
    display.setCursor(0, 40);
    display.println("7-F19 8-F20");
    //End Change
    
    display.display(); 
    
    //time for buttons
    //DO NOT CHANGE!!
    for(int i = 0; i < buttonSize; i++) {
      if (pressed(digitalRead(buttons[i]), i) == LOW) {
        doActionHidden(buttons[i]);
      }
    }
  }
}

/*
 * Debounce function
 *to replace delays when a button is pressed
 *prevents double clicks or misclicks
 *
 *DO NOT CHANGE!!
 */

boolean pressed(boolean state, int but) {
  if (state != lastButtonState[but]) {
    // reset the debouncing timer
    lastDebounceTime[but] = millis();
  }
  if ((millis() - lastDebounceTime[but]) > debounceDelay[but]) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    if (state != buttonState[but]) {
      buttonState[but] = state;

      //button is pressed
      if (buttonState[but] == LOW) {
        lastButtonState[but] = state;
        return(LOW);
      }
    }
  
  }
  //button is released
  lastButtonState[but] = state;
  return(HIGH);
}

/*
  The functions below display the functionality of each button.
  To explain the basics:
      Keyboard.press() presses and holds the key written inside
      Keyboard.print() sends or "prints" a string or a character to the computer 
      Keyboard.println() sends a string or character to the computer, followed by a newline and return
      Keyboard.release() releases the specified key
      Keyboard.releaseAll() releases all keys held  
      delay() pauses the program for the amount of time in ms specified

   For more info, please visit https://www.arduino.cc/reference/en/language/functions/usb/keyboard/
   For more Modifiers, please visit https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/
*/

/*Main keys to be used as windows shortcuts
 * Turns win shortcut combos as a single key press
 * Ex: Ctrl+C is now BUTTON_PIN1
 */
void doActionMain(uint8_t button) {
  //You may change inside the case event
  //but do not remove the break or releaseAll functionality at the end
  switch (button) {
    //copy
    case BUTTON_PIN1:
      Keyboard.press(KEY_LEFT_CTRL);  // send a 'CTRL' to the computer via Keyboard HID
      Keyboard.press('c'); //send a 'c' to the computer
      Keyboard.releaseAll();            // Release both of the above keys.
      break;

    //paste
    case BUTTON_PIN2:
      Keyboard.press(KEY_LEFT_CTRL);  // send a 'CTRL' to the computer via Keyboard HID
      Keyboard.press('v'); //send a 'v' to the computer
      Keyboard.releaseAll();            // Release both of the above keys.
      break;

    //Cut
    case BUTTON_PIN3:
      Keyboard.press(KEY_LEFT_CTRL);  // send a 'CTRL' to the computer via Keyboard HID
      Keyboard.press('x'); //send a 'x' to the computer
      Keyboard.releaseAll();            // Release both of the above keys.
      break;

    //Undo
    case BUTTON_PIN4:
      Keyboard.press(KEY_LEFT_CTRL);  // send a 'CTRL' to the computer via Keyboard HID
      Keyboard.press('Z'); //send a 'z' to the computer
      Keyboard.releaseAll();            // Release both of the above keys.
      break;

    //Alt+Tab
    case BUTTON_PIN5:
      Keyboard.press(KEY_LEFT_ALT);  // send a 'ALT' to the computer via Keyboard HID
      Keyboard.press(KEY_TAB); //send a 'Tab' to the computer
      Keyboard.releaseAll();            // Release both of the above keys.
      break;

    //Properties
    case BUTTON_PIN6:
      Keyboard.press(KEY_LEFT_ALT);  // send a 'ALT' to the computer via Keyboard HID
      Keyboard.press(KEY_RETURN); //send a 'Enter' to the computer
      Keyboard.releaseAll();            // Release both of the above keys.
      break;

    //Alt+F4
    case BUTTON_PIN7:
      Keyboard.press(KEY_LEFT_ALT);  // send a 'ALT' to the computer via Keyboard HID
      Keyboard.press(KEY_F4); //send a 'F4' to the computer
      Keyboard.releaseAll();            // Release both of the above keys.
      break;

    //Win+Lock
    case BUTTON_PIN8:
      Keyboard.press(KEY_LEFT_GUI);  // send a 'CTRL' to the computer via Keyboard HID
      Keyboard.press('l'); //send a 'l' to the computer
      Keyboard.releaseAll();            // Release both of the above keys.
      break;
  }
}

/*Keys used to open programs
 * Press a key and it opens the windows search bar
 * prints the application name and opens it
 */
void doActionApp(uint8_t button) {
  //You may change inside the case event
  //but do not remove the break or releaseAll functionality at the end
  switch (button) {
    //Fusion 360
    case BUTTON_PIN1:
      Keyboard.press(KEY_LEFT_GUI); //windows key
      Keyboard.releaseAll();
      delay(50);  //pause for 50ms
      Keyboard.print("Autodesk Fusion 360"); //print string into search bar
      delay(50);
      Keyboard.press(KEY_RETURN); //press enter
      Keyboard.releaseAll();
      break;

    //Cura
    case BUTTON_PIN2:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.releaseAll();
      delay(50);
      Keyboard.print("Ultimaker Cura");
      delay(50);
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
      break;

    //Spyder
    case BUTTON_PIN3:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.releaseAll();
      delay(50);
      Keyboard.print("Spyder");
      delay(50);
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
      break;

    //CurseForge (Minecraft)
    case BUTTON_PIN4:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.releaseAll();
      delay(50);
      Keyboard.print("CurseForge");
      delay(50);
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
      break;

    //Guild Wars 2
    case BUTTON_PIN5:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.releaseAll();
      delay(50);
      Keyboard.print("Guild Wars 2");
      delay(50);
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
      break;

    //Notepad++
    case BUTTON_PIN6:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.releaseAll();
      delay(50);
      Keyboard.print("Notepad+++"); //opens notepad, darn you windows and your "best match", put extra "+" so only note++
      delay(50);
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
      break;

    //Arduino IDE
    case BUTTON_PIN7:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.releaseAll();
      delay(50);
      Keyboard.print("Arduino");
      delay(50);
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
      break;

    //Paint.net
    case BUTTON_PIN8:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.releaseAll();
      delay(50);
      Keyboard.print("paint.net");
      delay(50);
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
      break;
  }
}


/*Hidden keys to be used as macros or hotkeys within applications
 * I used the SHIFT + F1-12 combinations
 * since some applications don't like the 
 * built in F13-F24 keys in Windows or the keyboard library
 * (darn Fusion 360)
 */
void doActionHidden(uint8_t button) {
  //You may change inside the case event
  //but do not remove the break or releaseAll functionality at the end
  switch (button) {
    //F13
    case BUTTON_PIN1:
      Keyboard.press(KEY_LEFT_SHIFT);  //secret key shift+F1
      Keyboard.press(KEY_F1);
      Keyboard.releaseAll();
      break;

    //F14
    case BUTTON_PIN2:
      Keyboard.press(KEY_LEFT_SHIFT);  //secret key shift+F2
      Keyboard.press(KEY_F2);
      Keyboard.releaseAll();
      break;

    //F15
    case BUTTON_PIN3:
      Keyboard.press(KEY_LEFT_SHIFT);  //secret key shift+F3
      Keyboard.press(KEY_F3);
      Keyboard.releaseAll();
      break;

    //F16
    case BUTTON_PIN4:
      Keyboard.press(KEY_LEFT_SHIFT);  //secret key shift+F4
      Keyboard.press(KEY_F4);
      Keyboard.releaseAll();
      break;

    //F17
    case BUTTON_PIN5:
      Keyboard.press(KEY_LEFT_SHIFT);  //secret key shift+F5
      Keyboard.press(KEY_F5);
      Keyboard.releaseAll();
      break;

    //F18
    case BUTTON_PIN6:
      Keyboard.press(KEY_LEFT_SHIFT);  //secret key shift+F6
      Keyboard.press(KEY_F6);
      Keyboard.releaseAll();
      break;

    //F19
    case BUTTON_PIN7:
      Keyboard.press(KEY_LEFT_SHIFT);  //secret key shift+F7
      Keyboard.press(KEY_F7);
      Keyboard.releaseAll();
      break;

    //F20
    case BUTTON_PIN8:
      Keyboard.press(KEY_LEFT_SHIFT);  //secret key shift+F8
      Keyboard.press(KEY_F8);
      Keyboard.releaseAll();
      break;
  }
}
