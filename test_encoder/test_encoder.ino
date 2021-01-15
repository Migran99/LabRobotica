float medida;
void setup() {
  Serial.begin(9600);
  pinMode(4,INPUT);
  
}

void loop() {
  medida = analogRead(A0);
  if(medida>0)Serial.println('1');
  else Serial.println('0');
  delay(100);
}
