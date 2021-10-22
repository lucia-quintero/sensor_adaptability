int luz = 0;
float lux = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  luz = analogRead (A0);
  lux = (luz *5999)/1023;
  
  delay (1000);
  Serial.println(luz,DEC);
}
