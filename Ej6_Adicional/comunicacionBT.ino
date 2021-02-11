void BTread(int* modoFuncionamiento, float* radioCurv, double *velRef) {
  //Esta función se ejecutará cuando haya 3 bytes listos para leer
  // X00 -> X indica el modo; y 00 la distancia
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
      Serial.println("Max Distance changed to -->  " + String(*radioCurv)); //Debug
      break;
    case 'r':
      phi = posx = posy = 0; //Inicializamos a 0
      break;
    case '0':
      *modoFuncionamiento = 0;
      Serial.println("Mode changed to --> MODE0 (Inactive)"); //Debug
      break;
    case '1':
      *modoFuncionamiento = 1;
      *radioCurv = (float)SerialIn;
      Serial.println("Mode changed to --> MODE1 \t with max Dist: " + String(*radioCurv)); //Debug
      break;
    case '2':
      *modoFuncionamiento = 1;
      *radioCurv = -(float)SerialIn;
      Serial.println("Mode changed to --> MODE2 \t with max Dist: " + String(*radioCurv)); //Debug
      break;
    case '3':
      *modoFuncionamiento = 2;
      *radioCurv = (double)SerialIn;
      Serial.println("Mode changed to --> MODE3 \t with max Dist: " + String(*radioCurv)); //Debug
      break;
    case '4':
      *modoFuncionamiento = 2;
      *radioCurv = -(double)SerialIn;
      Serial.println("Mode changed to --> MODE3 \t with max Dist: " + String(*radioCurv)); //Debug
      break;
    case '5':
      *modoFuncionamiento = 3;
      break;
  }

}

void telemetria(int t, double distL, double distR, double ref, int mode, int pwmL, int pwmR) {
  if (puttyReady) //Empezamos solo cuando el putty esté listo (Debemos asegurarnos de tener el mismo numero de columnas en todas las filas)
    Serial2.println(String(t) + " " + String(distL) + " " + String(distR) + " " + String(ref) + " " + String(mode) + " " + String(pwmL) + " " + String(pwmR));
}
void telemetriaOD(int t, float px, float py, float velx, int vely, int velL, int velR, int pwmL, int pwmR, float ang) {
  if (puttyReady) //Empezamos solo cuando el putty esté listo (Debemos asegurarnos de tener el mismo numero de columnas en todas las filas)
    Serial2.println(String(t) + " " + String(px) + " " + String(py) + " " + String(velx) + " " + String(vely) + " " + String(velL) + " " + String(velR) + " " + String(pwmL) + " " + String(pwmR) + " " + String(ang));
}
void telemetriaObj(int t, float px, float py, float ang,float dPto) {
  if (puttyReady) //Empezamos solo cuando el putty esté listo (Debemos asegurarnos de tener el mismo numero de columnas en todas las filas)
    Serial2.println(String(t) + " " + String(px) + " " + String(py) + " " + String(ang) + " " + String(dPto));
}
