// button_press_and_hold -- sketch for registering button presses and button holds -- easily expanded upon
//CONSTANTS:
  const byte DEBOUNCE_DELAY = 85;   //(in millis)the debounce time, set this to your needs. Higher = less flicker. Too high affects whether your press was read at all. It all depends on your individual buttons
  const unsigned short BUTTON_HOLD_TIME = 1000; //(in millis)time needed for button press to be considered long-pressed(1.0sec)                                  
  const byte NUM_BUT = 2; //this represents the number of buttons we're dealing with

  enum { BUT_ON, BUT_OFF }; //using pull-up in this example so BUT_ON = 0, BUT_OFF = 1. use enum { BUT_OFF, BUT_ON } for pulldown type circuit
  enum { LEFT, RIGHT };     //these are our button names. Can just as easily replace these with numbers.
  //button pins -- add as many of these as you have buttons. I needed two.
  const byte BUTTONS[NUM_BUT] = { 2, 3 }; //add as many buttons as you'd like, as well as their corresponing pins
  
//button states -- I have them as volatile as my final use involves attaching interrupts
  volatile byte buttonState[NUM_BUT] = { BUT_ON, BUT_ON }; //set to LOW because using internal pullup, change to HIGH if using Pulldown
  volatile byte prevButtonState[NUM_BUT];
 
//base reading for determing if long-press time has been met
  unsigned long lastButtonTime = 0;

//debounce variables/functions
  unsigned long lastDebounceTime = 0; //used to set a base time (in millis) for our debounce
  inline unsigned long debouncer();  //prototype for function that sets debounce timer

void setup() 
{
  Serial.begin(9600);//lets get that serial monitor running!
  byte init;
  for (init = 0; init < NUM_BUT; init++)
   pinMode(BUTTONS[init], INPUT_PULLUP); //change this to INPUT if you're not using the internal pullup
}

void loop() 
{
  byte i;
  //set our previous states to OFF
  for (i = 0; i < NUM_BUT; i++)
  {
    prevButtonState[i] = BUT_OFF;
  }
  
  if (buttonState[LEFT] == BUT_OFF && buttonState[RIGHT] == BUT_OFF)  //if buttons NOT pressed...
  {
    lastDebounceTime = millis(); //...reset the debouncing timer and
    lastButtonTime = millis();  //...reset the button_hold timer
  }
  //read both button states
  buttonState[LEFT] = digitalRead(BUTTONS[LEFT]);
  buttonState[RIGHT] = digitalRead(BUTTONS[RIGHT]);
  
  //make sure we've hit debounce time & check if left button has been pressed..
  if (debouncer() >= DEBOUNCE_DELAY && (buttonState[LEFT] == BUT_ON || buttonState[RIGHT] == BUT_ON))
  {
    while (buttonState[LEFT] == BUT_ON || buttonState[RIGHT] == BUT_ON)//as long as atleast one button is still pressed lets loop..
    {
      //read both button states again and store in prevButtonState array
      prevButtonState[LEFT] = buttonState[LEFT];
      prevButtonState[RIGHT] = buttonState[RIGHT];
      buttonState[LEFT] = digitalRead(BUTTONS[LEFT]); 
      buttonState[RIGHT] = digitalRead(BUTTONS[RIGHT]);
      //if BUTTONS[LEFT] was pressed but is no longer pressed and its been less than HOLD_TIME...
      if (buttonState[LEFT] != prevButtonState[LEFT] && (millis() - lastButtonTime) < BUTTON_HOLD_TIME)
      {
        Serial.println("BUTTONS[LEFT] press"); //obviously place your code here
      }else if (buttonState[RIGHT] != prevButtonState[RIGHT] && (millis() - lastButtonTime) < BUTTON_HOLD_TIME)//same for BUTTONS[RIGHT]
      {
        Serial.println("BUTTONS[RIGHT] press");//replace with your code
      }
      if (buttonState[LEFT] == BUT_ON && buttonState[RIGHT] == BUT_OFF && (debouncer() >= BUTTON_HOLD_TIME))//if we've eclipsed the hold time for left button...
      {
        Serial.println("BUTTONS[LEFT] hold");  //again, place your code here
        while (buttonState[LEFT] == BUT_ON) //stay in loop until button released so as not to repeat press.
        {
          buttonState[LEFT] = digitalRead(BUTTONS[LEFT]); 
        }
      }else if (buttonState[LEFT] == BUT_OFF && buttonState[RIGHT] == BUT_ON && (debouncer() >= BUTTON_HOLD_TIME))//or if we've eclipsed the hold time for right button...
      {
        Serial.println("BUTTONS[RIGHT] hold");  //again, place your code here
        while (buttonState[RIGHT] == BUT_ON)  //stay in loop until button released so as not to repeat press
        {
          buttonState[RIGHT] = digitalRead(BUTTONS[RIGHT]);
        }
      }
    }
  }
}

//this function is to ensure that one button press is registered as only one button press.
unsigned long debouncer()
{
  return millis() - lastDebounceTime;
}
