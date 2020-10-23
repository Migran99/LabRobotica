//PROGRAMA MEDIR DISTANCIAS CON ULTRASONIDOS
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

int ping(int TrgPin,int EchoPin) {
  long duration, distanceCm;
  
  digitalWrite(TrgPin, LOW); 
  delayMicroseconds(4);
  digitalWrite(TrgPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(TrgPin, LOW);
  
  duration = pulseIn(EchoPin, HIGH);
  
  distanceCm = duration * 10 / 292/ 2;   //convertimos a distancia en cm
  return distanceCm;
}

void forward(int vel = 200){
    analogWrite(ENA, vel);
    analogWrite(ENB, vel);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}
void Rstop(){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }

int BTread(){
  int SerialIn=-1;
  int dataIn;
  if(Serial2.available() > 0){
    SerialIn=0;
     delay(1000);
     while(Serial2.available() > 0){
       dataIn = int(Serial2.read());
       if(dataIn >= 48 && dataIn <= 57) SerialIn = 10*SerialIn + dataIn - 48;
     }
  }
  return SerialIn;
}

long dist1,dist2;

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
  Serial.begin(9600);
  while(!Serial2){
      delay(100);
    }
   Serial.println("SERIAL STARTED");
}

int SerialIn = 0;
int dataIn;
int BTr;
void loop() {
  dist1 = ping(Trig1,Echo1);
  dist2 = ping(Trig2,Echo2);

  if(Serial.available() > 0){
    SerialIn = 0;
    while(Serial.available() > 0){
      dataIn = int(Serial.read());
      if(dataIn >= 48 && dataIn <= 57) SerialIn = 10*SerialIn + dataIn - 48;
    }
      distStop = SerialIn;
      Serial.println("Max Distance changed to -->  " + String(distStop));
  }
<<<<<<< HEAD:TEST_RobotMovil.ino
  if(Serial2.available() > 0){
      Serial.println("Available Bytes: " +String(Serial2.available()));
      dataIn = int(Serial2.read());
      Serial.println("Read from BT: " +String(dataIn));

    }
  Serial.println("BLUETOOTH ----->> Medida 1: " + String(dist1) + "\t\t Medida 2: " + String(dist2));
=======

  BTr=BTread();
  if (BTr>-1)
  {
    distStop=BTr;
    Serial.println("Max Distance changed to -->  " + String(distStop));
  }
  
  //4Serial.println("BLUETOOTH ----->> Medida 1: " + String(dist1) + "\t\t Medida 2: " + String(dist2));
>>>>>>> main:TEST_RobotMovil/TEST_RobotMovil.ino
  Serial2.println(String(dist1) + " " + String(dist2));

  if(dist1 < distStop || dist2 < distStop) Rstop();
  else forward();
  

  delay(50);

}