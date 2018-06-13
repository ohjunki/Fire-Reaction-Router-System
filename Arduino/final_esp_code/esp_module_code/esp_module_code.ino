#include <mthread.h>
class FooThread : public Thread
{
public:
    FooThread(int id);
protected:
    bool loop();
    void networkThread();
    void sensorThread();
private:
    int id;
};

FooThread::FooThread(int id)
{
    this->id = id;
}


#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>
SoftwareSerial arduino(3,1);

const int FIRE_AP_CNT = 2;
const int MY_INDEX_OF_FIRE_AP = 0;
const int MODE_FIRE = 1, MODE_GENERAL = 0;
const char* AP_WIFI_NAME[] = {"Fire-Ap-Arduino", "Fire-Ap-Artik"};
const char* AP_WIFI_PAWD[] = {"12345678","12345678"};
unsigned long sendTime[FIRE_AP_CNT];
int curMode = MODE_GENERAL;
int changeTargetMode = MODE_GENERAL;
HTTPClient http;

String query = "";
char queryArray[256];
bool hasTarget = false;
int timeout = 0;
int curTargetApIndex = 0;
void FooThread::networkThread(){
  if( !hasTarget ){
      IPAddress myIP = WiFi.softAP(AP_WIFI_NAME[MY_INDEX_OF_FIRE_AP] ,AP_WIFI_PAWD[MY_INDEX_OF_FIRE_AP]);
      int n = WiFi.scanNetworks();
      for (int i = 0; i < n; i++) {
        for ( int j = 0; j < FIRE_AP_CNT; j++ ){
          if ( j != MY_INDEX_OF_FIRE_AP && WiFi.SSID(i) == AP_WIFI_NAME[j]  ){
            WiFi.begin(AP_WIFI_NAME[j],AP_WIFI_PAWD[j]);
            curTargetApIndex = j;
            hasTarget = true;
            return;
          }
        }
      }
  }else{
      bool nowDisconnect = false;
      if( timeout > 40 ){
          nowDisconnect = true;
      }else{
          timeout += 1;
          if(WiFi.status() == WL_CONNECTED) {
            arduino.println("Connected");
            if( queryArray[0] == 0 ){
                arduino.println("No Data");
            }else{
              char requestBuffer[256];
              sprintf( requestBuffer , "http://192.168.1.1:3000/fire/update%s", queryArray );
              http.begin( requestBuffer );
              int httpCode = http.GET();
              http.end();
      
              if( curMode != changeTargetMode ){
                  sprintf( requestBuffer , "http://192.168.1.1:3000/fire/mode?mode=%d", changeTargetMode );
                  http.begin( requestBuffer );
                  int httpCode = http.GET();
                  if( httpCode > 0 ){ 
                      curMode = changeTargetMode;
                  }
                  http.end();
              }
            }
          }
      }

      if( nowDisconnect ){
          WiFi.disconnect();
          hasTarget = false;
          timeout = 0;
      }
      sleep(1);
  }
}

char* formatList[6]={ "&level=%s", "&temp=%s", "&hum=%s", "&flame=%s", "&gastwo=%s", "&gasseven=%s" };
void FooThread::sensorThread(){
    int tempMode = curMode;
    if( arduino.available() ){
      String msg = arduino.readStringUntil('\n');
      arduino.println(msg);
      String type = msg.substring(0, msg.indexOf(','));
      msg = msg.substring( msg.indexOf(',')+1 );
      if( type.equals( "Update" ) ){
          char buf[64], buf2[64], result[128];
          int cnt=0;
          sprintf( result, "?id=123&location=1층화장실" );
          String tmp = "";
          while( msg.indexOf(',') != -1 ){
            tmp = msg.substring(0, msg.indexOf(','));
            msg = msg.substring( msg.indexOf(',')+1 );
            tmp.toCharArray( buf, 64 ); 
            sprintf( buf2, formatList[cnt] , buf );
            strcat( result, buf2 );
            cnt++;
          }
          if( !tmp.equals("") ){
            tmp.toCharArray( buf, 64 ); 
            sprintf( buf2, formatList[cnt] , buf );
            strcat( result, buf2 );
            cnt++;
          }
          if( cnt == 6 ){
            strcpy( queryArray, result );
          }
      }else if( type.equals("Mode")){
          tempMode = msg.toInt();
          if( curMode != tempMode ){
              changeTargetMode = tempMode;
              arduino.printf("mode change %d\n", tempMode );
          }
       }
    }
    delay(20);
}

bool FooThread::loop()
{
    if(kill_flag)
        return false;
    if( this->id == 1 ) networkThread();
    else sensorThread();
    return true;
}

void setup()
{
    arduino.begin(9600);
    WiFi.mode(WIFI_AP_STA);
    
    main_thread_list->add_thread(new FooThread(0));
    main_thread_list->add_thread(new FooThread(1));

    delay(1000);
}

// jl
