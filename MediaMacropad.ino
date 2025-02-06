#include <Keyboard.h>

int switch1Pin = 3;
int switch2Pin = 4;
int switch3Pin = 2;
int switch4Pin = 1; // variables for which switch corresponds to which pin. CHANGE THESE to whatever pins your switches are connected to
bool switch1State = false;
bool switch2State = false;
bool switch3State = false;
bool switch4State = false;

unsigned long currentTime = 0; // used to create a polling rate without using delay()
int releaseDebounce = 12;
int switch1Count = 0;
int switch2Count = 0;
int switch3Count = 0;
int switch4Count = 0;
bool switch1Pressed = false;
bool switch2Pressed = false;
bool switch3Pressed = false;
bool switch4Pressed = false; // all above used for debouncing

long potValue = 0;
long prevPotValue = 0; // both set to zero to initialise so that they start even

void setup() {
  pinMode(switch1Pin, INPUT_PULLUP);
  pinMode(switch2Pin, INPUT_PULLUP);
  pinMode(switch3Pin, INPUT_PULLUP);
  pinMode(switch4Pin, INPUT_PULLUP);

  Serial.begin(9600);
  Keyboard.begin();
  potValue = analogRead(A6); // A6 is the pin that the pot is connected to. In this case, for the Teensy 4.0, this corresponds to the pin labelled 20 on the board
  prevPotValue = analogRead(A6); // Change all instances of A6 to whichever analog pin the pot is connected to
}

void loop() {
  while(true) {
    currentTime = micros();
    if(currentTime % 500 < 50) { // in other words, a polling rate of 10000hz, with leeway to make sure the function is triggered every hz.
      checkPins();
    }
  }
}

void checkPins() {
  switch1State = digitalRead(switch1Pin);
  switch2State = digitalRead(switch2Pin);
  switch3State = digitalRead(switch3Pin);
  switch4State = digitalRead(switch4Pin); // get the current status of each pin, HIGH (1) when depressed and LOW (0) when pressed
  potValue = analogRead(A6);

  if(!switch1State) { // when the switch is pressed down
    pressSwitch1();
    switch1Count = 0; // reset the counter
  } else {
    switch1Count++;
    if(switch1Count >= releaseDebounce) { // if the switch has been unheld for 12 polling cycles, release
      releaseSwitch1();
    }
  }

  if(!switch2State) {
    pressSwitch2();
    switch2Count = 0;
  } else {
    switch2Count++;
    if(switch2Count >= releaseDebounce) {
      releaseSwitch2();
    }
  }

  if(!switch3State) {
    pressSwitch3();
    switch3Count = 0;
  } else {
    switch3Count++;
    if(switch3Count >= releaseDebounce) {
      releaseSwitch3();
    }
  }

  if(!switch4State) {
    pressSwitch4();
    switch4Count = 0;
  } else {
    switch4Count++;
    if(switch4Count >= releaseDebounce) {
      releaseSwitch4();
    }
  }

  if(potValue-prevPotValue < -10.24) { // if the volume has been increased, and by enough such that it warrants a change by one level (of a hundred)
    increaseVol();
  }
  else if(potValue-prevPotValue > 10.24) {
    decreaseVol();
  }
  // otherwise dont do anything
}

void pressSwitch1() {
  if(!switch1Pressed) { // executes when the switch isn't currently pressed to prevent sending signals every polling cycle the key is held
    Keyboard.press(KEY_MEDIA_PREV_TRACK);
    switch1Pressed = true;
  }
}

void releaseSwitch1() {
  if(switch1Pressed) { // prevents held release signals
    Keyboard.release(KEY_MEDIA_PREV_TRACK);
    switch1Pressed = false;
  }
}

void pressSwitch2() {
  if(!switch2Pressed) {
    Keyboard.press(KEY_MEDIA_NEXT_TRACK);
    switch2Pressed = true;
  }
}

void releaseSwitch2() {
  if(switch2Pressed) {
    Keyboard.release(KEY_MEDIA_NEXT_TRACK);
    switch2Pressed = false;
  }
}

void pressSwitch3() {
  if(!switch3Pressed) {
    Keyboard.press(KEY_MEDIA_PLAY_PAUSE);
    switch3Pressed = true;
  }
}

void releaseSwitch3() {
  if(switch3Pressed) {
    Keyboard.release(KEY_MEDIA_PLAY_PAUSE);
    switch3Pressed = false;
  }
}

void pressSwitch4() {
  if(!switch4Pressed) {
    Keyboard.press(KEY_MEDIA_STOP);
    switch4Pressed = true;
  }
}

void releaseSwitch4() {
  if(switch4Pressed) {
    Keyboard.release(KEY_MEDIA_STOP);
    switch4Pressed = false;
  }
}

void increaseVol() {
  prevPotValue = potValue; // set to the current value so that we can go again for the next rotation
  Keyboard.press(KEY_MEDIA_VOLUME_INC);
  Keyboard.release(KEY_MEDIA_VOLUME_INC); // only press once, since this is a potentiometer and not a switch/button
}

void decreaseVol() {
  prevPotValue = potValue;
  Keyboard.press(KEY_MEDIA_VOLUME_DEC);
  Keyboard.release(KEY_MEDIA_VOLUME_DEC);
}