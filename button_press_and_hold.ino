// button_press_and_hold -- sketch for registering button presses and button holds -- easily expanded upon
//CONSTANTS:
  const byte BUT_ON = LOW;   //easier to switch this to pulldown-type circuit
  const byte BUT_OFF = HIGH;  //ditto
  const byte LEFT_BUTTON = 2; //left/down button
  const byte RIGHT_BUTTON = 3;  //right/up button
  const byte DEBOUNCE_DELAY = 85;   //(in millis)the debounce time, set this to your needs. Higher = less flicker. Too high affects whether your press was read at all. It all depends on your individual buttons
  const unsigned int BUTTON_HOLD_TIME = 1500; //(in millis)time needed for button press to be considered long-pressed(1.5sec)                                  
  const byte NUM_BUT = 2;
  enum  { LEFT, RIGHT };

//button states
  volatile byte leftButtonState = BUT_ON;  //these are set LOW because pull-down circuit
  volatile byte rightButtonState = BUT_ON; //ditto. You can switch the #define BUT_ON/BUT_OFF above for pull-up

  byte prevButtonState[NUM_BUT];
 
//base reading for determing if long-press time has been met
  unsigned long lastButtonTime = 0;

//debounce variables/functions
  unsigned long lastDebounceTime = 0; //used to set a base time (in millis) for our debounce
  unsigned long debouncer();  //sets debounce timer

void setup() 
{
  Serial.begin(9600);//lets get that serial monitor running!
  pinMode(LEFT_BUTTON, INPUT_PULLUP); //change this to INPUT if you're not using the internal pullup
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);//change this to INPUT if you're not using the internal pullup
}

void loop() 
{
  byte i;
  //set our previous states to OFF
  for (i = 0; i < NUM_BUT; i++)
  {
    prevButtonState[i] = BUT_OFF;
  }
  
  if (leftButtonState == BUT_OFF && rightButtonState == BUT_OFF)  //if buttons NOT pressed...
  {
    lastDebounceTime = millis(); //...reset the debouncing timer and
    lastButtonTime = millis();  //...reset the button_hold timer
  }
  //read both button states
  leftButtonState = digitalRead(LEFT_BUTTON);
  rightButtonState = digitalRead(RIGHT_BUTTON);
  
  //make sure we've hit debounce time & check if left button has been pressed..
  if (debouncer() >= DEBOUNCE_DELAY && (leftButtonState == BUT_ON || rightButtonState == BUT_ON))
  {
    while (leftButtonState == BUT_ON || rightButtonState == BUT_ON)//as long as atleast one button is still pressed lets loop..
    {
      //read both button states again and store in prevButtonState array
      prevButtonState[LEFT] = leftButtonState;
      prevButtonState[RIGHT] = rightButtonState;
      leftButtonState = digitalRead(LEFT_BUTTON); 
      rightButtonState = digitalRead(RIGHT_BUTTON);
      //if LEFT_BUTTON was pressed but is no longer pressed and its been less than HOLD_TIME...
      if (leftButtonState != prevButtonState[LEFT] && (millis() - lastButtonTime) < BUTTON_HOLD_TIME)
      {
        Serial.println("LEFT_BUTTON press"); //obviously place your code here
      }else if (rightButtonState != prevButtonState[RIGHT] && (millis() - lastButtonTime) < BUTTON_HOLD_TIME)//same for RIGHT_BUTTON
      {
        Serial.println("RIGHT_BUTTON press");//replace with your code
      }
      if (leftButtonState == BUT_ON && rightButtonState == BUT_OFF && (debouncer() >= BUTTON_HOLD_TIME))//if we've eclipsed the hold time for left button...
      {
        Serial.println("LEFT_BUTTON hold");  //again, place your code here
        while (leftButtonState == BUT_ON) //stay in loop until button released so as not to repeat press.
        {
          leftButtonState = digitalRead(LEFT_BUTTON); 
        }
      }else if (leftButtonState == BUT_OFF && rightButtonState == BUT_ON && (debouncer() >= BUTTON_HOLD_TIME))//or if we've eclipsed the hold time for right button...
      {
        Serial.println("RIGHT_BUTTON hold");  //again, place your code here
        while (rightButtonState == BUT_ON)  //stay in loop until button released so as not to repeat press
        {
          rightButtonState = digitalRead(RIGHT_BUTTON);
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

