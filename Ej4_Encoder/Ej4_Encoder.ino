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
int m1;

//Motor Izquierdo
const int IN3 = 27;
const int IN4 = 28;
const int ENB = 45;
int m2;

// Enconders
const int ENCL = 2;
const int ENCR = 3;
const int rtL = 48;
const int rtR = 48;
unsigned long LenconderCnt;
unsigned long RenconderCnt;
unsigned long timeEncAnt, timeEncAct;
double rpmL, rpmR;

//MODO DE FUNCIONAMIENTO
int modo = 0;

//Variables de funcionamiento
bool puttyReady = false;
int tact, tant; //Tiempo entre ciclos
float distStop;
float dist1, old_dist1;
float dist2, old_dist2;
int vel1 = 0, vel2 = 0;
int tmAct, tmAnt;
float tm;
float maxDistDif = 0.5; //Threshold para parar el vehiculo en el modo 1 - Histeresis
float ping1, ping2;
float measureDiff = 0.2; //Maxima diferencia entre medidas para el filtro (malfuncionamiento sensor)
int mode = 0;
double velRef;

float dist1Sum, dist2Sum;
int nMed;
int velUmbral = 90;
int velMinG = 80;

// Modo 1 mucho mas sencillo sin controlador PID -  Solo control todo - nada
void modo5() {
  PERIODO_MUESTREO_MS = 50;

  int col1, col2;
  int velMin1 = velMinG;
  int velMin2 = velMinG;
  col1 = round(controlador1(velRef, m1*rpmR, tm));
  col2 = round(controlador2(velRef, m2*rpmL, tm));

  if (col1 > 0)
    vel1 = col1 + velMin1;
  else
    vel1 = col1 - velMin1;
  if (col1 > 0)
    vel2 = col2 + velMin2;
  else
    vel2 = col2 - velMin2;
}

void ISRLeft() {
  LenconderCnt++;
}

void ISRRight() {
  RenconderCnt++;
}

void setup() {
  pinMode(ENCL, INPUT_PULLUP);
  pinMode(ENCR, INPUT_PULLUP);

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

  velRef = 0;

  LenconderCnt = RenconderCnt = 0;
  attachInterrupt(digitalPinToInterrupt(ENCR), ISRRight, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCL), ISRLeft, RISING);
  timeEncAnt = timeEncAct = micros();
}

void loop() {

  //Hay mas de 2 Bytes listos para ser leidos - Usamos comandos de 3 parametros para configurarlo
  if (Serial2.available() > 3)
    BTread(&modo, &velRef);

  tmAct = millis();
  if ((tmAct - tmAnt) >= PERIODO_MUESTREO_MS) {
    //Lectura encoders
    medirVelocidad();
    tm = (tmAct - tmAnt) / 1000.0; //Se pasa a segundos

    switch (modo) {
      case 0:   //Modo 0 - Parado
        vel1 = 0; vel2 = 0;
        break;

      case 1:   //Modo 1 - Avanzar paralelo a la pared
        modo5();
        break;
    }


    //Telemetría (Putty)

    if (vel2 < velUmbral && vel2 > -velUmbral)
      vel2 = 0;

    if (vel1 < velUmbral && vel1 > -velUmbral)
      vel1 = 0;

    tact = millis();
    telemetria(tact - tant, m2*rpmL, m1*rpmR, velRef, mode, vel2, vel1);
    tant = millis();
    moveRobot(vel1, vel2);
    tmAnt = millis();
  }

  //delay(100);
}
