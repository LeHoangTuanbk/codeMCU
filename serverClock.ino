#include <ESP8266WiFiMulti.h> 
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <MFRC522.h>
#include <string.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ESP8266HTTPClient.h>

#define SS_PIN D4  
#define RST_PIN D3 
#define unLock D8
#define buzzer D0
const char* serverName = "http://172.20.10.3:3000/activity/rfid";

LiquidCrystal_I2C lcd(0x27,16,2);
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80



void unLockFuntion();
void wrongCardId();
void lcdDefault();
void buzzerRightIDCard();
void buzzerWrongIDCard();

void setup(void){
  Serial.begin(9600);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  wifiMulti.addAP("iPhone", "thegreatman");   // add Wi-Fi networks you want to connect to

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

  server.on("/unlock", HTTP_POST,unLockFuntion);

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");

  
  // khoi tao chan va lcd, MFRC
  pinMode(unLock, OUTPUT);
  digitalWrite(unLock, 0);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, 0);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  // khoi tao LCD 
  lcd.init();                    
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("LeHoangTuan");
  //lcd.clear();
  lcd.setCursor(2,1);
  lcd.print("DATN20192");

}
void loop(void){
  server.handleClient();                    // Listen for HTTP requests from clients
  if ( mfrc522.PICC_IsNewCardPresent()) 
      if ( mfrc522.PICC_ReadCardSerial())
      {
        Serial.println();
        Serial.print(" UID tag :");
        String content= "";
        byte letter;
        for (byte i = 0; i < mfrc522.uid.size; i++) 
        {
           content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : ""));
           content.concat(String(mfrc522.uid.uidByte[i], HEX));
        }
        content.toUpperCase();
        Serial.print(content);
        HTTPClient http;
      
        // Your Domain name with URL path or IP address with path
        http.begin(serverName);
        // Specify content-type header
        //http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        http.addHeader("Content-Type", "text/plain");
      // Data to send with HTTP POST
        String httpRequestData = content;           
      // Send HTTP POST request
        int httpResponseCode = http.POST(httpRequestData);
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        if( httpResponseCode == 200)
        {
          unLockFuntion();
        }
        else {
           wrongCardId();
        }
      // Free resources
        http.end();
      
        delay(1000);
      }
  
}

void unLockFuntion() {
  Serial.println("Da mo khoa");
  digitalWrite(unLock,1);
  //xuat thong bao ra LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Da mo khoa cua");
  // bao chuong
  buzzerRightIDCard();
  // tra ve trang thai default
  lcdDefault();
  digitalWrite(unLock,0);
  delay(1000);
  server.send(200); // 200 mean ok!
}
void wrongCardId()
{
  // hien thi ra LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ma the khong co");
    buzzerWrongIDCard();
    lcdDefault();
}
void lcdDefault()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Quet the de");
  lcd.setCursor(0,1);
  lcd.print("mo cua");
  
}

void buzzerRightIDCard()
{
  digitalWrite(buzzer,1);
  delay(2000);
  digitalWrite(buzzer,0);
  delay(50);
}

void buzzerWrongIDCard()
{
  // buzzer keu 5 lan
  digitalWrite(buzzer,1);
  delay(300);
  digitalWrite(buzzer,0);
  delay(50);
  digitalWrite(buzzer,1);
  delay(300);
  digitalWrite(buzzer,0);
  delay(50);
  digitalWrite(buzzer,1);
  delay(300);
  digitalWrite(buzzer,0);
  delay(50);
  digitalWrite(buzzer,1);
  delay(300);
  digitalWrite(buzzer,0);
  delay(50);
  digitalWrite(buzzer,1);
  delay(300);
  digitalWrite(buzzer,0);
  delay(50);
}
