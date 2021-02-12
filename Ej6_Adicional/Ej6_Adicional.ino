#define pi          3.1416
#define radioRueda  3.2     //cm
#define baseline    9.0     //cm

// Periodo de muestreo para control (ms)
unsigned long PERIODO_MUESTREO_MS = 50;

//Ultrasonidos
const int Echo1 = 11;
const int Trig1 = 9;

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

//Modo de funcionamiento
int modo = 0;

//Variables de funcionamiento
bool puttyReady = false;
int tact, tant;
int vel1 = 0, vel2 = 0;
int tmAct, tmAnt;
float tm;
int mode = 0;
int velUmbral = 85;
int velMinG = 80;
double velRef = 40.0;    /*Velocidad de crucero de la circunferencia*/

//Variables de odometria
float vl, vr, vx, vy, v;
float phid;
float posx, posy, phi;

//Variables descripción circulos
float radioCurv = 10;   /*Radio de la circunferencia que describe (valor por defecto)*/

//Medida nube puntos
float distPunto;        /*Medida de la distancia al objeto*/

void modo_avanzado() {
  float velRefL, velRefR;
  float curvatura;
  int col1, col2;
  int velMin1 = velMinG;
  int velMin2 = velMinG;

  //Calculo de la curvatura
  curvatura = 1.0 / radioCurv;

  //Calculo de la velocidad de giro de las ruedas
  velRefL = (velRef / radioRueda) * (1.0 + (baseline / 2.0) * curvatura);
  velRefR = (velRef / radioRueda) * (1.0 - (baseline / 2.0) * curvatura);
  
  //Pasamos de rad/s a rpm
  velRefL = velRefL * 30.0 / pi;
  velRefR = velRefR * 30.0 / pi;

  //Calculamos la señal de control
  col1 = round(controlador1(velRefR, m1 * rpmR, tm));
  col2 = round(controlador2(velRefL, m2 * rpmL, tm));

  //Evitamos las zonas muertas
  if (col1 > 0)
    vel1 = col1 + velMin1;
  else
    vel1 = col1 - velMin1;
  if (col2 > 0)
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

float angular2linear(float w) {
  return (float)w * pi / 30.0 * radioRueda;
}
float linear2angular(float vlin) {
  return (float)vlin * 30.0 / (pi * radioRueda);
}

void setup() {
  //Entrada de los encoders
  pinMode(ENCL, INPUT_PULLUP);
  pinMode(ENCR, INPUT_PULLUP);

  //Conexion del ultrasonidos
  pinMode(Trig1, OUTPUT);
  pinMode(Echo1, INPUT);

  //Actuadores: Motores
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  //Comunicacion con el BT
  Serial2.begin(38400);

  //Medidas para el envio de datos por Bluetooth
  tact =  tant =  millis(); 
  
  //Medidas intervalo control
  tmAct = tmAnt = millis(); 

  //Inicializamos a 0
  phi = posx = posy = 0;

  //Asociamos interrupciones para el cencoder
  LenconderCnt = RenconderCnt = 0;
  attachInterrupt(digitalPinToInterrupt(ENCR), ISRRight, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCL), ISRLeft, RISING);
  timeEncAnt = timeEncAct = micros();
}

void loop() {

  //Hay 4 o más Bytes listos para ser leidos (Usamos comandos de 4 parametros para configurarlo)
  if (Serial2.available() > 3)
    BTread(&modo, &radioCurv, &velRef);

  //Medida ultrasonido
  distPunto = ping(Trig1, Echo1);

  //FUNCIONAMIENTO
  tmAct = millis();
  if ((tmAct - tmAnt) >= PERIODO_MUESTREO_MS) {
    
    //Lectura encoders
    medirVelocidad();
    tm = (tmAct - tmAnt) / 1000.0; //s

    //Odometría
    vl = angular2linear(rpmL);
    vr = angular2linear(rpmR);
    phid = (vr - vl) / baseline;
    v = (vl + vr) / 2.0;
    phi += phid * (tmAct - tmAnt) / 1000.0;
    while (phi > 2 * pi) phi -= 2 * pi;
    while (phi < 0) phi += 2 * pi;
    vx = v * cos(phi);
    vy = v * sin(phi);
    posx += vx * (tmAct - tmAnt) / 1000.0;
    posy += vy * (tmAct - tmAnt) / 1000.0;

    //Control (según el modo seleccionado)
    switch (modo) {
      case 0:   //Modo 0 - Parado
        vel1 = 0; vel2 = 0;
        break;

      case 1:   //Modo 1 - Avanzar paralelo a la pared
        modo_avanzado();
        break;
    }
    tmAnt = millis();

    //Enviamos los datos para telemetría
    tact = millis();
    telemetriaObj(tact - tant, posx, posy, phi, distPunto);
    tant = millis();
    moveRobot(vel1, vel2);
  }
}
