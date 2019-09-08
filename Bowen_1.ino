int led = 3;                      /*color of the led: the red led, connected to pin 3, glows if the pushbutton is not pressed;
                                                      the green led, connected to pin 5, glows if the pushbutton is pressed;*/
bool brighten = true;             //whether there is need to turn on a led
bool fade = false;                //whether there is need to turn off a led
bool initial = true;              //initially, only need to brighten but no need to fade
int prev_potentiometer = 0;       //the previous brightness of the led, determined by the potentiometer
bool prev_photoresistor = false;  //the previous brightness around the photoresistor

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

void loop() {
  // read the input on analog pin 0, which measures potentiometer
  int potentiometer = analogRead(A0);
  Serial.print("potentiometer: ");
  Serial.println(potentiometer);

  // read the input on analog pin 1, which measures photoresistor
  int photoresistor = analogRead(A1);
  Serial.print("photoresistor: ");
  Serial.println(photoresistor);

  // read the input pin 2, which measures pushbutton
  int pushbutton = digitalRead(2);
  Serial.print("pushbutton: ");
  Serial.println(pushbutton);
  Serial.println();

  //determine the led color and whether to turn on/off led
  if (pushbutton == 1) {      //if unpressed (default)
    if (led != 3) {           //if green light was glowing
      led = 3;                //set the color to red
      brighten = true;        //red light will be on
      fade = true;            //green light will be off
      prev_potentiometer = 0; //because red light was not glowing, reset the initial brightness to zero
    } else {                  //if red light was glowing
      brighten = false;
      fade = false;           //keep glowing
    }
  } else {                    //if pressed
    if (led != 5) {           //if red light was glowing
      led = 5;                //set the color to green
      brighten = true;        //green light will be on
      fade = true;            //red light will be off
      prev_potentiometer = 0; //because green light was not glowing, reset the initial brightness to zero
    } else {                  //if green light was glowing
      brighten = false;
      fade = false;           //keep glowing
    }
  }

  analogWrite(3, 0);
  analogWrite(5, 0);          //set both lights off
  if (photoresistor > 200) {  //if it is dark enough around the photoresistor
    if (initial) {            //if it is the initial state, both leds are off
      brighten = true;        //now one will be turned on
      fade = false;           //the other is still off
      initial = false;
    }
    if (prev_photoresistor == false) {  //if it was not dark previously, both leds were off
      brighten = true;                  //now one will be turned on
      prev_potentiometer = 0;           //with initial brightness of zero
      prev_photoresistor = true;        //update the current state
    }
    
    // change the analog out value
    if (!brighten) {                    //no need to change led state
      int a = map(potentiometer, 0, 1023, 0, 255);
      analogWrite(led, a);              //simply keep glowing
    } else {                            //need to light the led
      if (fade) {                       //if needed, let the other fade first
        for (int timer = potentiometer; timer >= 0; timer --) {
          int b = map(timer, 0, 1023, 0, 255);
          if (led == 3) {
            analogWrite(5, b);
          } else {
            analogWrite(3, b);
          }
          delay(2);                     //change here: make the fading effect more obvious
        }
      }
      for (int timer = prev_potentiometer; timer < potentiometer; timer ++) { //then light the led
        int a = map(timer, 0, 1023, 0, 255);
        analogWrite(led, a);
        delay(3);                       //change here: make the brightening effect more obvious
      }
    }
  } else {
    if (prev_photoresistor == true) {   //if it was dark previously, one led was glowing
      for (int timer = potentiometer; timer >= 0; timer --) {                 //let it fade
        int b = map(timer, 0, 1023, 0, 255);
        analogWrite(led, b);
        delay(2);                       //change here: make the fading effect more obvious
      }
      prev_photoresistor = false;       //update the current state
    }
  }
  prev_potentiometer = potentiometer;   //update the current brightness of the led
}
