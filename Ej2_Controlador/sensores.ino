float ping(int TrgPin, int EchoPin) {
  long duration;
  float distanceCm;

  digitalWrite(TrgPin, LOW);
  delayMicroseconds(4);
  digitalWrite(TrgPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrgPin, LOW);

  duration = pulseIn(EchoPin, HIGH, (unsigned long) 20000); //Timeout de 20 ms = 20000 micro segundos

  distanceCm = (float)duration * 10.0 / 292.0 / 2.0;  //convertimos a distancia en cm
  return distanceCm;
}

int filtro(float* distAnt, float* distAct, float pingAct, float maxDiff){ //Filtro para el ultrasonido. Devuelve 1 si medida valida, 0 si no.
  if(abs(pingAct - *distAnt) <= maxDiff * *distAnt){
      *distAct = pingAct;
      return 1;
    }
  else{
      *distAct = *distAct;
      return 0;
    }
  *distAnt = pingAct;
}
