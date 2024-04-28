#include <Arduino.h>

const int r = 12, g = 14, b = 27;

void setup()
{
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);
}

void brillar(int red, int green, int blue)
{
  analogWrite(r, red);
  analogWrite(g, green);
  analogWrite(b, blue);
}

void loop()
{
  // hacer un degradado entre colores
  for (int i = 0; i < 255; i++)
  {
    brillar(i, 0, 0);
    delay(5);
  }
  for (int i = 0; i < 255; i++)
  {
    brillar(255, i, 0);
    delay(5);
  }
  for (int i = 0; i < 255; i++)
  {
    brillar(255 - i, 255, 0);
    delay(5);
  }
  for (int i = 0; i < 255; i++)
  {
    brillar(0, 255, i);
    delay(5);
  }
  for (int i = 0; i < 255; i++)
  {
    brillar(0, 255 - i, 255);
    delay(5);
  }
  for (int i = 0; i < 255; i++)
  {
    brillar(i, 0, 255);
    delay(5);
  }
  for (int i = 0; i < 255; i++)
  {
    brillar(255, i, 255);
    delay(5);
  }
  for (int i = 0; i < 255; i++)
  {
    brillar(255-i, 255-i, 255-i);
    delay(5);
  }
}