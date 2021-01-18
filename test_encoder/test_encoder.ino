float medida;
const int ENCL = 2;
const int ENCR = 3;
const int rtL = 48;
const int rtR = 48;
unsigned long LenconderCnt;
unsigned long RenconderCnt;

unsigned long timeEncAnt,timeEncAct;

double rpmL, rpmR;

void ISRLeft(){
  LenconderCnt++;
}

void ISRRight(){
  RenconderCnt++;
}

void setup() {
  Serial.begin(9600);
  pinMode(ENCL,INPUT_PULLUP);
  pinMode(ENCR,INPUT_PULLUP);
  
  LenconderCnt = RenconderCnt = 0;
  attachInterrupt(digitalPinToInterrupt(ENCR), ISRRight, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCL), ISRLeft, RISING);
  timeEncAnt = timeEncAct = micros();
}

void medirVelocidad()
{
  unsigned long interv;
  
  timeEncAct = micros();
  interv=timeEncAct-timeEncAnt;

  rpmL = (double)(LenconderCnt*pow(10,6)*60.0)/(interv*4.0*rtL);
  rpmR = (double)(RenconderCnt*pow(10,6)*60.0)/(interv*4.0*rtR); 

  timeEncAnt = micros();
  LenconderCnt = 0;
  RenconderCnt = 0;
}

void loop() {
  medirVelocidad();
  Serial.println(rpmL);
  delay(100);
}
