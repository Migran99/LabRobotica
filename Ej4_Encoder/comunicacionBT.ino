void BTread(int* modoFuncionamiento, double* velRef) {
  //Esta función se ejecutará cuando haya 4 bytes listos para leer
  // X000 -> X indica el modo; y 000 la velocidad de referencia
  int SerialIn = 0;
  int dataIn;
  int mod;
  mod = Serial2.read();
  Serial.println(mod);
  while (Serial2.available() > 0) {
    dataIn = int(Serial2.read());
    Serial.println(dataIn);
    if (dataIn >= 48 && dataIn <= 57) SerialIn = 10 * SerialIn + dataIn - 48;
  }

  switch (mod)  {
    case 'b': //Begin communication
      puttyReady = true;
      Serial.println("BEGIN"); //Debug
      break;
    case 'e': //End Communication
      puttyReady = false;
      Serial.println("END"); //Debug
      break;
    case 's': //Set velRef
      *velRef = (double)SerialIn;
      Serial.println("Max Distance changed to -->  " + String(*velRef)); //Debug
      break;
    case '0':
      *modoFuncionamiento = 0; 
      Serial.println("Mode changed to --> MODE0 (Inactive)"); //Debug
      break;
    case '1': //Velocidad de refencia positiva
      *modoFuncionamiento = 1; 
      *velRef = (double)SerialIn;
      Serial.println("Mode changed to --> MODE1 \t with max Dist: " + String(*velRef)); //Debug
      break;
    case '2': //Velocidad de referencia negativa
      *modoFuncionamiento = 1; 
      *velRef = -(double)SerialIn;
      Serial.println("Mode changed to --> MODE2 \t with max Dist: " + String(*velRef)); //Debug
      break;
  }
}

void telemetria(int t,double distL, double distR, double ref, int mode, int pwmL, int pwmR){
  if (puttyReady) //Empezamos solo cuando el putty esté listo (Debemos asegurarnos de tener el mismo numero de columnas en todas las filas)
    Serial2.println(String(t) + " " + String(distL) + " " + String(distR) + " " + String(ref) + " " + String(mode) + " " + String(pwmL) + " " + String(pwmR));
}
