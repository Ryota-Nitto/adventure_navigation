/*soundMode_new, viewMode_newを更新する*/
void settingMode() {
  if (digitalRead(WIO_KEY_A) == LOW) {
    Serial.println("A Key pressed");
    if (debugMode_old == 0) {
      debugMode_new = 1;
      lat_now = ORIGIN[0] - trackLat[0]/MPLAT;
      lon_now = trackLon[0]/MPLON + ORIGIN[1];
    } 
    else {
      debugMode_new = 0;
    }
    state_old = 3;
    while(digitalRead(WIO_KEY_A) == LOW){}
  }
  if (digitalRead(WIO_KEY_B) == LOW) {
    Serial.println("B Key pressed");
    if (soundMode_old == 0) {
      soundMode_new = 1;
    } 
    else {
      soundMode_new = 0;
    }
    state_old = 3;
    while(digitalRead(WIO_KEY_B) == LOW){}
  }
  if (digitalRead(WIO_KEY_C) == LOW) {
    Serial.println("C Key pressed");
    if (viewMode_old == 0) {
      viewMode_new = 1;
    } 
    else {
      viewMode_new = 0;
    }
    state_old = 3;
    while(digitalRead(WIO_KEY_C) == LOW){}
  }
  if (digitalRead(WIO_5S_PRESS) == LOW) {
    Serial.println("5 Way press");
    if (scaleMode_old == 0) {
      scaleMode_new = 1; //x=100にする
      OUT_RANGE = 10;
      DISPLAY_X_LENGTH = 200;
      DISPLAY_Y_LENGTH = DISPLAY_X_LENGTH*2/3;
    } 
    else if(scaleMode_old == 1) {
      scaleMode_new = 2; //x=32にする
      OUT_RANGE = 5;
      DISPLAY_X_LENGTH = 40;
      DISPLAY_Y_LENGTH = DISPLAY_X_LENGTH*2/3;
    }
    else {
      scaleMode_new = 0; //x=1000にする
      OUT_RANGE = 20;
      DISPLAY_X_LENGTH = 1000;
      DISPLAY_Y_LENGTH = DISPLAY_X_LENGTH*2/3;
    }
    state_old = 3;
    while(digitalRead(WIO_5S_PRESS) == LOW){}
  }
}

/*GPS情報を取得し、lat_now, lon_now, lat2, lon2, state_newを更新する*/
void settingGPS() {
  while (mySerial.available() > 0) {
      char c = mySerial.read();
      //Serial.print(c);
      gps.encode(c);
      //tft.fillRect(100,210,200,30,TFT_BLACK);
      //tft.drawString("ReceiveGPS...",100,210);
  }
  //Serial.println("");

  lat_now = gps.location.lat();
  lon_now = gps.location.lng();

  lat2 = (ORIGIN[0] - lat_now) * MPLAT;
  lon2 = (lon_now - ORIGIN[1]) * MPLON;
  Serial.print(lat_now,6);
  Serial.print(" ");
  Serial.println(lon_now,6);
}
/*
void myserialEvent(){
  if(mySerial.available()>0){
    settingGPS();
  }
}
*/
void settingGPS_debug() {
  if (digitalRead(WIO_5S_UP) == LOW) {
    lat_now += 5.0/MPLAT; //5m分北に進む
    Serial.println("5 Way Up");
    while(digitalRead(WIO_5S_UP) == LOW){}
  }
  else if (digitalRead(WIO_5S_DOWN) == LOW) {
    lat_now -= 5.0/MPLAT; //5m分南に進む
    Serial.println("5 Way Down");
    while(digitalRead(WIO_5S_DOWN) == LOW){}
  }
  else if (digitalRead(WIO_5S_LEFT) == LOW) {
    lon_now -= 5.0/MPLON; //5m分西に進む
    Serial.println("5 Way Left");
    while(digitalRead(WIO_5S_LEFT) == LOW){}
  }
  else if (digitalRead(WIO_5S_RIGHT) == LOW) {
    lon_now += 5.0/MPLON; //5m分東に進む
    Serial.println("5 Way Right");
    while(digitalRead(WIO_5S_RIGHT) == LOW){}
  }

  lat2 = (ORIGIN[0] - lat_now) * MPLAT;
  lon2 = (lon_now - ORIGIN[1]) * MPLON;
}

//北を0、"時計回り"を正として-π≤angle≤πを出力。
//rotatePoints()は"反時計回り"を正とするが、
//自分が時計回りにangle回り地図が反時計回りにangle回れば表示は同じ方向を向く。
void settingAngle() {
  //BMX055 磁気の読み取り
  BMX055_Mag();
  double theta = compass(mag[0],mag[1]);
  /*Serial.print("theta=");
  Serial.print(theta*180/3.141592);
  Serial.println("");*/
  angle = -(float)theta - DECLINATION;
}

void settingAngle_debug() {
  if (digitalRead(WIO_5S_PRESS) == LOW) {
    angle += 3.141592/6;
    Serial.println("5 Way Press");
  }
  while(digitalRead(WIO_5S_PRESS) == LOW){}
}

