#include <Wire.h> //INCLUSÃO DA BIBLIOTECA
#include <idDHT11.h> //INCLUSÃO DA BIBLIOTECA
#include "RTClib.h" //INCLUSÃO DA BIBLIOTECA
#include "dht.h" //INCLUSÃO DE BIBLIOTECA
#include <LiquidCrystal_I2C.h>

const int RelePin = 7; //PINO DIGITAL UTILIZADO PELO RELÉ
const int pinoDHT11 = A2; //PINO ANALÓGICO UTILIZADO PELO DHT11

dht DHT; //VARIÁVEL DO TIPO DHT


RTC_DS3231 rtc; //OBJETO DO TIPO RTC_DS3231

// Config. Sensor Solo

#define pinSensorA A0

// Config. LCD 

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); //Inicializa o display no endereco 0x27
 
int posi_LCD = 0 ;
int length = 26;
int umidadeSl = 700;

//DECLARAÇÃO DOS DIAS DA SEMANA
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};

void setup(){
  lcd.init(); // INICIALIZA LCD
  pinMode(RelePin, OUTPUT);  // SETA PINO DA VENTOINHA COMO SAÍDA
  Serial.begin(9600); //INICIALIZA A SERIAL
  if(! rtc.begin()) { // SE O RTC NÃO FOR INICIALIZADO, FAZ
    Serial.println("DS3231 não encontrado"); //IMPRIME O TEXTO NO MONITOR SERIAL
    while(1); //SEMPRE ENTRE NO LOOP
  }
  else{
      Serial.println("DS3231 OK!");
      header();
  }

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //CAPTURA A DATA E HORA EM QUE O SKETCH É COMPILADO
  //rtc.adjust(DateTime(2024, 8, 12, 11, 30, 15)); //(ANO), (MÊS), (DIA), (HORA), (MINUTOS), (SEGUNDOS)

  delay(100); //INTERVALO DE 100 MILISSEGUNDOS
}

void registro () {
    DateTime now = rtc.now(); //CHAMADA DE FUNÇÃO
    //Serial.print("Data: "); //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.print(now.day(), DEC); //IMPRIME NO MONITOR SERIAL O DIA
    Serial.print('/'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.month(), DEC); //IMPRIME NO MONITOR SERIAL O MÊS
    Serial.print('/'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.year(), DEC); //IMPRIME NO MONITOR SERIAL O ANO
    Serial.print(" ; "); //IMPRIME O TEXTO NA SERIAL
    //Serial.print(" Dia: "); //IMPRIME O TEXTO NA SERIAL
    //Serial.print(daysOfTheWeek[now.dayOfTheWeek()]); //IMPRIME NO MONITOR SERIAL O DIA
   // Serial.print(" ; "); //IMPRIME O TEXTO NA SERIAL
    //Serial.print(" Horas: "); //IMPRIME O TEXTO NA SERIAL
    Serial.print(now.hour(), DEC); //IMPRIME NO MONITOR SERIAL A HORA
    Serial.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.minute(), DEC); //IMPRIME NO MONITOR SERIAL OS MINUTOS
    Serial.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.second(), DEC); //IMPRIME NO MONITOR SERIAL OS SEGUNDOS
    Serial.print(" ; ");
    delay(1000); //INTERVALO DE 1 SEGUNDO
}

void dht11() {
    DHT.read11(pinoDHT11); //LÊ AS INFORMAÇÕES DO SENSOR
    //Serial.print("Umidade do ar: "); //IMPRIME O TEXTO NA SERIAL
    Serial.print(DHT.humidity, 0); //IMPRIME NA SERIAL O VALOR DE UMIDADE MEDIDO
    Serial.print(" ; "); //ESCREVE O TEXTO EM SEGUIDA
    //Serial.print("Temperatura do ar: "); //IMPRIME O TEXTO NA SERIAL
    Serial.print(DHT.temperature, 0); //IMPRIME NA SERIAL O VALOR DE UMIDADE MEDIDO E REMOVE A PARTE DECIMAL
    //Serial.print("°C"); //IMPRIME O TEXTO NA SERIAL
    Serial.print(" ; ");
}

void solo() {
  //Serial.print("Umidade do solo: ");
  if (analogRead(pinSensorA) > umidadeSl) {
       Serial.print("seco");
    } else {
       Serial.print("molhado");
    }
    Serial.println("  ");
  }

void fan() {
    //VARIÁVEL PARA TEMPERATURA EXTERNA PELO MÓDULO RTC
  float ExTemp = rtc.getTemperature();
  if ( ExTemp < 24) {  
    digitalWrite(RelePin, HIGH); // DESLIGA VENTOINHA
  }
  else {
    digitalWrite(RelePin, LOW); // LIGA VENTOINHA
  }
}

void header() {
  Serial.println("Data ; Hora ; Umidade do ar (%) ; Temperatura do ar (°C) ; Umidade do solo ");
}

void lcdPrint1() {
  lcd.setCursor(1, 0);            
  lcd.print("  Lavoisier  ");      
  lcd.setCursor(2, 1);            
  lcd.print("Etec Lorena");
  delay(10000); 
  lcd.clear();
}

void lcdPrint2() {
  lcd.setCursor(0, 0);            
  lcd.print("Temper.: ");
  lcd.print(DHT.temperature, 0);
  lcd.print((char) 223);
  lcd.print("C");      
  lcd.setCursor(2, 1);            
  lcd.print("Umid.: ");
  lcd.print(DHT.humidity, 0);
  lcd.print("%");           
  delay(10000); 
  lcd.clear();
}

void lcdPrint3() {
  lcd.setCursor(2, 0);            
  lcd.print("Umid. Solo: ");

  if (analogRead(pinSensorA) > umidadeSl) {
    lcd.setCursor(6, 1); 
    lcd.print("seco");
  } else {
    lcd.setCursor(6, 1); 
     lcd.print("molhado");
  }
          
  delay(10000); 
  lcd.clear();
}

void loop() {
    lcd.setBacklight(HIGH); // ACENDE BACKLIGHT DO LCD
    registro();
    dht11();
    solo();
    fan();
    lcdPrint1();
    lcdPrint2();
    lcdPrint3();
    lcd.setBacklight(LOW); // APAGA BACKLIGHT DO LCD
    delay(100000);
}