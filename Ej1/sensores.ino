float ping(int TrgPin, int EchoPin) {
  long duration;
  float distanceCm;

  digitalWrite(TrgPin, LOW);
  delayMicroseconds(4);
  digitalWrite(TrgPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrgPin, LOW);

  duration = pulseIn(EchoPin, HIGH);

  distanceCm = (float)duration * 10.0 / 292.0 / 2.0;  //convertimos a distancia en cm
  return distanceCm;
}
