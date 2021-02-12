#define pi          3.1416
#define radioRueda  3.2   //cm
#define baseline    8.0 //cm

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
int m1=1;

//Motor Izquierdo
const int IN3 = 27;
const int IN4 = 28;
const int ENB = 45;
int m2=1;

//Enconders
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
double velRef;
int velUmbral = 85;
int velMinG = 80;

//Variables de odometria
float vl, vr, vx, vy, v;
float phid;
float posx, posy, phi;
const float lado = 100.0;
float posRef[4][2] = {{lado, 0.0}, {lado, lado}, {0.0, lado},{0.0,0.0}};
int ip = 0;

//Modo 7 - Control de posición
float wCont, vCont; /*Las hacemos globales para sacarlas por la odometría*/
void modo7() {
  PERIODO_MUESTREO_MS = 50;
  int col1, col2;
  int velMin1 = velMinG;
  int velMin2 = velMinG;
  float angRef, eAng;
  float eLineal;
  float vLCont, vRCont;

  //Calculamos el ángulo que debe tomar el robot para ir al punto destino
  angRef = atan2(posRef[ip][1] - posy, posRef[ip][0] - posx);
  
  //El angulo resultante deberá estar entre 0 y 2*pi
  while (angRef < 0) angRef += 2.0 * pi;

  //Error de ángulo (entre -pi y pi)
  eAng = angRef - phi;
  if (eAng > pi) eAng -= 2.0 * pi;
  else if (eAng < -pi) eAng += 2.0 * pi;

  //Error lineal
  eLineal = sqrt(pow(posRef[ip][1] - posy, 2) + pow(posRef[ip][0] - posx, 2)); //error lineal

  //Si tenemos un error en angulo superior a 0.1 rad, nos orientamos:
  if (abs(eAng) > 0.1) {
    //Control de velocidad angular del robot
    wCont = controlador3(0, -eAng, tm);
    vCont = 0;

    //Calculo de la velocidad de referencia de cada rueda
    vLCont = vCont - baseline * wCont / 2.0;
    vRCont = vCont + baseline * wCont / 2.0;
    vLCont = linear2angular(vLCont);
    vRCont = linear2angular(vRCont);

    //Control de velocidad de las ruedas
    col1 = round(controlador1(vRCont, m1 * rpmR, tm));
    col2 = round(controlador2(vLCont, m2 * rpmL, tm));
  }

  //Si el angulo es el correcto, avanzamos:
  else if (eLineal > 1.0) {
    //Control de velocidad linear
    vCont = controlador4(0, -eLineal, tm);
    wCont = 0;

    //Calculo de la velocidad de referencia de cada rueda
    vLCont = vCont - baseline * wCont / 2.0;
    vRCont = vCont + baseline * wCont / 2.0;
    vLCont = linear2angular(vLCont);
    vRCont = linear2angular(vRCont);

    //Control de velocidad de las ruedas
    col1 = round(controlador1(vRCont, m1 * rpmR, tm));
    col2 = round(controlador2(vLCont, m2 * rpmL, tm));
  }
  //Hemos llegado al punto deseado:
  else {
    //Cambiamos de punto destino
    vCont = wCont = 0;
    col1 = col2 = 0;
    if(ip < 3)ip++;
  }

  //Nos quitamos las zonas muertas
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

//Funciones para convertir de velocidad angular a linear, y viceversa
float angular2linear(float w) {
  return (float)w * pi / 30.0 * radioRueda;
}
float linear2angular(float vlin) {
  return (float)vlin * 30.0 / (pi * radioRueda);
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
  Serial.begin(9600); /*Debug*/
  Serial.println("SERIAL STARTED"); //Debug

  tact =  tant =  millis();
  tmAct = tmAnt = millis();
  velRef = 0;

  //Posicion y orientacion global inicial del robot 
  phi = posx = posy = 0;

  //Encoders
  LenconderCnt = RenconderCnt = 0;
  attachInterrupt(digitalPinToInterrupt(ENCR), ISRRight, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCL), ISRLeft, RISING);
  timeEncAnt = timeEncAct = micros();
}

void loop() {

  //Hay 4 o más Bytes listos para ser leidos - Usamos comandos de 4 parametros para configurarlo
  if (Serial2.available() > 3)
    BTread(&modo, &velRef);
  
  tmAct = millis();
  if ((tmAct - tmAnt) >= PERIODO_MUESTREO_MS) {
    //Lectura encoders
    medirVelocidad();
    tm = (tmAct - tmAnt) / 1000.0;

    //ODOMETRIA
    //---
    //Calculamos la velocidad linear de cada rueda
    vl = angular2linear(m2 * rpmL);
    vr = angular2linear(m1 * rpmR);

    //Calculamos la velocidad angular y linear del robot
    phid = (vr - vl) / baseline;
    v = (vl + vr) / 2.0;

    //Integramos para obtener el angulo del robot [0,2*pi]
    phi += phid * (tmAct - tmAnt) / 1000.0;
    while (phi > 2 * pi) phi -= 2 * pi;
    while (phi < 0) phi += 2 * pi;

    //Obtenemos la velocidad del robot en x e y global
    vx = v * cos(phi);
    vy = v * sin(phi);

    //Integramos para obtener la velocidad
    posx += vx * (tmAct - tmAnt) / 1000.0;
    posy += vy * (tmAct - tmAnt) / 1000.0;
    //---

    //Control
    switch (modo) {
      case 0:   //Modo 0 - Parado
        vel1 = 0; vel2 = 0;
        break;

      case 1:   //Modo 7
        modo7();
        break;
    }
    tmAnt = millis();

    //Telemetría (Putty) -- Adaptada a la odometría
    tact = millis();
    telemetriaOD(tact - tant, posx, posy, vx, vy, wCont, vCont, vel2, vel1, phi);
    tant = millis();
    moveRobot(vel1, vel2);
  }
}
