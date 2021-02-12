void BTread(int* modoFuncionamiento, float* radioCurv, double *velRef) {
  //Esta función se ejecutará cuando haya 4 bytes listos para leer
  // X000 -> X indica el modo; y 000 la distancia
  int SerialIn = 0;
  int dataIn;
  int mod;
  mod = Serial2.read();
  
  while (Serial2.available() > 0) {
    dataIn = int(Serial2.read());
    if (dataIn >= 48 && dataIn <= 57) SerialIn = 10 * SerialIn + dataIn - 48;
  }

  switch (mod)  {
    case 'b': //Begin communication
      puttyReady = true;
      break;
    case 'e': //End Communication
      puttyReady = false;
      break;
    case 's': //Set velRef
      *velRef = (double)SerialIn;
      break;
    case 'r':
      phi = posx = posy = 0; //Inicializamos a 0
      break;
    case '0':
      *modoFuncionamiento = 0;
      break;
    case '1':
      *modoFuncionamiento = 1;
      *radioCurv = (float)SerialIn;
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
