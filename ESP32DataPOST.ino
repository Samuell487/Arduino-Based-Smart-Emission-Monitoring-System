#include <RunningAverage.h>
#include <WiFi.h> 
#include <HTTPClient.h> 
#define RXD2 16
#define TXD2 17

String URL = "http://192.168.100.31/trial/index.php";

const char* ssid = "FathWifi"; 
const char* password = "1234567"; 

RunningAverage RAC(100);
RunningAverage RAH(100);

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); 
  connectWiFi();
  RAC.clear(); 
  RAH.clear();
}

void loop() {
    float value1, value2, value3;
    String hasil = "FAILED";
    


    if(WiFi.status() != WL_CONNECTED) { 
    connectWiFi();
  }
    if (Serial2.available() >= sizeof(float) * 2) {
        Serial2.readBytes((char*)&value1, sizeof(float));
        Serial2.readBytes((char*)&value2, sizeof(float));
        Serial2.readBytes((char*)&value3, sizeof(float));
        RAC.addValue(value2);
        RAH.addValue(value1);
        float HC = RAH.getAverage();
        float CO = RAC.getAverage();

        // Now you have received value1 and value2
        Serial.print("Received value1: ");
        Serial.print(HC,2); Serial.print(" "); Serial.println(value1);
        Serial.print("Received value2: ");
        Serial.print(CO,2); Serial.print(" "); Serial.println(value2);
        if(value3 = 1){
          hasil = "PASSED";
          Serial.print("Received value3: ");
          Serial.println(hasil);


        }

        String postData = "temperature=" + String(HC) + "&humidity=" + String(CO) + "&result=" + String(hasil); 

        HTTPClient http; 
        http.begin(URL);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
        int httpCode = http.POST(postData);
        String payload = http.getString(); 
  
       Serial.print("URL : "); Serial.println(URL); 
       Serial.print("Data: "); Serial.println(postData); 
       Serial.print("httpCode: "); Serial.println(httpCode); 
       Serial.print("payload : "); Serial.println(payload); 
    }

  Serial.println("--------------------------------------------------");

  delay(60000);
}



void connectWiFi() {
  WiFi.mode(WIFI_OFF);
  delay(1000);
  //This line hides the viewing of ESP as wifi hotspot
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    
  Serial.print("connected to : "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
}