void gatherNearbyPoints() {
  //nearbyLat, nearbyLonの初期化
  for (int j = 0; j < NUMBER_OF_NEARBYPOINTS; j++){
    nearbyLat[j] = 0;
    nearbyLon[j] = 0;
  }
  //nearbyLat, nearbyLonに値を入れる
  int k = -1; //nearbyLat, nearbyLonの指数
  int i_old = -1; //当たり判定だったtrackLat, trackLonの添字をストック
  //tft.fillScreen(TFT_PURPLE);
  //delay(100);
  for (int i = 0; i < NUMBER_OF_POINTS; i++){
    if (trackLat[i] != 0) {
      //現在地とtrackPointの距離が付近距離より小さければ
      double dis =  pow((pow((lat2 - trackLat[i]),2) + pow((lon2 - trackLon[i]),2)), 0.5);
      if (dis < NEARBY_LENGTH) {
        //連続して付近にあったらnearbyLat, nearbyLonの添字は1つ増やす
        if (i - i_old == 1) {
          k = k+1;
        }
        //不連続して付近にあったらnearbyLat, nearbyLonの添字は1つあける。間に0が入る。
        else{
          k = k+2;
        }
        nearbyLat[k] = trackLat[i];
        nearbyLon[k] = trackLon[i];
        i_old = i;
      }
    }
    //tft.drawString(String(k,DEC),100,150);
    //delay(0.1);
  }
  real_number_of_nearbypoints = k+1;
}

void rotatePoints() {
  //tft.drawString(String(real_number_of_nearbypoints,DEC),100,150);
  //delay(100);
  for (int i = 0; i < real_number_of_nearbypoints; i++) {
    //nearbyLat, nearbyLonのうち0でないものを座標変換する
    if (nearbyLat[i] != 0) {
      float dx = nearbyLat[i] - lat2;
      float dy = nearbyLon[i] - lon2;
      rotatedLat[i] = dx*cos(angle) - dy*sin(angle);
      rotatedLon[i] = dx*sin(angle) + dy*cos(angle);
    }
    //不連続点を示す0点の座標は[NEARBY_LENGTH+1, NEARBY_LENGTH+1]とする。
    else if (nearbyLat[i] == 0) {
      rotatedLat[i] = NEARBY_LENGTH+1;
      rotatedLon[i] = NEARBY_LENGTH+1;
    }
  }
}

int pixelState(float x, float y) {
  if(lat2 < 0 || lat2 >= V || lon2 < 0 || lon2 >= H){
    return 0;
  }
  else{
    //経路pointの半径OUT_RANGE内はセーフ
    for (int i = 0; i < real_number_of_nearbypoints; i++) {
      //nearbyLat, nearbyLonが外れ点でなければ
      if (rotatedLat[i] != NEARBY_LENGTH+1){
        float x1 = rotatedLat[i];
        float y1 = rotatedLon[i];
        if ((x-x1)*(x-x1)+(y-y1)*(y-y1) < OUT_RANGE*OUT_RANGE) {
          return 2;
        }
      }
    }
    //経路point間をつなぐ長方形内はセーフ
    for (int i = 0; i < real_number_of_nearbypoints - 1; i++) {
      //nearbyLat, nearbyLonが外れ点出なければ
      if (rotatedLat[i] != NEARBY_LENGTH+1 && rotatedLat[i+1] != NEARBY_LENGTH+1) {
        float e = OUT_RANGE;
        float x1 = rotatedLat[i];
        float y1 = rotatedLon[i];
        float x2 = rotatedLat[i+1];
        float y2 = rotatedLon[i+1];
        float dx = rotatedLat[i+1] - rotatedLat[i];
        float dy = rotatedLon[i+1] - rotatedLon[i];
        if (dx == 0) {
          if (x1-e < x && x < x1+e && ((y1 < y && y < y2) || (y2 < y && y < y1))) {
            return 2;
          }
        }
        else if (dy == 0) {
          if (y1-e < y && y < y1+e && ((x1 < x && x < x2) || (x2 < x && x < x1))) {
            return 2;
          }
        }
        else if (dy/dx > 0) {
          if (dx > 0){
            if (y > -dx/dy*(x-x1)+y1 && 
                y < -dx/dy*(x-x2)+y2 && 
                y > dy/dx*(x-x1)+y1-e*sqrt(pow(dx,2)+pow(dy,2))/dx && 
                y < dy/dx*(x-x1)+y1+e*sqrt(pow(dx,2)+pow(dy,2))/dx) {
              return 2;
            }
          }
          else if (dx < 0) {
            if (y < -dx/dy*(x-x1)+y1 && 
                y > -dx/dy*(x-x2)+y2 && 
                y > dy/dx*(x-x1)+y1-e*sqrt(pow(dx,2)+pow(dy,2))/(-dx) && 
                y < dy/dx*(x-x1)+y1+e*sqrt(pow(dx,2)+pow(dy,2))/(-dx) ){
              return 2;
            }
          }
        }
        else if (dy/dx < 0) {
          if (dx > 0) {
            if (y < -dx/dy*(x-x1)+y1 && 
                y > -dx/dy*(x-x2)+y2 && 
                y > dy/dx*(x-x1)+y1-e*sqrt(pow(dx,2)+pow(dy,2))/dx && 
                y < dy/dx*(x-x1)+y1+e*sqrt(pow(dx,2)+pow(dy,2))/dx) {
                return 2;
            }
          }
          else if (dx < 0) {
            if (y > -dx/dy*(x-x1)+y1 && 
                y < -dx/dy*(x-x2)+y2 && 
                y > dy/dx*(x-x1)+y1-e*sqrt(pow(dx,2)+pow(dy,2))/(-dx) && 
                y < dy/dx*(x-x1)+y1+e*sqrt(pow(dx,2)+pow(dy,2))/(-dx)) {
                return 2;
            }
          }
        }
      }
    }
    return 1;//どれでもなければOUT
  }
}

void settingState() {
  state_new = pixelState(0.0, 0.0);
}
