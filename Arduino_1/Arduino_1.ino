//Librerias
#include <SoftwareSerial.h>
#include <OLED.h>
#include <EEPROM.h>
#include "EEPROMAnything.h"
#include <Timer.h>

#define RX_PIN A5// RX Pantalla OLED
#define TX_PIN A4//TX Pantalla OLED
#define RESET_PIN A3//Reset OLED
#define BAUD_RATE 57600//OLED

#define PUNT A2//Reset OLEDy
/*
 *LED 1 y pulsadpor 1 
 */
#define LED1 9
#define PULSADOR1 2
/*
 *LED 4 y pulsadpor 4 
 */
#define LED4 8
#define PULSADOR4 5
/*
 *LED 4 y pulsadpor 5 
 */
#define LED5 7
#define PULSADOR5 6
/*
 *LED 5 y pulsadpor 1 
 */
#define PULSADOR2 3
#define ZUMBADOR1 10
/*
 *zumbador 1 y pulsadpor 1 
 */
#define PULSADOR3 4
#define ZUMBADOR2 11
/*
 *zumbador 2 y pulsadpor 1 
 */
#define LED2 12
#define SENSOR1 A0
/*
 *LEd 2 y Sensor 1 1 
 */
#define LED3 13
#define SENSOR2 A1
//Definición de estados
#define ENESPERA      1 //Espera encendido a la interaccion con Usuario
#define FUNCIONANDO       2 //Funcionamiento del juego
#define FINALIZAR         3 //Termina con interaccion Bluetooth

//Variables para la máquina de estados
byte estado = ENESPERA;
byte subestado;
unsigned long tiempoAnterior;
boolean standBy = true;
boolean jugando = false;

struct player
{
    char nombre[14];
    int pMaxima;
};
int Puntos = 0;//puntos
int opcion;
int correcto=0;// Si e correcto el boton pulsado
OLED myOled; //
int fallos=10; //Fallos Permitidos
int Jugando=0;
boolean nombre=false;
//Base de datos
int punteroBD=0;
int numJugadores=0;
char datos[5];
int posY=0;
int Veces=4;
Timer t;
Timer T;
Timer Cont;

int pantalla=0;
int inicios [6] ={0, 100, 200, 300,400,500};
 struct player jugador = {0};

