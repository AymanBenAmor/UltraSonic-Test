const byte TRIGGER = 12; // broche TRIGGER
const byte ECHO = 13; // broche ECHO
/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = 8m à 340m/s
/* Vitesse du son dans l'air en mm/us */
const float SOUND_SPEED = 340.0 / 10000;


long measure;
float distance_cm;

int speedm1;
int speedm2;



//moteur 1
int av_m1 = 18;
int re_m1 = 19;
int vitt_1 = 5;
int basespeedm1 = 153 ;
int maxspeedm1 = 255;

// Moteur 2
int av_m2 = 21;
int re_m2 = 22;     
int vitt_2 = 23;
int basespeedm2 = 180;
int maxspeedm2 = 255;

const int freq = 30000;
const int pwmChannel_1 = 0;
const int pwmChannel_2 = 1;
const int resolution = 8;

char position_1 ;

int kps = 6;
int kds =100;

float error,P,D;
float lasterror = 0;
float motorspeed;



void setup() {
  /*code pour module ultrason HCSR04 */
  pinMode(av_m1, OUTPUT);
  pinMode(re_m1, OUTPUT);
  pinMode(vitt_1, OUTPUT);
  
  pinMode(av_m2, OUTPUT);
  pinMode(re_m2, OUTPUT);
  pinMode(vitt_2, OUTPUT);
  
  ledcSetup(pwmChannel_1, freq, resolution);
  ledcSetup(pwmChannel_2, freq, resolution);
  ledcAttachPin(vitt_1, pwmChannel_1);
  ledcAttachPin(vitt_2, pwmChannel_2);
  
  Serial.begin(115200);
  /* ATTENTION SUR ECRAN SERIAL MONITOR , il faudra choisir 115200 baud dans menu deroulant */
  pinMode(TRIGGER, OUTPUT);
  digitalWrite(TRIGGER, LOW);
  pinMode(ECHO, INPUT);

  
}


void loop() {
  /* 1 envoie une impulsion HIGH de 10 us sur broche trigger */
  

  read_dist();
    pid_sonic();
  
  

  print();
  delay(10);
    

 
  
}





void pid_sonic(){

  if(distance_cm<1 || distance_cm>40){
    distance_cm = 12;
  }
  error = distance_cm-12;

  P = error;
  D = error - lasterror;
  lasterror = error;

  motorspeed = kps*P + kds*D;

  speedm1 = basespeedm1 + motorspeed;
  speedm2 = basespeedm2 - motorspeed;

  if (speedm1 <=0){
    speedm1=0;
  }
  if (speedm2 <=0){
    speedm2=0;
  }
  if (speedm1 >=255){
    speedm1=maxspeedm1;
  }
  if (speedm2 >=255){
    speedm2=maxspeedm2;
  }

  forward_brake(speedm1,speedm2);
}


void read_dist(){
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);
  
  /* 2 Mesure le temps entre l'envoi de l'impulsion et son echo */
  measure = pulseIn(ECHO, HIGH, MEASURE_TIMEOUT);
  
  /* 3 calcul la distance a partir du temps mesuré */
  
  distance_cm = measure / 2.0 * SOUND_SPEED;
}




void forward_brake(int posa, int posb) {
  //set the appropriate values for aphase and bphase so that the robot goes straight
  digitalWrite(re_m1, LOW);
  digitalWrite(av_m1, HIGH);
  ledcWrite(pwmChannel_1,posa);
  digitalWrite(re_m2, LOW);
  digitalWrite(av_m2, HIGH);
  ledcWrite(pwmChannel_2,posb);
}
void forward(int x) {
  //set the appropriate values for aphase and bphase so that the robot goes straight
  digitalWrite(re_m1, LOW);
  digitalWrite(av_m1, HIGH);
  ledcWrite(pwmChannel_1,153);
  digitalWrite(re_m2, LOW);
  digitalWrite(av_m2, HIGH);
  ledcWrite(pwmChannel_2,180);
  delay(x);
}
void back(int posa, int posb) {
  //set the appropriate values for aphase and bphase so that the robot goes straight
  digitalWrite(re_m1, HIGH);
  digitalWrite(av_m1, LOW);
  ledcWrite(pwmChannel_1,posa);
  digitalWrite(re_m2, HIGH);
  digitalWrite(av_m2, LOW);
  ledcWrite(pwmChannel_2,posb);
}

void left(int posa, int posb) {
  //set the appropriate values for aphase and bphase so that the robot goes straight
  digitalWrite(re_m1, HIGH);
  digitalWrite(av_m1, LOW);
  ledcWrite(pwmChannel_1,posa);
  digitalWrite(re_m2, LOW);
  digitalWrite(av_m2, HIGH);
  ledcWrite(pwmChannel_2,posb);
}
void right(int posa, int posb) {
  //set the appropriate values for aphase and bphase so that the robot goes straight
  digitalWrite(re_m1, LOW);
  digitalWrite(av_m1, HIGH);
  ledcWrite(pwmChannel_1,posa);
  digitalWrite(re_m2, HIGH);
  digitalWrite(av_m2, LOW);
  ledcWrite(pwmChannel_2,posb);
}

void print(){
   
 Serial.print(distance_cm , 2);
  Serial.print("cm de ");
  Serial.print("\t");
  Serial.print(speedm1);
  Serial.print("\t");
  Serial.print(speedm2);
  Serial.print("\t");
  Serial.print(error);
  Serial.print("\n");

}
