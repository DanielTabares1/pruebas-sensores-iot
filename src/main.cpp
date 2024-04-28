#include <Arduino.h>

int Led = 13;
// define switch port
int avoidance = 12;
// define digital variable val
int val;
void setup()
{
  // define LED as a output port
  pinMode(Led, OUTPUT);
  // define switch as a output port
  pinMode(avoidance, INPUT);
  Serial.begin(9600);
}
void loop()
{
  // read the value of the digital interface 3 assigned to val
  val = analogRead(avoidance);
  Serial.println(val);
  // when the switch sensor have signal, LED blink
  if (val) //
  {
    digitalWrite(Led, HIGH);
  }
  else
  {
    digitalWrite(Led, LOW);
  }
  delay(200);
}