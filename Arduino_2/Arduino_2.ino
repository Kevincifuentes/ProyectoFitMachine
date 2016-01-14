#include <LiquidCrystal.h>
//Display 7*4
#define a 1
#define b 2
#define c 3
#define d 4
#define e 5
#define f 6
#define g 7
#define p 8
int d4 = 9;
int d3 = 11;
int d2 = 10;
int d1 = 12;
#define pul 13
#include <Timer.h>
//define pantalla LCD
#define RS 0
Timer compr;
int preTime=0;
LiquidCrystal lcd(0,A1,A2,A3,A4,A5);
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int Num= 0000;
int punt=0000;
int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded! 
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat". 
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

// Regards Serial OutPut  -- Set This Up to your needs
static boolean serialVisual = true;   // Set to 'false' by Default.  Re-set to 'true' to see Arduino Serial Monitor ASCII Visual Pulse 

int static pulsaciones=60;
int pulsacionesAt;
byte corazonG[8] = {
  B00000,
  B11011,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000,
};

byte corazonP[8] = {
  B00000,
  B00000,
  B01010,
  B01110,
  B00100,
  B00000,
  B00000,
};
void setup() {
  // Inicializar 4*7 segmentos
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(p, OUTPUT);
  pinMode(pul,INPUT);
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);
  //Pantalla OLED pusaciones// 
  interruptSetup();  
  lcd.createChar(0, corazonG);
  lcd.begin(16, 2);
  lcd.setCursor(2,0);
  lcd.print("Pulsaciones:");
  compr.every(10000,comprobar);
}
boolean pulsado =false;
void loop() {
  // put your main code here, to run repeatedly:

  compr.update();
if(digitalRead(pul)==1 && !pulsado){
  pulsado=true; 
  Num++;
}
else
 {  if(digitalRead(pul)==0 && pulsado){
    pulsado=false;
  }
}
for(int i=12;i>8;i--){
  selecionarDigito(i);
  
  int Digit0 = Num %10 ;
  int Digit1 = (Num % 100) / 10 ;
  int Digit2 = (Num % 1000) / 100 ;
  int Digit3 = Num / 1000;
  
  if(i==12){
    selecionarNumero(Digit2); 
  }
  
  else if(i==11){
    selecionarNumero(Digit1);
  }
    else if(i==10){
     selecionarNumero(Digit0);
  }
    else if(i==9){
     selecionarNumero(Digit3);
  }
}
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);

  if (QS == true){     
        pulsaciones = BPM;
        if(pulsaciones<100){
          lcd.setCursor(9,1);
          lcd.print(" "); 
        }
        QS = false;                       
  }
  lcd.setCursor(7, 1);
  lcd.print(pulsaciones);
 
  if(pulsaciones%2==0){
      lcd.createChar(1, corazonP);
      lcd.setCursor(6,1);
      lcd.write(byte(1));
  }
  else{
    lcd.setCursor(6,1);
     lcd.write(byte(0));
  }

}
void comprobar(){
  if(Num==punt){
    Num=0;
  }
  else{
    punt=Num;
  }
}
void selecionarNumero(int x)   
{
  switch(x)
  {
  default: 
    zero(); 
    break;
  case 1: 
    uno(); 
    break;
  case 2: 
    dos(); 
    break;
  case 3: 
   tres(); 
    break;
  case 4: 
    cuatro(); 
    break;
  case 5: 
    cinco(); 
    break;
  case 6: 
    seis(); 
    break;
  case 7: 
    siete(); 
    break;
  case 8: 
    ocho(); 
    break;
  case 9: 
    nueve(); 
    break;
  }
}
void selecionarDigito(int x)  
{
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);
  switch(x)
  {
  case 12: 
    digitalWrite(d1, LOW); 
    break;
  case 11: 
    digitalWrite(d2, LOW); 
    break;
  case 10: 
    digitalWrite(d3, LOW); 
    break;
  default: 
    digitalWrite(d4, LOW);
    break;
  }
  delay(1);
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);
}

 
void zero() 
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
}
 
void uno()  
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}
 
void dos() 
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}
 
void tres()  
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e,LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);

}
 
void cuatro()  
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}
 
void cinco()  
{
  digitalWrite(a,HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}
 
void seis()  
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}
 
void siete()  
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d,LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}
 
void ocho() 
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d,HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}
 
void nueve()  
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c,HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}
int getDigit(unsigned int number, int digit) {
    for (int i=0; i<digit-1; i++) { 
      number /= 10; 
    }
    return number % 10;
}


