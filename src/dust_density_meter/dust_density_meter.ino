/*
version : v1.0.0-alpha

MIT License

Copyright (c) 2019 nulLeeKH <i_am@nulleekh.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#include <LiquidCrystal.h>

#define dustPin A0
#define ledPower 2
//set pins for dustsensor

#define samplingTime 280
#define deltaTime 40
#define sleepTime 9680
//set measuring time for sensor

double dustVal = 0, dustDensity = 0, dustSum = 0, dustAvg = 0, dusts[100];
//declare variables

int count = 0;
//sample count

LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
//set pins for liquidcrystal

void setup() {
    pinMode(ledPower, OUTPUT);
    //set led pin of sensor

    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("Dust density");
    lcd.setCursor(0, 1);
    lcd.print("-----------ug/m3");
    //initial settings for liquidcrystal
}

void loop() {
    digitalWrite(ledPower, LOW);
    delayMicroseconds(samplingTime);
    dustVal = analogRead(dustPin);
    delayMicroseconds(deltaTime);
    digitalWrite(ledPower, HIGH);
    delayMicroseconds(sleepTime);
    //measure dust density in air

    dustDensity = 0.17 * (dustVal * 0.0049) - 0.1;
    dusts[count] = dustDensity * 1000;
    Serial.println(dusts[count]);
    //change the unit to ug/m^2


    if (count == 99) {
        for (int i = 0; i < 100; i++) {
            dustSum += dusts[i];
        } //sum all sample

        dustAvg = dustSum / 100; //save avarage dust density

        lcd.setCursor(0, 1); //print average dust density

        if (dustAvg<540) {
            lcd.print("-----------ug/m3");
            lcd.setCursor(0, 1);
            lcd.print(dustAvg);
        } //if dust density is in nomal range, print it
        else {
            lcd.print("ERROR------ug/m3");
        } //if dust density is too high, print ERROR

        dustSum = 0; dustAvg = 0; //reset temporary variables
        count = -1; //reset count of sample
    } //calculate average dust density once per second
    ++count;

    delay(10);
    //collect data once per 0.01 seconds
}
