#include <DHT.h>
#include <DHT_U.h>


/*
mq-2 아날로그
불꽃감지 디지털
온습도 디지털
mq-7 아날로그
fire: 화재 1 비화재 0
*/

#include <SD.h>
#include <SoftwareSerial.h>

#define DHTPIN 5
#define SDPIN 10
#define FLAMEPIN 2
#define DHTTYPE DHT22

SoftwareSerial mySerial(10, 11); // RX, TX
const int GASPINA = A0;
const int GASPINB = A1;
float hum;
float temp;
float flame;
float gastwo;
float gasseven;
int curMode = 0;
int level = 0;
String id = "arduino";
String location="Toilet Area";

DHT dht(DHTPIN, DHTTYPE);
File myFile;

void setup()
{
    Serial.begin(9600);
    mySerial.begin(9600);
    Serial.println("connected with espModule"); 
    Serial.println("Start Sensing");
    dht.begin();
    //pinMode(SDPIN, OUTPUT);  // sd 카드 모듈에 연결
    pinMode(FLAMEPIN, INPUT);
    delay(1000);
}

int timeSync = 0;
void loop()
{
    if (mySerial.available()) {
      Serial.println("module@@"+mySerial.readStringUntil('\n'));
    }
    if( timeSync % 40 == 0 ){
        hum = dht.readHumidity();
        temp = dht.readTemperature();  // 온습도계 정보 저장
        flame=digitalRead(FLAMEPIN); // 1 = 감지, 0 = 미감지
          //flameVal=analogRead(flamePin);  // 불꽃감지 센서 값 저장
        gastwo=analogRead(GASPINA); // 가스 정보 저장
        gasseven=analogRead(GASPINB); // 가스 정보 저장
        int fire = 0;
        if(flame == 0)
        {
         fire=1;
        }     // 불꽃감지 센서 작동 시
        if(temp >= 50)
        {
        fire=1;
        }  //온도가 50 이상일 시
        if(gastwo>=400)
        {
        fire=1;
        } // MQ-2의 값이 400 이상일 시
        if(gasseven>=400)
        {
        fire=1;
        } // MQ-7의 값이 400 이상일 시
        if( fire != curMode ){
          curMode = fire;
          if( curMode == 1 ){
            mySerial.println("Mode,1");
            level = 2;
          }else{
            mySerial.println("Mode,0");
            level = 0;
          }
          Serial.println("Mode Change to="+fire);
          delay(100);
        }
        char result[128];
        char str_temp[10];
        int len = 0;
        sprintf( result , "Update,%d,", level);
        dtostrf(temp, 4, 2, str_temp); 
        len = strlen(str_temp);
        str_temp[len]=','; str_temp[len+1]=0;
        strcat( result, str_temp );
        
        dtostrf(hum, 4, 2, str_temp); 
        len = strlen(str_temp);
        str_temp[len]=','; str_temp[len+1]=0;
        strcat( result, str_temp );
        
        dtostrf(flame, 4, 2, str_temp); 
        len = strlen(str_temp);
        str_temp[len]=','; str_temp[len+1]=0;
        strcat( result, str_temp );
        
        dtostrf(gastwo, 4, 2, str_temp); 
        len = strlen(str_temp);
        str_temp[len]=','; str_temp[len+1]=0;
        strcat( result, str_temp );
        
        dtostrf(gasseven, 4, 2, str_temp); 
        len = strlen(str_temp);
        strcat( result, str_temp );
        //String data = "Update,?id="+id+"&location="+location+"&level="+level+"&temp="+temp+"&hum="+hum+"&flame="+flame+"&gastwo="+gastwo+"&gasseven="+gasseven;
        //String data = temp+level+","+temp+","+hum+","+flame+","+gastwo+","+gasseven;
        //mySerial.println( tmp );
        mySerial.println(result);
    }
    timeSync++;
    delay(50);
}
