/* soundMode_new, viewMode_newの表示 */
void displayMode() {

  if (viewMode_new != viewMode_old) {
    tft.fillScreen(TFT_BLACK);
  }
  
  /** debugMode の表示切り替え **/
  if (debugMode_new != debugMode_old || viewMode_new != viewMode_old){
    if (debugMode_new == 0){
      tft.fillRect(170,0,150,20,TFT_BLACK);
      tft.drawString("NOMAL",170,0);
      analogWrite(WIO_BUZZER, 0);
    }
    else if (debugMode_new == 1) {
      tft.fillRect(170,0,150,20,TFT_BLACK);
      tft.drawString("DEBUG",170,0);
    }
    debugMode_old = debugMode_new;
  }

  /** soundMode の表示切り替え **/
  if (soundMode_new != soundMode_old || viewMode_new != viewMode_old){
    if (soundMode_new == 0){
      tft.fillRect(70,0,80,20,TFT_BLACK);
      tft.drawString("MUTE",70,0);
      analogWrite(WIO_BUZZER, 0);
    }
    else if (soundMode_new == 1) {
      tft.fillRect(70,0,80,20,TFT_BLACK);
      tft.drawString("BUZZ",70,0);
    }
    soundMode_old = soundMode_new;
  }

  /** scaleModeの切り替え **/
  if (scaleMode_new != scaleMode_old || viewMode_new != viewMode_old) {
    tft.fillRect(0,210,100,30,TFT_BLACK);
    tft.drawString("X:"+String((int)DISPLAY_X_LENGTH)+'m',0,210);
    scaleMode_old = scaleMode_new;
  }

  /** viewModeの表示切り替え **/
  if (viewMode_new != viewMode_old) {
    if (viewMode_new == 0) {
      tft.fillRect(0,0,60,20, TFT_BLACK);
      tft.drawString("TXT",0,0);
    }
    else if (viewMode_new == 1) {
      tft.fillRect(0,0,60,20, TFT_BLACK);
      tft.drawString("MAP",0,0);
    }
    viewMode_old = viewMode_new;
  }
}


/*viewMode_new == 0の表示を変更する */
void displayState() {
  /** 緯度, 経度の表示切り替え **/
  if (lat_now != lat_old || lon_now != lon_old || state_new != state_old || angle != angle_old) {
    int angle_deg = (int)(angle*180/3.14159) % 360;
    tft.fillRect(100,40,200,100,TFT_BLACK);
    tft.drawString(String(lat_now,6),100,40);
    tft.drawString(String(lon_now,6),100,65);
    tft.drawString(String(angle_deg,DEC),100,90);
    lat_old = lat_now;
    lon_old = lon_now;
    angle_old = angle;

    if (state_new == 0) { //OUT OF AREA
      //tft.fillRect(100,105,200,30,TFT_BLACK);
      tft.drawString("OUT OF AREA",100,115);
    }
    else if (state_new == 1) { //OUT
      //tft.fillRect(100,105,200,30,TFT_BLACK);
      tft.drawString("OUT",100,115);
    }
    else if (state_new == 2){
      //tft.fillRect(100,105,200,30,TFT_BLACK);
      tft.drawString("IN",100,115);
    }
    else if(state_new == 3) {
      //tft.fillRect(100,105,200,30,TFT_BLACK);
      tft.drawString("STRANGE",100,115);
    }
  }
  state_old = state_new;
}

/*viewMode_new == 1のときにマップを表示する*/
void displayMap() {
  if (lat_now != lat_old || lon_now != lon_old || state_new != state_old || angle != angle_old) {
    for (int i=-120/RESOLUTION; i<120/RESOLUTION; i++) {
      for (int j=-160/RESOLUTION; j<160/RESOLUTION; j++) {
        int pixel = pixelState((float)i*DISPLAY_X_LENGTH*RESOLUTION/DISPLAY_X_PIXEL, (float)j*DISPLAY_Y_LENGTH*RESOLUTION/DISPLAY_Y_PIXEL);
        if (pixel == 0) {
          tft.fillRect((j*RESOLUTION+DISPLAY_X_PIXEL/2), (i*RESOLUTION+DISPLAY_Y_PIXEL/2), RESOLUTION, RESOLUTION, TFT_BLACK);
        }
        else if (pixel == 1) {
          tft.fillRect((j*RESOLUTION+DISPLAY_X_PIXEL/2), (i*RESOLUTION+DISPLAY_Y_PIXEL/2), RESOLUTION, RESOLUTION, TFT_BLACK);
        }
        else if (pixel == 2){
          tft.fillRect((j*RESOLUTION+DISPLAY_X_PIXEL/2), (i*RESOLUTION+DISPLAY_Y_PIXEL/2), RESOLUTION, RESOLUTION, TFT_WHITE);
        }
      }
    }
    lat_old = lat_now;
    lon_old = lon_now;
    angle_old = angle;
  
    if (state_new == 0) { //OUT OF AREA
      tft.fillRect(150,110,20,20,TFT_LIGHTGREY);
    }
    else if (state_new == 1) { //OUT
      tft.fillRect(150,110,20,20,TFT_RED);
    }
    else if (state_new == 2){
      tft.fillRect(150,110,20,20,TFT_GREEN);
    }
    else if(state_new == 3) {
      tft.fillRect(100,90,200,30,TFT_BLACK);
      tft.drawString("STRANGE",100,80);
    }
  }
  state_old = state_new;
}
