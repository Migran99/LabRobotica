void BTread() {
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
    case 's': //Set distStop
      distStop = SerialIn;
      Serial.println("Max Distance changed to -->  " + String(distStop)); //Debug
      break;
  }
}
