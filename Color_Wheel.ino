// 2019.9.29
// RGB controller using analog joystick
// Based on angle of joystick

// Arduino pin layout
const int SW_pin = 2;
const int X_pin = 0;
const int Y_pin = 1;
const int blue = 3;
const int green = 5;
const int red = 6;
void setup()
{
    pinMode(SW_pin, INPUT);
    digitalWrite(SW_pin, HIGH);
    Serial.begin(9600);
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(blue, OUTPUT);
}
int redValue, greenValue, blueValue;
int x, y;
float theta;
const float pi = 3.14;
bool stickPress = bool lastPress = bool onoff = false;
int clickCount = 0; //click 1 on. Click 2 color. Click 3 off.
float tilt;
float r(int x, int y) {
    return sqrt(pow(x, 2) + pow(y, 2));
}
void loop() {
    lastPress = stickPress;
    stickPress = digitalRead(SW_pin);
    //Lock or Turn off
    if (lastPress == true && stickPress == false) {
        clickCount++;
    }
    if (clickCount == 3) {
        digitalWrite(red, LOW);
        digitalWrite(green, LOW);
        digitalWrite(blue, LOW);
        for (int i = 0; i < 3; i++) {
            delay(50);
            digitalWrite(red, HIGH);
            delay(75);
            digitalWrite(red, LOW);
        }
        clickCount = 0;
    }
    else if (clickCount == 1) {
        x = analogRead(X_pin) - 508;
        y = (511 - analogRead(Y_pin));
        tilt = r(x, y) / r(500, 500);

        //Rotation
        if (x > 0 && y < 0)
            theta = 2 * pi + atan(float(y) / float(x));
        else if (x < 0)
            theta = atan(float(y) / float(x)) + pi;
        else if (x == 0 && y > 0)
            theta = pi / 2;
        else if (x == 0 && y < 0)
            theta = 3 * pi / 2;
        else
            theta = atan(float(y) / float(x));
        //Red
        if (theta < 2 * pi / 3)
            redValue = int(255 * cos(1.5 * theta / 2));
        else if (theta > 4 * pi / 3) 
            redValue = int(255 * cos(1.5 * (theta - 2 * pi) / 2));
        else redValue = 0;
        //Green
        if (theta > 0 && theta < 4 * pi / 3)
            greenValue = int(255 * cos(1.5 / 2 * (theta - 2 * pi / 3)));
        else greenValue = 0;
        //Blue
        if (theta > 2 * pi / 3 && theta < 2 * pi)
            blueValue = int(255 * cos(1.5 / 2 * (theta - 4 * pi / 3)));
        else blueValue = 0;
        
        analogWrite(red, redValue * tilt);
        analogWrite(green, greenValue * tilt);
        analogWrite(blue, blueValue * tilt);
    }
}
