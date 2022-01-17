#include <SPI.h>
#include <Seeed_FS.h>
#include "SD/Seeed_SD.h"
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "TFT_eSPI.h"
#include "Free_Fonts.h"
#include <Wire.h>
#include <math.h>
#define Addr_Mag 0x13 // BMX055　磁気センサのI2Cアドレス
#define NUMBER_OF_POINTS 1300 //mapの経路pointの数の上限。2750まで(float, int型の時)
#define NUMBER_OF_NEARBYPOINTS 100 //現在地付近の経路pointの数の上限

SoftwareSerial mySerial(2, 3); // RX, TX
TinyGPSPlus gps;
TFT_eSPI tft;
TinyGPSCustom ExtLat(gps, "GPGGA", 3);  //N for Latitude
TinyGPSCustom ExtLng(gps, "GPGGA", 5);  //E for Longitude

//設定値
float MPLAT; //lat1度あたりの距離(m)
float MPLON; //lon1度あたりの距離(m)
float INTERVAL_LENGTH; //補完後のpoint間距離(m)
float OUT_RANGE = 20.0; //道外れ判定距離(m)
float DISPLAY_X_LENGTH = 1000.0; //画面のx方向長さ(m)
float DISPLAY_Y_LENGTH = DISPLAY_X_LENGTH*2/3; //画面のy方向長さ(m)
int RESOLUTION = 5; //解像度(dot)
int DISPLAY_X_PIXEL = 320; //画面のx方向ピクセル(dot)
int DISPLAY_Y_PIXEL = 240; //画面のy方向ピクセル(dot)
float NEARBY_LENGTH; //現在地からneabyPointsまでの最大距離(m)
float ORIGIN[2]; //原点座標
float TERMINAL[2];
float H; //水平距離(m)
float V; //垂直距離(m)
float trackLat[NUMBER_OF_POINTS]; //経路pointの緯度(m)
float trackLon[NUMBER_OF_POINTS]; //経路pointの経度(m)
float nearbyLat[NUMBER_OF_NEARBYPOINTS]; //付近pointの緯度(m)
float nearbyLon[NUMBER_OF_NEARBYPOINTS]; //付近pointの経度(m)
int real_number_of_nearbypoints; //実際の付近pointの個数(個)
float rotatedLat[NUMBER_OF_NEARBYPOINTS]; //並進・回転後の付近pointの緯度(m)
float rotatedLon[NUMBER_OF_NEARBYPOINTS]; //並進・回転後の付近pointの経度(m)
float angle = 0.0; //反時計回りの回転角度(rad)
float angle_old = 3.141592;
float lat2; //GPSから受信した座標(度)を(m)に直したもの
float lon2;
int state_new = 3; //0 out of area, 1 out, 2 in, 3 initial_state, 4 strange
int state_old = 3;
int soundMode_new = 0; //0 mute, 1 buzzer, 2 initial_old
int soundMode_old = 2;
int viewMode_new = 0; //0 nothing, 1 map, 2 initial_old
int viewMode_old = 2;
int debugMode_new = 0; //0 nomal, 1 debug, 2 initial_old
int debugMode_old = 2;
int scaleMode_new = 0; //0 x=750, 1 120, 2 32, 3 initial_old
int scaleMode_old = 3; 
float lat_now;
float lon_now;
float lat_old = 1.0;
float lon_old = 1.0;
int   mag[2] = {0,0}; //xMag, yMagを格納
int   max_buffer[2] = {-2274, 44558};
int   min_buffer[2] = {-3567, 43528};
int   fix_value[2];
int   fix_max[2];
int   fix_min[2];
float DECLINATION = (7+42/60)*3.141592/180; //偏角7°42′をradに変換したもの
int time_new = millis();
int time_old = -1;

void setup() {

  pinMode(WIO_KEY_A, INPUT_PULLUP);
  pinMode(WIO_KEY_B, INPUT_PULLUP);
  pinMode(WIO_KEY_C, INPUT_PULLUP);
  pinMode(WIO_BUZZER, OUTPUT);
  pinMode(WIO_5S_UP, INPUT_PULLUP);
  pinMode(WIO_5S_DOWN, INPUT_PULLUP);
  pinMode(WIO_5S_LEFT, INPUT_PULLUP);
  pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
  pinMode(WIO_5S_PRESS, INPUT_PULLUP);
  
  Serial.begin(9600);
  Serial.println("Start serial!");
  mySerial.begin(9600);

  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setFreeFont(&FreeSansBoldOblique12pt7b);

  Serial.print("Initializing SD card...");
  if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  readingSD("output.txt");

  //デバッグ時は初期値をスタート地点に置く
  if(debugMode_new == 1){
    lat_now = ORIGIN[0] - trackLat[0]/MPLAT;
    lon_now = trackLon[0]/MPLON + ORIGIN[1];
  }

  //磁気センサのセットアップ
  Wire.begin();
  BMX055_Init();
  delay(300);
  Serial.println("Fixing...");
  fix();

  delay(1000);
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  
  tft.fillRect(100,210,200,30,TFT_BLACK);
  //tft.drawString("SettingMode...",100,210);
  settingMode();

  //tft.fillRect(100,210,200,30,TFT_BLACK);
  //tft.drawString("DisplayMode...",100,210);
  displayMode();
  
  if (debugMode_new == 0){
    //tft.fillRect(100,210,200,30,TFT_BLACK);
    //tft.drawString("SettingGPS...",100,210);
    settingGPS();
  }
  else{
    //tft.fillRect(100,210,200,30,TFT_BLACK);
    //tft.drawString("SettingGPS...",100,210);
    settingGPS_debug();
  }
  
  time_new = millis();
  //2秒に一回行う処理
  if (time_new - time_old > 2000) {
    tft.fillRect(100,210,200,30,TFT_BLACK);
    tft.drawString("SettingAngle...",100,210);
    settingAngle();

    //tft.fillRect(100,210,200,30,TFT_BLACK);
    //tft.drawString("Gather...",100,210);
    gatherNearbyPoints();

    //tft.fillRect(100,210,200,30,TFT_BLACK);
    //tft.drawString("Rotate...",100,210);
    rotatePoints();
    
    //tft.fillRect(100,210,200,30,TFT_BLACK);
    //tft.drawString("SettingState...",100,210);
    settingState();
  
    alert();

    /****** mapなしモード  ******/
    if (viewMode_new == 0) {
      displayState();
      tft.fillRect(100,210,200,30,TFT_BLACK);
      tft.drawString("DisplayState...",100,210);
    }
    
    /****** mapありモード  ******/
    else if (viewMode_new == 1) {
      tft.fillRect(100,210,200,30,TFT_BLACK);
      tft.drawString("DisplayMap...",100,210);
      displayMap();
    }
    time_old = time_new;
  }

}
