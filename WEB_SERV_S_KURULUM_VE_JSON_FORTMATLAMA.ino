/*WEB SERVİS KURULUM VE JSON FORTMATLAMA*/
/* http bağlantı örnek kodlar*/
/*http istek kodları*/
/*http gelen verileri görüntüleme*/
/* JSON Formatıyla Çalışmak*/ /*KAYNAK SİTE = json.org/json-tr.html*/
//!!!!!DİKKAT!!!== ArduinoJson 5.12.0 kütüphanesi kurulunca çalışıyor bu kodlar(ArduinoJson5 kurulu olmalı,ArduinoJson6 da çalışmıyor KODLAR!!)
#include <ESP8266WiFi.h> //
#include <ESP8266HTTPClient.h>//HTTP KÜTÜPHANESİ 1
#include <WiFiClient.h>//HTTP KÜTÜPHANESİ 2
#include <ArduinoJson.h> //ArduinoJson kütüphanesini include ediyoruz


//ESP'mizin bağlanacağı wifi adresinin Id ve password giriyoruz.
const char* ssid="YOUR SSID";
const char* password="YOUR PASSWORD";


boolean wifi_status=false;//wifi bağlanıp bağlanmadığını belirten boolean tipinde değişken

const char* host="http://jsonplaceholder.typicode.com/posts/1";//http istek gönderme sabiti. bu sayede esp bu siteden bilgi alabiliyor.//http isteği gönderiyoruz

void setup() {
  // put your setup code here, to run once:

Serial.begin(115200);//seri mönüter öçalıştırıldı. bağlanma sürüceni adım adım seri mönüterde görüyoruz.
Serial.println("");
Serial.print("Connecting to ");
Serial.println(ssid);

WiFi.mode(WIFI_STA);//wifi durumunu belirttik
WiFi.begin(ssid,password);//wifi başlamasını sağladık

int try_count=0;
while(WiFi.status()!= WL_CONNECTED)
{
  Serial.print(".");//nokta animasyonu yapar.
  try_count++;
  if(try_count>16)break;
  delay(500);
}
if(WiFi.status()== WL_CONNECTED)//wifi bağlandığında wifi statusünü true yapıp seri mönütere bilgi verir
{
   wifi_status=true;
   Serial.println("wifi ağına bağlanıldı");
}
else{
  Serial.println("wifi ağına bağlanılamadı");
}

if(wifi_status)
{
  WiFiClient client;//WiFiClient kütüphanemizi "client" ismiyle çağırıyoruz.
  HTTPClient http;// HttpClient çağırıyoruz.
  const int httpPort = 80; //http portu belirledik. int sabitiyle portumuz belirlendi.80 http nin standart portudur.

  if(http.begin(client,host))//http mizi başlatıyoruz. host== bizim göndereceğimiz servisin ismi //http isteği gönderiyoruz ve bu isteği if-else ile denetliyıruz
  {
     Serial.println("http isteği gönderildi");
     
     int httpCode = http.GET();//burada bağlantı kuruyoruz."GET" Metoduyla istekte bulunuyoruz web servisinden.
    
     if(httpCode>0)//http kod sıfır değilse yani hata çıkmadıysa, kod çalışır.
     {
      Serial.printf("[HTTP] GET... code: %d\n",httpCode);//kodu ekrana yazdırıyor...."[HTTP] GET... code: 200" ekrana yazılan kod
      
       if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY){

        String payload = http.getString(); // http den gelen veriyi String hale getirip payload'a atıyor. kod 
        
        Serial.println(payload);

        StaticJsonBuffer<3000> jsonBuffer; //burada JsonBuffer isminde bir değişken oluşturduk.3000 değeri bellekte kapladığı değer

         /*"jsonBuffer.parseObject(payload)" = payload dan gelen 'metni//veriyi' objeye dönüştürüyoruz.:: " JsonObject& jsonRoot" = başka dataların gelmesini önlüyoruz, jsonobjesi oluşturarak önlüyoruz. 
          dönüştürülen obje jsonRoot a atanıyor */
         JsonObject& jsonRoot = jsonBuffer.parseObject(payload);

         if(jsonRoot.success())//jsonRoot şart içinde çalıştırıyoruz// başarılıysa döndürme işlemi okuyabiliriz.
         {
          String title=jsonRoot["title"]; // title değişken atıyoruz.
           
           Serial.println("Title verisi = " +title); // atanan değişkeni ekrana yazdırıyoruz( serial mönütöre )
         }

        
       }
     }
     else{
         Serial.printf("[HTTP] GET... code: %d\n",http.errorToString(httpCode).c_str());
     }
  }
  else{
     Serial.println("http isteği gönderilemedi");
  }
}
}

void loop() {
  // put your main code here, to run repeatedly:

}
