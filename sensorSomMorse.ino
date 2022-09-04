int pinoSensor = A1;
 
float magnitude ;
int magnitudelimit = 100;
int magnitudelimit_low = 100;
int realstate = LOW;
int realstatebefore = LOW;
int filteredstate = LOW;
int filteredstatebefore = LOW;
 
 
float coeff;
float Q1 = 0;
float Q2 = 0;
float sine;
float cosine;  
float sampling_freq=8928.0;
float target_freq=558.0; 
float n=48.0; 
int testData[48];
 
int nbtime = 6;     
 
long starttimehigh;
long highduration;
long lasthighduration;
long hightimesavg;
long lowtimesavg;
long startttimelow;
long lowduration;
long laststarttime = 0;
 
char code[20];
int stop = LOW;
int wpm;
 
 

void setup() {
  
// Calculo de goertzel
  int  k;
  float omega;
  k = (int) (0.5 + ((n * target_freq) / sampling_freq));
  omega = (2.0 * PI * k) / n;
  sine = sin(omega);
  cosine = cos(omega);
  coeff = 2.0 * cosine;
 

 Serial.begin(9600);
          
 
}

 void loop() {
 
  //TOM
  for (char index = 0; index < n; index++)
  {
    testData[index] = analogRead(pinoSensor);
  }
  for (char index = 0; index < n; index++){
    float Q0;
    Q0 = coeff * Q1 - Q2 + (float) testData[index];
    Q2 = Q1;
    Q1 = Q0;  
  }
  float magnitudeSquared = (Q1*Q1)+(Q2*Q2)-Q1*Q2*coeff;
  magnitude = sqrt(magnitudeSquared);
  Q2 = 0;
  Q1 = 0;
 
  if (magnitude > magnitudelimit_low){
    magnitudelimit = (magnitudelimit +((magnitude - magnitudelimit)/6));
  }
 
  if (magnitudelimit < magnitudelimit_low)
  magnitudelimit = magnitudelimit_low;
 
 
  if(magnitude > magnitudelimit*0.6)
     realstate = HIGH;
  else
    realstate = LOW;
 
 
  if (realstate != realstatebefore){
  laststarttime = millis();
  }
  if ((millis()-laststarttime)> nbtime){
  if (realstate != filteredstate){
    filteredstate = realstate;
  }
  }
 
 if (filteredstate != filteredstatebefore){
  if (filteredstate == HIGH){
    starttimehigh = millis();
    lowduration = (millis() - startttimelow);
  }
 
  if (filteredstate == LOW){
    startttimelow = millis();
    highduration = (millis() - starttimehigh);
        if (highduration < (2*hightimesavg) || hightimesavg == 0){
      hightimesavg = (highduration+hightimesavg+hightimesavg)/3;
    }
    if (highduration > (5*hightimesavg) ){
      hightimesavg = highduration+hightimesavg;
    }
  }
  }
 
 
 if (filteredstate != filteredstatebefore){
  stop = LOW;
  if (filteredstate == LOW){
   if (highduration < (hightimesavg*2) && highduration > (hightimesavg*0.6)){
  strcat(code,".");
   }
   if (highduration > (hightimesavg*2) && highduration < (hightimesavg*6)){
  strcat(code,"-");
  wpm = (wpm + (1200/((highduration)/3)))/2;
   }
  }
 
   if (filteredstate == HIGH){
   
   float lacktime = 1;
   if(wpm > 25)lacktime=1.0;
   if(wpm > 30)lacktime=1.2;
   if(wpm > 35)lacktime=1.5;

   //espaco
   if (lowduration > (hightimesavg*(2*lacktime)) && lowduration < hightimesavg*(5*lacktime)){
  decodificar();
  code[0] = '\0';
  Serial.print(" ");
   }
   
   //pular linha
   if (lowduration >= hightimesavg*(5*lacktime)){
  decodificar();
  code[0] = '\0';
  printarASCII(32);
  Serial.println();
   }
  }
 }

 //passou o tempo de ter letras
  if ((millis() - startttimelow) > (highduration * 6) && stop == LOW){
   decodificar();
   code[0] = '\0';
   stop = HIGH;
  }
 
 //fim do loop
 realstatebefore = realstate;
 lasthighduration = highduration;
 filteredstatebefore = filteredstate;
 }
 
 
 
