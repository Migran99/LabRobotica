//Modos de movimiento del robot

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
}

void loop() {
  moveRobot(200, 200, 1); //Movemos hacia delante
  delay(1000);
  moveRobot();//Paramos
  delay(3000);//Calibrar si se desvía
  moveRobot(200, 200, 2); //Movemos hacia detrás
  delay(1000);
  moveRobot();//Paramos
  delay(3000);//Ver si acaba en el mismo sitio
  moveRobot(200, 200, 3); //Pivota hacia la derecha
  delay(1000);
  moveRobot();//Paramos
  delay(3000);
  moveRobot(200, 200, 4); //Pivota hacia la izquierda
  delay(1000);
  moveRobot();//Paramos
  delay(3000);
}
