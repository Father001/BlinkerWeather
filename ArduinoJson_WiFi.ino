

#define BLINKER_WIFI

#include <Blinker.h>

char auth[] = "aa07ed5dde8e";
char ssid[] = "ESP12E";
char pswd[] = "12345678";

unsigned int SetTime = 90000;
unsigned long LastTime = 0;

BlinkerNumber Hum("num-qb2");
BlinkerNumber Tem("num-uh4");
BlinkerNumber Body("num-q3t");


void weatherData(const String & data)
{
  BLINKER_LOG("weather: ", data);
   parseJOSN(data);
}

//解析Json数据
void parseJOSN(String Weather)
{
  const size_t capacity = JSON_OBJECT_SIZE(13) + 140;
  DynamicJsonDocument doc(capacity);
  
  deserializeJson(doc, Weather);

  const int cloud = doc["cloud"]; 
  const int cond_code = doc["cond_code"]; 
  const char* cond_txt = doc["cond_txt"]; 
  const int fl = doc["fl"]; 
  const int hum = doc["hum"];
  const char* pcpn = doc["pcpn"]; 
  const char* pres = doc["pres"]; 
  const int tmp = doc["tmp"]; 
  const char* vis = doc["vis"]; 
  const char* wind_deg = doc["wind_deg"]; 
  const char* wind_dir = doc["wind_dir"]; 
  const char* wind_sc = doc["wind_sc"]; 
  const char* wind_spd = doc["wind_spd"]; 
 
  Hum.print(hum);
  Tem.print(tmp);
  Body.print(fl);
  BLINKER_LOG("温度:",tmp);
  BLINKER_LOG("湿度:",hum);
  BLINKER_LOG("体感温度:",fl);
}


void dataRead(const String & data)
{
  BLINKER_LOG("Blinker readString: ", data);

  uint32_t BlinkerTime = millis();

  Blinker.vibrate();
  Blinker.print("millis", BlinkerTime);

  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  
  
}

void setup()
{
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Blinker.begin(auth, ssid, pswd);
  
  Blinker.attachData(dataRead);
  Blinker.attachWeather(weatherData);
}

void loop()
{

  Blinker.run();
  unsigned long NowTime = millis();
  if(NowTime - LastTime > SetTime)
  {
    LastTime = NowTime;
    Blinker.weather("hangzhou");
   }
   if(NowTime - LastTime <0)
    LastTime = NowTime;
}
