#include "Wire.h"

int digit_pins[4] = { 7, 6, 5, 4 };
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

int seven_segment(int number, int dot)
{
    switch (number)
    {
    //                ABCDEFGd
    case 0: return ~(B11111100 | dot);
    case 1: return ~(B01100000 | dot);
    case 2: return ~(B11011010 | dot);
    case 3: return ~(B11110010 | dot);
    case 4: return ~(B01100110 | dot);
    case 5: return ~(B10110110 | dot);
    case 6: return ~(B10111110 | dot);
    case 7: return ~(B11100000 | dot);
    case 8: return ~(B11111110 | dot);
    case 9: return ~(B11100110 | dot);
    }
}

int get_temperature_fahrenheit()
{
    Wire.beginTransmission(72);
    Wire.write(0x00);
    Wire.endTransmission();
    Wire.requestFrom(72, 2);
    Wire.endTransmission();
    return (int)(((float)((Wire.read() << 4) | (Wire.read() >> 4)) * 0.0625 * 9 / 5 + 32) * 100);
}

void setup()
{
    for (int i = 0; i < sizeof(digit_pins) / sizeof(int); ++i)
        pinMode(digit_pins[i], OUTPUT);
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    Wire.begin();
}

void loop()
{
    for (int i = 0, temp = get_temperature_fahrenheit(); i < sizeof(digit_pins) / sizeof(int); ++i, temp /= 10)
    {
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, LSBFIRST, seven_segment(temp % 10, (i == 2) ? 1 : 0));  
        digitalWrite(latchPin, HIGH);
        digitalWrite(digit_pins[i], HIGH);
        delay(5);
        digitalWrite(digit_pins[i], LOW);
    }
}
