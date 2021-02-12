//Mismo que en ejercicio anterior
void moveRobot(int vel1, int vel2) {
  int velMin = velUmbral;
  int m1, m2;
  analogWrite(ENA, abs(vel1));
  analogWrite(ENB, abs(vel2));

//CONTROL DIRECCION
  if (vel1 < -velMin) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    m1 = -1;
  }
  else if (vel1 > velMin) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    m1 = 1;
  }
  else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    m1 = 0;
  }

  if (vel2 < -velMin) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    m2 = -1;
  }
  else if (vel2 > velMin) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    m2 = 1;
  }
  else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    m2 = 0;
  }

// CAMBIO MODO TELEMETRIA
  if(m1 * m2 == 1){
    if(m1 > 1) mode = 1;
    else mode = 2;  
  }
  else{
    if(m1 == 0 && m2 == 0) mode = 0;
    else if(m1 == 1 || m2 == -1) mode = 3;
    else if(m2 == 1 || m1 == -1) mode = 4; 
  }

}
