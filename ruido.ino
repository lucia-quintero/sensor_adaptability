int sensorPin = A5; 
int sensorValue = 0; // variable to store the value coming from the sensor
int luz = 0;
float voltage = 0;

void setup ()
{
 Serial.begin (9600);
}
void loop ()
{
 sensorValue = analogRead (sensorPin);
 voltage = (sensorValue * 5.0) / 1023.0;
  
 luz = analogRead (A1);
 delay (100);
 Serial.println (sensorValue);
 //Serial.println(voltage);
 //Serial.print(",");
 //Serial.println(luz); 
 
}
