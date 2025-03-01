#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <AverageValue.h>
#define pinSensormq7 A0
#define pinSensormq135 A1
Adafruit_SSD1306 display(128, 64, &Wire, -1);
const int RL = 1000;
const long MAX = 50;
long Ro7,Ro135;
int p = 1;
int k = 0;
int jarak = 0;
int tresholdCO,tresholdHC;

int counter=0;
unsigned long last_run=0;
int a = 5;
int b = 5;
int c = 1;
int d = 0;


AverageValue<long> averageValue(MAX);



void setup(){
  Serial.begin(9600); 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  attachInterrupt(digitalPinToInterrupt(3), shaft_moved, FALLING);
  pinMode(4,INPUT);
  pinMode(5, INPUT_PULLUP);
  DDRB = 0b00100000;
  display.clearDisplay();

  for(int i = 0; i<50; i++){

    int sensorvalue = analogRead(pinSensormq7); 
    float VRL= sensorvalue*5.00/1024;  
    float Rs7 = ( 5.00 * RL / VRL ) - RL;
    averageValue.push(Rs7);
    Rs7 = averageValue.average();
    Ro7 = Rs7/2.863;
  }

  Serial.println();

  //MQ 135
  for(int i = 0; i<50; i++){
    float adcRaw = analogRead(pinSensormq135);
    adcRaw = adcRaw*5.00/1024;
    double rS = ((5.00 * RL) / adcRaw) - RL; 
    averageValue.push(rS);
    rS = averageValue.average();
    Ro135 = rS/19.117;


  }





}


void shaft_moved(){
  if (millis()-last_run>5){
    if (digitalRead(4)==1){
      counter++;
      }
    if (digitalRead(4)==0){
      counter--; }
    last_run=millis();
  }
}



void loop(){

  float result = 1;


  if (digitalRead(5) == 0 && abs(counter) % 2 == 0){
    a = a-c;
    b = b+c;
  }
  if(digitalRead(5) == 0 && abs(counter) % 2 != 0){
    b = b-c;
    a = a+c;
  }
  delay(300);

  if(digitalRead(5) == 0){
    d = d+1;
  }
  if(d == 5){
    a = 5;
    b = 5;
    delay(500);
    d = 0;
  }

  //CO2
  float adcRaw = analogRead(pinSensormq135);
  adcRaw = adcRaw*5.00/1024;
  double rS = ((5.00 * RL) / adcRaw) - RL; 
  float ppmCO2 = 4306.46 * pow(rS / Ro135, -2.6219);

  //CO
  int sensorvalue = analogRead(A0); 
  float VRL= sensorvalue*5.00/1024;  
  double Rs = ( 5.00 * RL / VRL ) - RL;
  float ppm_CO = 100 * pow(Rs / Ro7,-1.53); // ppm = 100 * ((rs/ro)^-1.53);
  ppm_CO = ppm_CO / 10000;








  if(abs(counter)%2 == 0 && a==5){
    display.clearDisplay();
    display.fillRect(18, 5, 90, 20, SSD1306_WHITE);
    display.setTextSize(2); // Draw 2X-scale text
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(30, 7);
    display.println("MOBIL");
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(30, 40);
    display.println("MOTOR");
    display.display();
    c = 1;

  }

  if(abs(counter) % 2 != 0 && b == 5){
    display.clearDisplay();
    display.fillRect(18, 37, 90, 20, SSD1306_WHITE);
    display.setTextSize(2); // Draw 2X-scale text
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(30, 40);
    display.println("MOTOR");
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(30, 7);
    display.println("MOBIL");
    display.display();
    c = 1;


  }

    // 2ND
  if(abs(counter)%2 == 0 && a == 4){
       display.clearDisplay();
       display.fillRect(18, 5, 90, 20, SSD1306_WHITE);
       display.setTextSize(2); // Draw 2X-scale text
       display.setTextColor(SSD1306_BLACK);
       display.setCursor(30, 7);
       display.println("> 2007");
       display.setTextColor(SSD1306_WHITE);
       display.setCursor(30, 40);
       display.println("< 2007");
       display.display();
       c = 1;
       tresholdCO = 1.5;
       tresholdHC = 200;

  }




  if(abs(counter) % 2 != 0 && a == 4){
       display.clearDisplay();
       display.fillRect(18, 37, 90, 20, SSD1306_WHITE);
       display.setTextSize(2); // Draw 2X-scale text
       display.setTextColor(SSD1306_WHITE);
       display.setCursor(30, 7);
       display.println("> 2007");
       display.setTextColor(SSD1306_BLACK);
       display.setCursor(30, 40);
       display.println("< 2007");
       display.display();
       c = (c*-1);
       tresholdCO = 4.5;
       tresholdHC = 1200;
  }

  if(abs(counter)%2 == 0 && b == 4){
       display.clearDisplay();
       display.fillRect(18, 5, 90, 20, SSD1306_WHITE);
       display.setTextSize(2); // Draw 2X-scale text
       display.setTextColor(SSD1306_BLACK);
       display.setCursor(30, 7);
       display.println("> 2010");
       display.setTextColor(SSD1306_WHITE);
       display.setCursor(30, 40);
       display.println("< 2010");
       display.display();
       c = -3;
       tresholdCO = 4.5;
       tresholdHC = 2000;


  }




  if(abs(counter) % 2 != 0 && b == 4){
       display.clearDisplay();
       display.fillRect(18, 37, 90, 20, SSD1306_WHITE);
       display.setTextSize(2); // Draw 2X-scale text
       display.setTextColor(SSD1306_WHITE);
       display.setCursor(30, 7);
       display.println("> 2010");
       display.setTextColor(SSD1306_BLACK);
       display.setCursor(30, 40);
       display.println("< 2010");
       display.display();
       c = 1;


  }

  //3RD

  if(abs(counter)%2 == 0 && a == 7){
       display.clearDisplay();
       display.fillRect(18, 22, 90, 20, SSD1306_WHITE);
       display.setTextSize(2); // Draw 2X-scale text
       display.setTextColor(SSD1306_BLACK);
       display.setCursor(30, 25);
       display.println("4 tak");
       display.display();
       c = -1;
       tresholdCO = 5.5;
       tresholdHC = 2400;
  }




  if(abs(counter) % 2 != 0 && b == 3){
       display.clearDisplay();
       display.fillRect(18, 22, 90, 20, SSD1306_WHITE);
       display.setTextSize(2); 
       display.setTextColor(SSD1306_BLACK);
       display.setCursor(30, 25);
       display.println("2 tak");
       display.display();
       c = 1;
       tresholdCO = 4.5;
       tresholdHC = 12000;
  }


  if((b<3 && a>7) || (a<4 && b>6)){
    for(k ; k < p; k++){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(20,32);
    display.print("calibrating...");
    display.display();
    delay(5000);



   }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("HC (ppm): ");
  display.print(ppmCO2);
  // display.display();


  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,35);
  display.println("CO (%): ");
  display.print(ppm_CO);
  display.display();
  delay(500);

  if(ppmCO2 > tresholdHC && ppm_CO > tresholdCO ){
    result = 0;
    PORTB = 0b00100000;
    _delay_ms(1000);
    PORTB = 0b00000000;
    _delay_ms(1000);

   }

  }
  
  if (ppm_CO > 0.009 && ppmCO2 > 70){
     Serial.write((uint8_t*)&ppmCO2, sizeof(float));
     Serial.write((uint8_t*)&ppm_CO, sizeof(float));
     Serial.write((uint8_t*)&result, sizeof(float));
  }










}