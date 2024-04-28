#include <Arduino.h>

int buzzer = 13;
void setup()
{
  // initialize the buzzer pin as an output
  pinMode(buzzer, OUTPUT);
}
void loop()
{
  //define a char variable i 
  unsigned char i;
  while (1)
  {
    // output an frequency 
    for (i = 0; i < 150; i++)
    {
      //When the buzzer is high, it sounds
      digitalWrite(buzzer, HIGH);
      delay(1); // wait for 1ms
      digitalWrite(buzzer, LOW);
      delay(1); // wait for 1ms
    }
    // output another frequency
    for (i = 0; i < 100; i++)
    {
      digitalWrite(buzzer, HIGH);
      delay(2); // wait for 2ms
      digitalWrite(buzzer, LOW);
      delay(2); // wait for 2ms
    }
  }
}