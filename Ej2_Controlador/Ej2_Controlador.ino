#define PERIODO_MUESTREO_MS     ((unsigned long) 20)  // Periodo de muestreo para control (ms).

//Ultrasonidos
const int Echo1 = 12;
const int Echo2 = 11;
const int Trig1 = 10;
const int Trig2 = 9;

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
float distStop;
float dist1;
float dist2;
int mode = 0, vel1 = 0, vel2 = 0;

float maxDistDif = 0.5;

void modo1() {
  vel1 = 70;
  vel2 = 70;
  if (abs(dist1 - distStop) < maxDistDif || abs(dist2 - distStop) < maxDistDif) mode = 0;
  else if (dist1 > distStop || dist2 > distStop) mode = 1;
  else if (dist1 < distStop || dist2 < distStop) mode = 2;
}

void modo2() {
  float distDiff = dist1 - dist2;
  vel1 = 70;
  vel2 = 70;
  if (abs(dist1 - distStop) < maxDistDif || abs(dist2 - distStop) < maxDistDif) {
    if (distDiff > maxDistDif / 2)  mode = 3;
    else if (distDiff < -maxDistDif / 2) mode = 4;
    else mode = 0;
  }
  else if (dist1 > distStop || dist2 > distStop) mode = 1;
  else if (dist1 < distStop || dist2 < distStop) mode = 2;

  Serial.println("\nMODE 2: Robot Mode-> " + String(mode) +"\t dist1: "+ String(dist1) + "  ;  dist2: " + String(dist2) + "  ;  distDiff:  " + String(distDiff));
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

  tact = millis();
  tant = millis();
}

void loop() {
  dist1 = ping(Trig1, Echo1);
  dist2 = ping(Trig2, Echo2);

  if (Serial2.available() > 2)
    BTread(&modo, &distStop);

  switch (modo)
  {
    case 0:
      mode = 0; vel1 = 0; vel2 = 0;
      break;

    case 1: //MODO1
      modo1();
      break;

    case 2:
      modo2();
      break;

  }

  moveRobot(vel1, vel2, mode);
  //Telemetría (Putty)
  tact = millis();
  telemetria(tact - tant, dist1, dist2, distStop, mode, vel1, vel2);
  tant = millis();
}
