//PROGRAMA MEDIR DISTANCIAS CON ULTRASONIDOS
//Telemetría implementada
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

int distStop = 7;
bool puttyReady = false;

int ping(int TrgPin, int EchoPin) {
  long duration, distanceCm;

  digitalWrite(TrgPin, LOW);
  delayMicroseconds(4);
  digitalWrite(TrgPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrgPin, LOW);

  duration = pulseIn(EchoPin, HIGH);

  distanceCm = duration * 10 / 292 / 2;  //convertimos a distancia en cm
  return distanceCm;
}

void forward(int vel1 = 200, int vel2 = 200) {
  analogWrite(ENA, vel1);
  analogWrite(ENB, vel2);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void Rstop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

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

//Factor de conversión (d -> cm)
#define fcL 1
#define fcR 1

void telemetria(int t,int distL, int distR, int ref, int mode, int pwmL, int pwmR)
{
  if (puttyReady) //Empezamos solo cuando el putty esté listo (Debemos asegurarnos de tener el mismo numero de columnas en todas las filas)
    Serial2.println(String(t) + " " + String(fcL*distL) + " " + String(fcR*distR) + " " + String(ref) + " " + String(mode) + " " + String(pwmL) + " " + String(pwmR));
}

long dist1, dist2;
int SerialIn = 0;
int dataIn;

int BTr; //Valor leído por el putty
int ref = 0; //Valor de referencia
int mode = 0; //Modo del robot
int pwm[] = {200, 200}; //Valor de pwm de los motores
int tact, tant; //Tiempo entre ciclos

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

  while (!Serial2) {
    delay(100);
  }

  Serial.println("SERIAL STARTED"); //Debug

  tact = millis();
  tant = millis();
}

void loop() {
  dist1 = ping(Trig1, Echo1);
  dist2 = ping(Trig2, Echo2);

  /*
    //DEBUG
    if(Serial.available() > 0){
    SerialIn = 0;
    while(Serial.available() > 0){
      dataIn = int(Serial.read());
      if(dataIn >= 48 && dataIn <= 57) SerialIn = 10*SerialIn + dataIn - 48;
    }
      distStop = SerialIn;
      Serial.println("Max Distance changed to -->  " + String(distStop));
    }
    //Serial.println("BLUETOOTH ----->> Medida 1: " + String(dist1) + "\t\t Medida 2: " + String(dist2));
  */

  //Lectura de datos desde el putty
  if (Serial2.available()>2)
    BTread();

  //Telemetría (Putty)
  tact = millis();
  telemetria(tact-tant,dist1,dist2,distStop,mode,pwm[0],pwm[1]);
  tant = millis();

  //Control
  if (dist1 < distStop || dist2 < distStop) Rstop();
  else forward();


  delay(50);

}
