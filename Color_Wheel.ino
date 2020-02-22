//2019.9.29
// RGB controller using analog joystick
//Based on angle of joystick


// Arduino pin numbers
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output
const int blue = 3;
const int green = 5;
const int red = 6;
void setup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
}

// define variables
int redValue;
int greenValue;
int blueValue;
double redDoub, blueDoub, greenDoub;
int x;
int y;
double theta;
const double pi = 3.14;
bool stickPress = false;
bool lastPress = false;
bool onoff = false;
int clickCount = 0; //click 1 turns on. Click 2 sets color. Click 3 turns off.
double tilt;
double r(int x, int y) {
  return sqrt(pow(x, 2) + pow(y, 2));
}

// main loop
void loop() {
  lastPress = stickPress;
  stickPress = digitalRead(SW_pin);

  //Lock or Turn off
  if (lastPress == true && stickPress == false) {
    clickCount++;
  }
    if (clickCount == 3)
    {
      Serial.println("Count is 3");
      digitalWrite(red, LOW);
      digitalWrite(green, LOW);
      digitalWrite(blue, LOW);
      delay(50);
      digitalWrite(red, HIGH);
      delay(75);
      digitalWrite(red, LOW);
      delay(50);
      digitalWrite(red, HIGH);
      delay(75);
      digitalWrite(red, LOW);
      delay(50);
      digitalWrite(red, HIGH);
      delay(75);
      digitalWrite(red, LOW);

      clickCount = 0;

    }
    else if (clickCount == 2 || clickCount == 0)
      Serial.println("Count is 2 or zero");
    else if (clickCount == 1) {
      Serial.println("Count is 1");
      x = analogRead(X_pin) - 508;
      y = (511 - analogRead(Y_pin));
      tilt = r(x, y) / r(500, 500);

      //Calculate theta, taking critical points and range of atan into consideration
      if (x > 0 && y < 0)
        theta = 2 * pi + atan(double(y) / double(x));
      else if (x < 0)
        theta = atan(double(y) / double(x)) + pi;
      else if (x == 0 && y > 0)
        theta = pi / 2;
      else if (x == 0 && y < 0)
        theta = 3 * pi / 2;
      else
        theta = atan(double(y) / double(x));

      if (theta < 2 * pi / 3) {
        redDoub = 255 * cos(1.5 * theta / 2);
        redValue = int(redDoub);
      }
      else if (theta > 4 * pi / 3) {
        redDoub =  255 * cos(1.5 * (theta - 2 * pi) / 2);
        redValue = int(redDoub);
      }
      else {
        redValue = 0;
      }

      if (theta > 0  && theta < 4 * pi / 3) {
        greenDoub = 255 * cos(1.5 / 2 * ( theta - 2 * pi / 3));
        greenValue = int(greenDoub);
      }
      else {
        greenValue = 0;
      }
      if (theta > 2 * pi / 3 && theta < 2 * pi) {
        blueDoub = 255 * cos(1.5 / 2 * ( theta - 4 * pi / 3));
        blueValue = int(blueDoub);
      }
      else {
        blueValue = 0;
      }

          analogWrite(red, redValue * tilt);
    analogWrite(green, greenValue * tilt);
    analogWrite(blue, blueValue * tilt);
    }

 

  //delay(500);
}




