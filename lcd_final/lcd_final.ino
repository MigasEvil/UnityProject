#include <HCSR04.h>

#include <IRremote.hpp>
#include <DFRobot_DHT11.h>
#include <LiquidCrystal.h>
 

#define pino_sensor_temp 6
#define ventilador 7 // hardware por relé

const int trig = 9;
const int echo =  10;

int ledArray[] =  {A4,A3,A2,A1,A0};
int buzzer = A5;
int intensidade;

String warn = "4";

HCSR04 hc(trig, echo); //initialisation class HCSR04 (trig pin , echo pin)
DFRobot_DHT11 sensor_temp_hum;
LiquidCrystal lcd (12, 11, 5, 4, 3, 2);


void setup(){
  
  Serial.begin(9600);
  lcd.begin(16, 2);

  pinMode(ventilador,OUTPUT);
  IrReceiver.begin(8, ENABLE_LED_FEEDBACK);
  
  for (int i = 0;i < 5; i++) {
  pinMode(ledArray[i],OUTPUT);
  }

  pinMode (buzzer,OUTPUT);

}
 
void loop(){
  
  switch (Serial.read())
    {
        case 'L':
            digitalWrite(ventilador,LOW);
            warn = "4";
            break;
        case 'D':
            digitalWrite(ventilador,HIGH);
            warn = "3";
            break;
        case 'Y':
            Serial.println(warn);
            break;
    }

  float distancia=hc.dist();
  intensidade=map(distancia,30,180,0,6);
  //Serial.println(distancia);
  for(int i=5;i>=0;i--){
    if(i>=intensidade && distancia>5.00 && distancia<180.00){
      digitalWrite(ledArray[i],1);
       if(i<=2){
      tone(buzzer,5000);
      }
      else{
      noTone(buzzer);
      }
    }
    else{
      digitalWrite(ledArray[i],0);
    }
  }

  if(distancia<5.00 || distancia>180){
    noTone(buzzer);
  }

  delay(50);

//---------------------------------------------------------------------------------
  sensor_temp_hum.read(pino_sensor_temp);

  float t = sensor_temp_hum.temperature;
  float h = sensor_temp_hum.humidity;
  
//--------------------------------------------------------------------------------
  
  if(t<15){
    refrigerar(1);
    delay(10000);
  }

  if ( isnan(t) || isnan(h) ){
    lcd.setCursor(0, 0);
    lcd.print("Falha na leitura do sensor");
  }
  else {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Humidade: ");
    lcd.print(h);
    lcd.print("%");

    Serial.print("Humidade: ");
    Serial.print(h);
    Serial.println("%");

    lcd.setCursor(0,1);
    lcd.print("Temperatura:");
    lcd.print(t);
    lcd.print("C");

    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println("C");

    delay(150);
  }
}

void refrigerar(int estado){
  if(estado){
    digitalWrite(ventilador,HIGH);
  }
  else{
    digitalWrite(ventilador,LOW);
  }
}