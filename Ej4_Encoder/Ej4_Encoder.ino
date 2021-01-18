unsigned long PERIODO_MUESTREO_MS = 50;  // Periodo de muestreo para control (ms).

//Ultrasonidos
const int Echo1 = 11;
const int Echo2 = 12;
const int Trig1 = 9;
const int Trig2 = 10;

//Motor Derecho
const int IN1 = 25;
const int IN2 = 26;
const int ENA = 44;

//Motor Izquierdo
const int IN3 = 27;
const int IN4 = 28;
const int ENB = 45;

//Encoders
const int ENCL = 2;
const int ENCR = 3;
const int rtL = 48;
const int rtR = 48;
unsigned long LenconderCnt;
unsigned long RenconderCnt;
unsigned long timeEncAnt,timeEncAct;
double rpmL, rpmR;

//MODO DE FUNCIONAMIENTO
int modo = 0;

//Variables de funcionamiento
bool puttyReady = false;
int tact, tant; //Tiempo entre ciclos
float distStop;
float dist1, old_dist1 = 0;
float dist2, old_dist2 = 0;
int vel1 = 0, vel2 = 0;
int tmAct, tmAnt;
float tm;
float maxDistDif = 0.5; //Threshold para parar el vehiculo en el modo 1 - Histeresis
float ping1, ping2;
float measureDiff = 0.2; //Maxima diferencia entre medidas para el filtro (malfuncionamiento sensor)
int mode = 0;

float dist1Sum, dist2Sum;
int nMed;
int velUmbral = 90;
int velMinG = 85;

// Modo 1 mucho mas sencillo sin controlador PID -  Solo control todo - nada
void modo1() {
  int col1, col2;
  int velMin1 = velMinG;
  int velMin2 = velMinG;
  double refR = 100;
  double refL = 100;
  
  PERIODO_MUESTREO_MS = 50;
  col1 = round(controlador1(refR, rpmR, tm));
  col2 = round(controlador2(refL, rpmL, tm));
  if (col1 >= 0)
    vel1 = col1 + velMin1;
  else
    vel1 = col1 - velMin1;

  if (col2 >= 0)
    vel2 = col2 + velMin2;
  else
    vel2 = col2 - velMin2;
}

// Modo 2 con PID para cada uno de los motores -  Suponemos que estan desacoplados y funciona bastanta bien.
void modo2() { 
  float distMedia = (dist1+dist2)/2;
  PERIODO_MUESTREO_MS = 50;
  vel1 = 130;

  int col1, col2;
  int velMin1 = velMinG;
  int velMin2 = velMinG;
  col1 = controlador1(distStop, distMedia, tm);
  col2 = round(controlador2(col1, dist1-dist2, tm));
  vel2 = col2 + vel1;
  //vel2 = 130;
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

  pinMode(ENCL,INPUT_PULLUP);
  pinMode(ENCR,INPUT_PULLUP);
  
  LenconderCnt = RenconderCnt = 0;
  attachInterrupt(digitalPinToInterrupt(ENCR), ISRRight, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCL), ISRLeft, RISING);
  timeEncAnt = timeEncAct = micros();
}

void loop() {
  //Hay mas de 2 Bytes listos para ser leidos - Usamos comandos de 3 parametros para configurarlo
  if (Serial2.available() > 2)
    BTread(&modo, &distStop);

  tmAct = millis();
  if ((tmAct - tmAnt) >= PERIODO_MUESTREO_MS) {
    medirVelocidad();
    tm = (tmAct - tmAnt) / 1000.0; //Se pasa a segundos
    switch (modo) {
      case 0:   //Modo 0 - Parado
        vel1 = 0; vel2 = 0;
        break;

      case 1:   //Modo 1 - Avanzar paralelo a la pared
        modo1();
        break;

      case 2:
        modo2(); //Modo 2 - Avanzar paralelo a la pared a distancia dada
        break;
    }

    
    //Telemetría (Putty)
//
//     if (vel2<velUmbral&&vel2>-velUmbral)
//      vel2=0;
//    
//    if (vel1<velUmbral&&vel1>-velUmbral)
//      vel1=0;
    
    tact = millis();
    telemetria(tact - tant, rpmL, rpmR, distStop, mode, vel2, vel1);
    tant = millis();
    moveRobot(vel1, vel2);

    tmAnt = millis();
  }


}
