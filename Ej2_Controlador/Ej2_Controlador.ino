unsigned long PERIODO_MUESTREO_MS = 100;  // Periodo de muestreo para control (ms).

//Ultrasonidos
const int Echo1 = 11;
const int Echo2 = 12;
const int Trig1 = 9;
const int Trig2 = 10;

//Motor Derecho
const int IN1 = 2;
const int IN2 = 3;
const int ENA = 44;

//Motor Izquierdo
const int IN3 = 4;
const int IN4 = 5;
const int ENB = 45;

//MODO DE FUNCIONAMIENTO
int modo = 0;

//Variables de funcionamiento
bool puttyReady = false;
int tact, tant; //Tiempo entre ciclos
float distStop; //Distancia de parado
float dist1, old_dist1;
float dist2, old_dist2;
int vel1 = 0, vel2 = 0;
int tmAct, tmAnt;
float tm;
float maxDistDif = 0.5; //Threshold para parar el vehiculo en el modo 1 - Histeresis
float ping1, ping2;
float measureDiff = 0.2; //Maxima diferencia entre medidas para el filtro (malfuncionamiento sensor)
int mode = 0; //modo giro ruedas, para telemetria

float dist1Sum, dist2Sum;
int nMed;
int velUmbral = 90;
int velMin = 85;

// Modo 1 mucho mas sencillo sin controlador PID -  Solo control todo - nada
void modo1() {
  PERIODO_MUESTREO_MS = 50;
  if (abs(dist1 - distStop) < maxDistDif || abs(dist2 - distStop) < maxDistDif) {
    vel1 = vel2 = 0;
  }
  else if (dist1 > distStop || dist2 > distStop) {
    vel1 = vel2 = 110;
  }
  else if (dist1 < distStop || dist2 < distStop) {
    vel1 = vel2 = -110;
  }
}

// Modo 2 con PID para cada uno de los motores -  Suponemos que estan desacoplados y funciona bastanta bien.
void modo2() {
  PERIODO_MUESTREO_MS = 100;
  int col1, col2;
  int velMin1 = velMin;
  int velMin2 = velMin;
  col1 = round(controlador1(distStop, dist1, tm));
  col2 = round(controlador2(distStop, dist2, tm));
  Serial.println("Col1->" + String(col1) + "Col2->" + String(col2) + "tm->" + String(tm));

  // Para evitar la zona de arranque
  if (col1 >= 0)
    vel1 = col1 + velMin1;
  else
    vel1 = col1 - velMin1;

  if (col2 >= 0)
    vel2 = col2 + velMin2;
  else
    vel2 = col2 - velMin2;
}

void setup() {
  pinMode(Trig1, OUTPUT);
  pinMode(Echo1, INPUT);
  pinMode(Trig2, OUTPUT);
  pinMode(Echo2, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial2.begin(38400);

  Serial.begin(9600); //Debug

  Serial.println("SERIAL STARTED"); //Debug

  tact =  tant =  millis(); //Medidas para el envio de datos por Bluetooth
  tmAct = tmAnt = millis(); //Medidas intervalo control

  old_dist1 = dist1 = ping(Trig1, Echo1); //inicializamos las distancias
  old_dist2 = dist2 = ping(Trig2, Echo2);

  dist1Sum = 0;
  dist2Sum = 0;
  nMed = 0;
}

void loop() {
  //Lectura sensores y filtrado
  ping1 = ping(Trig1, Echo1);
  ping2 = ping(Trig2, Echo2);
  filtro(&old_dist1, &dist1, ping1, measureDiff);
  filtro(&old_dist2, &dist2, ping2, measureDiff);

  dist1Sum += dist1;
  dist2Sum += dist2;
  nMed += 1;

  //Hay mas de 2 Bytes listos para ser leidos - Usamos comandos de 3 parametros para configurarlo
  if (Serial2.available() > 2)
    BTread(&modo, &distStop);

  tmAct = millis();
  if ((tmAct - tmAnt) >= PERIODO_MUESTREO_MS) {
    //Eliminamos el ruido de las muestras con un promediado
    dist1 = dist1Sum / nMed;
    dist2 = dist2Sum / nMed;

    //Resetamos las variables
    dist1Sum = 0;
    dist2Sum = 0;
    nMed = 0;

    tm = (tmAct - tmAnt) / 1000.0; //Se pasa a segundos

    switch (modo) {
      case 0:   //Modo 0 - Parado
        vel1 = 0; vel2 = 0;
        break;

      case 1:   //Modo 1 - Avanzar/Retroceder hasta quedarse a X distancia
        modo1();
        break;

      case 2:
        modo2(); //Modo 2 - Quedarse a X distancia y en PARALELO
        break;
    }

    moveRobot(vel1, vel2);
    //Telemetría (Putty)

     if (vel2<velUmbral&&vel2>-velUmbral)
      vel2=0;
    
    if (vel1<velUmbral&&vel1>-velUmbral)
      vel1=0;
    
    tact = millis();
    telemetria(tact - tant, dist2, dist1, distStop, mode, vel2, vel1);
    tant = millis();

    tmAnt = millis();
  }


}