void decodificar(){
  
  if (strcmp(code,".-") == 0) printarASCII(65);
  if (strcmp(code,"-...") == 0) printarASCII(66);
  if (strcmp(code,"-.-.") == 0) printarASCII(67);
  if (strcmp(code,"-..") == 0) printarASCII(68);
  if (strcmp(code,".") == 0) printarASCII(69);
  if (strcmp(code,"..-.") == 0) printarASCII(70);
  if (strcmp(code,"--.") == 0) printarASCII(71);
  if (strcmp(code,"....") == 0) printarASCII(72);
  if (strcmp(code,"..") == 0) printarASCII(73);
  if (strcmp(code,".---") == 0) printarASCII(74);
  if (strcmp(code,"-.-") == 0) printarASCII(75);
  if (strcmp(code,".-..") == 0) printarASCII(76);
  if (strcmp(code,"--") == 0) printarASCII(77);
  if (strcmp(code,"-.") == 0) printarASCII(78);
  if (strcmp(code,"---") == 0) printarASCII(79);
  if (strcmp(code,".--.") == 0) printarASCII(80);
  if (strcmp(code,"--.-") == 0) printarASCII(81);
  if (strcmp(code,".-.") == 0) printarASCII(82);
  if (strcmp(code,"...") == 0) printarASCII(83);
  if (strcmp(code,"-") == 0) printarASCII(84);
  if (strcmp(code,"..-") == 0) printarASCII(85);
  if (strcmp(code,"...-") == 0) printarASCII(86);
  if (strcmp(code,".--") == 0) printarASCII(87);
  if (strcmp(code,"-..-") == 0) printarASCII(88);
  if (strcmp(code,"-.--") == 0) printarASCII(89);
  if (strcmp(code,"--..") == 0) printarASCII(90);
  if (strcmp(code,".----") == 0) printarASCII(49);
  if (strcmp(code,"..---") == 0) printarASCII(50);
  if (strcmp(code,"...--") == 0) printarASCII(51);
  if (strcmp(code,"....-") == 0) printarASCII(52);
  if (strcmp(code,".....") == 0) printarASCII(53);
  if (strcmp(code,"-....") == 0) printarASCII(54);
  if (strcmp(code,"--...") == 0) printarASCII(55);
  if (strcmp(code,"---..") == 0) printarASCII(56);
  if (strcmp(code,"----.") == 0) printarASCII(57);
  if (strcmp(code,"-----") == 0) printarASCII(48);
  if (strcmp(code,"..--..") == 0) printarASCII(63);
  if (strcmp(code,".-.-.-") == 0) printarASCII(46);
  if (strcmp(code,"--..--") == 0) printarASCII(44);
  if (strcmp(code,"-.-.--") == 0) printarASCII(33);
  if (strcmp(code,".--.-.") == 0) printarASCII(64);
  if (strcmp(code,"---...") == 0) printarASCII(58);
  if (strcmp(code,"-....-") == 0) printarASCII(45);
  if (strcmp(code,"-..-.") == 0) printarASCII(47);
  if (strcmp(code,"-.--.") == 0) printarASCII(40);
  if (strcmp(code,"-.--.-") == 0) printarASCII(41);
  if (strcmp(code,".-...") == 0) printarASCII(95);
  if (strcmp(code,"...-..-") == 0) printarASCII(36);
  if (strcmp(code,"...-.-") == 0) printarASCII(62);
  if (strcmp(code,".-.-.") == 0) printarASCII(60);
  if (strcmp(code,"...-.") == 0) printarASCII(126);
  if (strcmp(code,".-.-") == 0) printarASCII(3);
  if (strcmp(code,"---.") == 0) printarASCII(4);
  if (strcmp(code,".--.-") == 0) printarASCII(6);
 
}
 

void printarASCII(int numeroASCII){
 Serial.print(char(numeroASCII));
}
 
