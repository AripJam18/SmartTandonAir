/*************************************** 
* Project Smart Home dg MIT App Inventor dan Firebase  
* www.ardutech.com 
 ****************************************/ 
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define FIREBASE_HOST "monitoring-86863-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "6H47RPs1niSveGhmHv84SvdxLSVpcVZ5ly5WjJLp" 

#define WIFI_SSID "Maxxis 😘"
#define WIFI_PASSWORD "Tuyul123"
#define valve D5
#define pump D6
#define trig D8
#define echo D7

FirebaseData firebaseData;
String c,s; 
int humi, limit;
float persen;
const int MSpin = A0;
long duration, jarak,tinggi, volume;

//Tangki air 500L dengan diameter=80 cm dan tinggi 100 cm
//rumus volume tabung =3.14 * r2 * T
// 3.14 x 40cm x 100 cm
// 40 * 40 = 1600 
// 1600/1000 = 1.6 cm3
//konversi dari cm3 ke liter = 1000:1
// 1 Liter = 1000 cm3

//nilai kelembaban tanah pada sensor analog
//0-300 = kering
//301-700 = Lembab
//701-950 = basah

//// ketika nilai analog 1024 maka persentase = 1%
//// ketika nilai analog 438-442 maka persentase = 58 %
//// ketika nilai analog 469-472 maka persentase = 55 %
//// ketika nilai analog 469-512 maka persentase = 50 %
//// berarti kalau nilai analog 700 maka persentasenya = 30 %
//// ketika nilai analog > 700 atau persentase kelembapan < 30 % maka perlu dilakukan penyiraman. 


//map(value, fromLow, fromHigh, toLow, toHigh);
//Penjelasan Sintaksis
//Fungsi map() memiliki 5 parameter yaitu value, fromLow, fromHigh, toLow dan toHigh. Berikut penjelasannya.
//
//value = nilai yang akan dipetakan.
//fromLow = batas bawah nilai awal.
//fromHigh = batas atas nilai awal.
//toLow = batas bawah rentang target nilai.
//toHigh = batas atas rentang target nilai.
//======================================
void sendMoisture()
{  
  humi = analogRead(MSpin);
  persen = map(humi,0,1024,0,100); // angka sensor aktual dijadikan persen
  Serial.print("Soil Moisture(in Percentage) = ");
  Serial.print(persen);
  Serial.println(" %");
  lcd.setCursor(0, 1);
  lcd.print("Arip James");
//  lcd.print("Kel. Tanah = ");
//  lcd.print(persen);
//  lcd.print(" %");
  
//  Firebase.setFloat(firebaseData,"/datasensor/humi",persen); 
//if (humi < limit) {
//  Firebase.setString(firebaseData,"/datasensor/tanah","kering"); 
//  Serial.println("Tanah kering perlu disiram!");
//} else {
//  Firebase.setString(firebaseData,"/datasensor/tanah","lembab"); 
//  Serial.println("Tanah lembab TIDAK perlu disiram!");
//}
  
}


//======[WaterLevel]=====
void WaterLevel(){
  digitalWrite(trig, LOW);
  delayMicroseconds(2); 
  digitalWrite(trig, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  jarak = (duration/2) / 29.1;
  tinggi = 100 - jarak;
  volume = (3.14 * 1.6) * tinggi;
  Serial.println("jarak sensor ke air :");
  Serial.print(jarak);
  Serial.println(" cm");
  Serial.println("ketinggian air :");
  Serial.print(tinggi);
  Serial.println(" cm");
  Serial.println("volume air :");
  Serial.print(volume);
  Serial.println(" Liter");
  lcd.setCursor(0, 0);
  lcd.print("Arip James");
//  lcd.print("Vol. Air =");
//  lcd.print(volume);
//  lcd.print(" %");   
//  Firebase.setFloat(firebaseData,"/datasensor/wlevel",volume);
//if ((volume >= 380) && (volume <= 500)) {
//  Firebase.setString(firebaseData,"/datasensor/air","penuh"); 
//  Serial.print("Tangki penuh tutup valve!");
//} else if ((volume >= 240) && (volume <= 379)) {
//  Firebase.setString(firebaseData,"/datasensor/air","Batas aman"); 
//  Serial.print("Tangki masih dalam batas aman!");
//}else if ((volume >= 120) && (volume <= 239)) {
//  Firebase.setString(firebaseData,"/datasensor/air","hampir kosong"); 
//  Serial.print("Tangki hampir kosong!");
//}else if ((volume >= 1) && (volume <= 119)) {
//  Firebase.setString(firebaseData,"/datasensor/air","kosong"); 
//  Serial.print("Tangki kosong buka valve!");
//}
//  delay(1000);
}

//============================================
void setup()
{
  Serial.begin(9600);
  pinMode(valve,OUTPUT);
  pinMode(pump,OUTPUT);
  pinMode(MSpin, INPUT); 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
  Serial.print(".");
  delay(300);  }
  Serial.println();
  Serial.print("Connected ..."); 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  lcd.init();                     
  lcd.backlight();
 }
//============================================
void loop()
{
    delay(1000);
    if(Firebase.get(firebaseData, "/datasensor/valve"))
    {   
      c=firebaseData.stringData(); 
      Serial.print("valve=");
      Serial.println(c);     
      digitalWrite(valve,(c.toInt()));
     }else{    
      Serial.print("Error in getInt valve, ");
      Serial.println(firebaseData.errorReason());
      }
    //===========
    if(Firebase.get(firebaseData, "/datasensor/pump"))
    {   
      c=firebaseData.stringData(); 
      Serial.print("pump=");
      Serial.println(c);     
      digitalWrite(pump,(c.toInt()));
     }else{    
      Serial.print("Error in getInt pump, ");
      Serial.println(firebaseData.errorReason()); }
    //============= 
      sendMoisture();
      delay(1000);
      WaterLevel();
      delay(1000);
    }
