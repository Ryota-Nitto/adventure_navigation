#include <SPI.h>
#include <Wire.h>
#include <math.h>
#include <GNSS.h>
#include "epd2in13b.h"
#include "imagedata.h"
#include "epdpaint.h"

#define Addr_Mag 0x13 // BMX055　磁気センサのI2Cアドレス
#define NUMBER_OF_POINTS 1300 //mapの経路pointの数の上限。2750まで(float, int型の時)
#define NUMBER_OF_NEARBYPOINTS 100 //現在地付近の経路pointの数の上限
#define COLORED     0
#define UNCOLORED   1

Epd epd;
unsigned char image[22048];
Paint paint(image, 104, 50);

const int BUTTON0 = 2;
const int BUTTON1 = 3;
const int BUTTON2 = 4;
const int BUZZER = 5;

static SpGnss Gnss; //SpGnss object

float MPLAT; //lat1度あたりの距離(m)
float MPLON; //lon1度あたりの距離(m)
float INTERVAL_LENGTH; //補完後のpoint間距離(m)
float OUT_RANGE = 20.0; //道外れ判定距離(m)
int DISPLAY_X_PIXEL = 104; //画面のx方向ピクセル(dot)
int DISPLAY_Y_PIXEL = 212; //画面のy方向ピクセル(dot)
float DISPLAY_RATIO = DISPLAY_Y_PIXEL/DISPLAY_X_PIXEL; //画面の縦横比(Y/X)
float DISPLAY_X_LENGTH = 1000.0; //画面のx方向長さ(m)
float DISPLAY_Y_LENGTH = DISPLAY_X_LENGTH*DISPLAY_RATIO; //画面のy方向長さ(m)
int RESOLUTION = 3; //解像度(dot)8の倍数である必要がある
int CENTER_SQUARE_SIZE = 16;//現在地の正方形の一辺(dot)
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
int soundMode = 0; //0 mute, 1 buzz
int viewMode = 0; //0 txt, 1 map
int scaleMode = 0; //0 wide, 1 detail
int debugMode = 1; //0 nomal, 1 debug
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
    pinMode(LED0, OUTPUT);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(BUTTON0, INPUT);
    pinMode(BUTTON1, INPUT);
    pinMode(BUTTON2, INPUT);
    pinMode(BUZZER, OUTPUT);

    Serial.begin(9600);

    readingSD("output.txt");

    //デバッグ時は初期値をスタート地点に置く
    if(debugMode == 1){
        lat_now = ORIGIN[0] - trackLat[0]/MPLAT;
        lon_now = trackLon[0]/MPLON + ORIGIN[1];
    }

    //gpsのセットアップ
    gps_setup();
    
    //磁気センサのセットアップ
    Wire.begin();
    BMX055_Init();
    fix();

    //電子ペーパーのセットアップ
    Serial.println("display_setup");
    display_setup();

    delay(1000);
}

void loop (){
    //Serial.println("settingMode");
    settingMode();
    if (debugMode == 0) {
        Serial.println("settingGPS");
        settingGPS();
    }
    else{
        settingGPS_debug();
    }
    time_new = millis();
    if (time_new - time_old > 16000) {
        settingAngle();
        gatherNearbyPoints();
        rotatePoints();
        settingState();
        alert();
        Serial.print(lat_now,6);
        Serial.print(" ");
        Serial.print(lat_old,6);
        Serial.print(" ");
        Serial.print(lon_now,6);
        Serial.print(" ");
        Serial.print(lon_old,6);
        Serial.print(" ");
        Serial.print(state_new);
        Serial.print(" ");
        Serial.print(state_old);
        Serial.print(" ");
        Serial.print(angle,6);
        Serial.print(" ");
        Serial.println(angle_old,6);
        if (viewMode == 0) {
            Serial.println("displayState");
            displayState();
        }
         else if (viewMode == 1) {
            Serial.println("displayMap");
             displayMap();
         }
        time_old = time_new;
    }
}