//Inicializa los LEDS utilizados 
void inicializarLeds()
{
    pinMode(LED1, OUTPUT);
    pinMode(LED4, OUTPUT);
    pinMode(LED5, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
}
void establecerNumJugadores(){
 numJugadores=5;
EEPROM_writeAnything(inicios[6],numJugadores);
   EEPROM_readAnything(inicios[6], numJugadores);
}
//inicia los temporizadores para el fin de partida de la OLED
void establecerTemporizadores(){
   t.every(4000,limpiar);
   T.every(4000,dibujarJugadores);
   Cont.every(8000,contMostrar);
}

//Inicializa los pulsadores
void inicializarPulsadores()
{
   pinMode(PULSADOR3, INPUT_PULLUP);
   pinMode(PULSADOR2, INPUT_PULLUP);
   pinMode(PULSADOR5, INPUT_PULLUP);
   pinMode(PULSADOR4, INPUT_PULLUP);
   pinMode(PULSADOR1, INPUT_PULLUP);
  
}
void inicialiizarMyOled()
{
   myOled.begin(RX_PIN, TX_PIN, RESET_PIN, BAUD_RATE);
}
void inicializarPuntuaciones()
{
   pinMode(PUNT,OUTPUT);
}
void inicializarZumbadores()
{ 
  pinMode(ZUMBADOR2, OUTPUT);
  pinMode(ZUMBADOR1, OUTPUT);
}

void funcionEnEspera()
{
  if(Serial.available())       //Si hay datos
  {
        Serial.readBytes(jugador.nombre, 14);
        standBy = false;
        jugando = true;
        estado = FUNCIONANDO;
        Jugando=1;
        digitalWrite(13, LOW);
        Serial.print("OK");
         guardar();

  }
     myOled.drawUnformattedString(0, 50, 2, 2, "Nombre", 0x001F);  
         delay(400);
}

void comprobarBluetooth()
{
   if( Serial.available() )       //Si hay datos
        {
          int bit = Serial.read();
          if (bit == 'F')
          {
            digitalWrite(13, HIGH);
             standBy = true;
             jugando = false;
             Jugando = -1;
             estado = FINALIZAR;
          }
}
}

void loopOled()
{
  //myOled.drawImageFromSD(40, 50, 22, 22, OLED_COLOUR16BITS, 0x000001);
  delay(2000);
}

//Parte de inicializacion del programa
void setup() {
  
  inicializarLeds();
  inicializarPulsadores();
  inicializarZumbadores();
  inicializarPuntuaciones();
  inicialiizarMyOled();
  establecerTemporizadores();
  establecerNumJugadores();
  //prueba
  Serial.begin(9600);
}


void loop() {
 /*struct player j1 = {0};
  String N="Jon";
N.toCharArray(j1.nombre,14);
jugador.pMaxima=0;
 EEPROM_writeAnything(inicios[0],j1);
 N="Javi";
 N.toCharArray(j1.nombre,14);
 EEPROM_writeAnything(inicios[1],j1);
N="Iker";
N.toCharArray(j1.nombre,14);
 EEPROM_writeAnything(inicios[2],j1);
N="Kevi";
N.toCharArray(j1.nombre,14);
 EEPROM_writeAnything(inicios[3],j1);
N="Sergi";
N.toCharArray(j1.nombre,14);
 EEPROM_writeAnything(inicios[4],j1);
*/
  switch(estado)
  {
  case ENESPERA:
      //Codigo ejecutado mientras está en espera de interaccion
      //Loop de animación en pantalla OLED
      funcionEnEspera();
      loopOled();
    break;
  case FUNCIONANDO:
  if(fallos>0 && Jugando==1){
      comprobarSiEsCorrecto();//Codigo ejecutado para el funcionamiento
      seleccionarMovimiento();//Se enciende un LED o se activa un Zumbador
      //X segundos para pulsar el pulsador asociado (determinado por el potenciometro)
      //(AL MOSTRAR SE PASA AL ESTADO DE VERIFICAR)
      comprobarBluetooth();
  }
    break;
  case FINALIZAR:
      //Ha saltado una interrupcion del Serial, ya que sea enviado por Bluetooth (FINALIZAR)
      //(OPCIONAL) Se muestra un pequeño Ranking con los puntos anteriormente obtenidos
      finalizar();
      
    break;
  }
  delay(10);
}
void comprobarSiEsCorrecto(){
  if(correcto==1&&fallos>0){
    digitalWrite(PUNT,HIGH); 
    delay(100); 
    digitalWrite(PUNT,LOW); 
     Puntos++;
    
  }
  else if(correcto==-1&&fallos>0){
    fallos=fallos-1;
  }
    correcto=0;
}
void seleccionarMovimiento(){
  if(fallos>0){
  Veces=5; //Mostrar la puntuacion final
 opcion = random(0, 7);  
   // genera número aleatorio entre 0 y 7
  switch (opcion) {
    case 0:
    case 1:
    case 2:
      {
        pulsador(opcion);
        break;
      }

    case 3:
    case 4:
      {
        zumbador(opcion);
        break;
      }
    case 5:
    case 6:
      {
        sensor(opcion);
        break;
      }
  }}
  else if(fallos==0){
    myOled.clear();
    myOled.setBackcolor(8);
    jugador.pMaxima=Puntos;
        guardar(); 
    Jugando=-1;
    estado=FINALIZAR; 
     Veces--;
  }
  }
void finalizar(){
 if(fallos<=0&&Veces>0&&Veces<5){
  dibujarTabla();
  t.update();
  T.update();
  Cont.update();
  }
else if(fallos<=0&&Veces==0){
  myOled.clear();
  myOled.drawUnformattedString(0, 0, 2, 2, "GAME OVER", 0x001F);
  delay(400);
  myOled.drawUnformattedString(0, 25, 2, 2, "Presionar", 0x001F);
  delay(400);
  myOled.drawUnformattedString(0, 75, 2, 2, "Boton", 0x001F);
  delay(400);
  myOled.drawUnformattedString(0, 100, 2, 2, "para reset", 0x001F);
  Veces--;
  delay(3000);
}
   else if(fallos<=0&&Veces<0){
    myOled.clear();
    Jugando=0;
    fallos=10;
    estado=ENESPERA;
    nombre=false;
    pantalla=0;
    }
}
void pulsador(int opcion) {
  switch (opcion) {
    case 0: //Led Amarillo
      {   myOled.clear();
           delay(50);
          myOled.setBackcolor(0x06669);
      
          delay(400);
          myOled.drawUnformattedString(20, 50, 2, 2, "Pulsador", 0x001F);
         while (correcto==0) {
          digitalWrite(LED1, HIGH);
             esCorrecto(opcion);
        }
        digitalWrite(LED1, LOW);
        break;
      }
    case 1://AMARILLO BIEN
      {    myOled.clear();
          delay(50);
          myOled.setBackcolor(0xFFFF00);
          delay(400);
          myOled.drawUnformattedString(20, 50, 2, 2, "Pulsador", 0x001F);
        while (correcto==0) {
          digitalWrite(LED4, HIGH);
             esCorrecto(opcion);
        }
        digitalWrite(LED4, LOW);
        break;
      }
    case 2://ROJO BIEN
      {    myOled.clear();
          delay(50);
          myOled.setBackcolor(0x008000);
          delay(400);
          myOled.drawUnformattedString(20, 50, 2, 2, "Pulsador", 0xFF0000);
         while (correcto==0) {
          digitalWrite(LED5, HIGH);
             esCorrecto(opcion);
        }
        digitalWrite(LED5, LOW);
        break;
      }
  }
}

void sensor(int opcion) {
 switch (opcion)
  {
    case 5://blanco
      {   myOled.clear();
          delay(50);
          myOled.setBackcolor(0x3300FF);
          delay(400);
          myOled.drawUnformattedString(20,50, 2, 2, "Sensor",  0x663300a);
     
        while (analogRead(SENSOR1) <= 200) {
          digitalWrite(LED2, HIGH);
             
        }
        correcto=1;
        digitalWrite(LED2, LOW);
        break;
      }
    case 6://azul
      {    myOled.clear();
           delay(50);
          myOled.setBackcolor(0xFFFFFF);
          delay(400);
          myOled.drawUnformattedString(20, 50, 2, 2, "Sensor", 0x001F);
        while (analogRead(SENSOR2) <= 200) {
        
          digitalWrite(LED3, HIGH);
        }
          correcto=1;
        digitalWrite(LED3, LOW);
        break;
      }
  }
}

void zumbador(int opcion) {

  switch (opcion) {

    case 3:
      {   
          myOled.clear();
          delay(50);
          myOled.setBackcolor(0x000F);
          delay(400);
          myOled.drawUnformattedString(20, 50, 2, 2, "Zumbador", 0x663300);
        while (correcto==0) {
          tone(ZUMBADOR1, 700);
             esCorrecto(opcion);
          
        }
        noTone(ZUMBADOR1);
        break;
      }
    case 4:
      {   myOled.clear();
          delay(50);
          myOled.setBackcolor(0x000F);
          delay(400);
          myOled.drawUnformattedString(20, 50, 2, 2, "Zumbador", 0x663300);
        while (correcto==0) {
          tone(ZUMBADOR2, 350);
          esCorrecto(opcion);
        }
        noTone(ZUMBADOR2);
        break;
      }
  }

}


void esCorrecto(int pos){
  for(int i=0;i<7;i++){

    switch (opcion) {
    case 0:
     {
          if(digitalRead(PULSADOR1) == LOW){
      correcto=1;
    }else if(digitalRead(PULSADOR4) == LOW||digitalRead(PULSADOR3) == LOW||digitalRead(PULSADOR2) == LOW||digitalRead(PULSADOR5) == LOW){
      correcto=-1;
    }
        break;
      }
    case 1:
     {
     if(digitalRead(PULSADOR4) == LOW){
      correcto=1;
    }else if(digitalRead(PULSADOR1) == LOW||digitalRead(PULSADOR3) == LOW||digitalRead(PULSADOR2) == LOW||digitalRead(PULSADOR5) == LOW){
      correcto=-1;
    }
        break;
      }
    case 2:
      {
     if(digitalRead(PULSADOR5) == LOW){
      correcto=1;
    }else if(digitalRead(PULSADOR1) == LOW||digitalRead(PULSADOR3) == LOW||digitalRead(PULSADOR2) == LOW||digitalRead(PULSADOR4) == LOW){
      correcto=-1;
    }
        break;
      }

    case 3:
      {
          if(digitalRead(PULSADOR2) == LOW){
      correcto=1;
    }else if(digitalRead(PULSADOR5) == LOW||digitalRead(PULSADOR3) == LOW||digitalRead(PULSADOR2) == LOW||digitalRead(PULSADOR4) == LOW){
      correcto=-1;
    }
        break;
      }
    case 4:
      {
          if(digitalRead(PULSADOR3) == LOW){
      correcto=1;
    }else if(digitalRead(PULSADOR1) == LOW||digitalRead(PULSADOR5) == LOW||digitalRead(PULSADOR2) == LOW||digitalRead(PULSADOR4) == LOW){
      correcto=-1;
    }
        break;
      }
    case 5:
        {
     if( analogRead(SENSOR1) <= 200){
      correcto=1;
    }else if(digitalRead(PULSADOR1) == LOW||digitalRead(PULSADOR5) == LOW||digitalRead(PULSADOR3) == LOW||digitalRead(PULSADOR2) == LOW||digitalRead(PULSADOR4) == LOW){
      correcto=-1;
    }
        break;
      }
    case 6:
      {
           if( analogRead(SENSOR2) <= 200){
      correcto=1;
    }else if(digitalRead(PULSADOR1) == LOW||digitalRead(PULSADOR5) == LOW||digitalRead(PULSADOR3) == LOW||digitalRead(PULSADOR2) == LOW||digitalRead(PULSADOR4) == LOW){
      correcto=-1;
    }
        break;
      }
  }
 }
}
void contMostrar(){
  Veces--;
}

void limpiar(){ //Limpia la pantalla cada 2 segundos
  myOled.clear();
}

void guardar()//Guarda el jugador en la EEprom
{  
  int existe=-1;
   struct player p1 = {0};
  for(int i=0;i<5;i++){
    EEPROM_readAnything(inicios[i], p1);
    String n1=jugador.nombre;
    String n2=p1.nombre;
   if(n1.equals(n2)){
   existe=i;

  }
 }
if(existe!=-1){
     EEPROM_writeAnything(inicios[existe],jugador);
}
else if(numJugadores<5){
   EEPROM_writeAnything(inicios[numJugadores],jugador);
   EEPROM_writeAnything(inicios[6],numJugadores);
}
else{
      EEPROM_writeAnything(inicios[2],jugador);   
}
}

void dibujarTabla(){//dibuja la tabla de los jugadores
  delay(50);
 myOled.drawLine(100, 0, 100, 200, 0x001F);
   delay(50);
 myOled.drawLine(0, 50, 200, 50, 0x001F); 
   delay(50);
 myOled.drawLine(0, 100, 200, 100, 0x001F); 
}

void dibujarJugadores(){
  dibujarTabla();
   struct player p1= {0};
  if(pantalla==0){
     for(int i =0; i<=numJugadores-3; i++)
  // for(int i =0; i<=2; i++)
    {
      EEPROM_readAnything(inicios[i], p1);
      dibujarJugador(p1);
            
      }
  }
if(pantalla==1){
  for(int i=3;i<=numJugadores-1;i++ ){
    //  for(int i=3;i<=4;i++ ){
        EEPROM_readAnything(inicios[i], p1);
       dibujarJugador(p1);
       Serial.println(numJugadores);
   }}
 if(pantalla==0){
   pantalla=1;
    posY=0; 
   }
else if(pantalla==1){
    pantalla=0;
    posY=0; 
   }
}

void dibujarJugador(player p1){      //Dibuja un jugadore en la poscion indicada por la variable posY con su puntuacion;
      String p=String(p1.pMaxima);
      delay(200);
      myOled.drawUnformattedString(5, posY, 2, 2, p1.nombre, 0x0012);
      char Puntuacion[4];
      p.toCharArray(Puntuacion,4);
      delay(200);
      myOled.drawUnformattedString(100, posY, 2, 2,Puntuacion, 0x0012);
      posY=posY+50;
      if(posY>=150){
        posY=0;
      }
}

void limpiarArray(){
      for( int i = 0; i < sizeof(datos);  ++i )
    {
      datos[i] = (char)0;
    }
}


