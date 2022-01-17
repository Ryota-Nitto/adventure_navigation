void display_setup() {
  if (epd.Init() != 0) {
    Serial.print("e-Paper init failed");
    return;
  }
  epd.ClearFrame();
  Serial.println("epd.ClearFrame");
}


/*viewMode == 0のときtxtを表示 */
void displayState() {
  /** 緯度, 経度の表示切り替え **/
  if (lat_now != lat_old || lon_now != lon_old || state_new != state_old || angle != angle_old) {
    int angle_deg = (int)(angle*180/3.14159) % 360;
    paint.SetWidth(104);
    paint.SetHeight(106);
    paint.Clear(UNCOLORED);
    char buf[20];
    int ret = snprintf(buf, sizeof buf, "%f", lat_now);
    paint.DrawStringAt(8,2,buf, &Font12, COLORED);
    ret = snprintf(buf, sizeof buf, "%f", lon_now);
    paint.DrawStringAt(8,14,buf, &Font12, COLORED);
    ret = snprintf(buf, sizeof buf, "%d", angle_deg);
    paint.DrawStringAt(8,26,buf, &Font12, COLORED);
    epd.SetPartialWindowBlack(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
    lat_old = lat_now;
    lon_old = lon_now;
    angle_old = angle;

    paint.Clear(UNCOLORED);
    if (state_new == 0) { //OUT OF AREA
      paint.DrawStringAt(8,2,"OUT OF AREA", &Font12, COLORED);
    }
    else if (state_new == 1) { //OUT
      paint.DrawStringAt(8,2,"OUT", &Font12, COLORED);
    }
    else if (state_new == 2){
      paint.DrawStringAt(8,2,"IN", &Font12, COLORED);
    }
    else if(state_new == 3) {
      paint.DrawStringAt(8,2,"STRANGE", &Font12, COLORED);
    }
    epd.SetPartialWindowBlack(paint.GetImage(), 0, 106, paint.GetWidth(), paint.GetHeight());
    epd.DisplayFrame();
    epd.DisplayFrame(IMAGE_BLACK, IMAGE_RED);
  }
  state_old = state_new;
}

/*viewMode == 1のときにマップを表示する*/
void displayMap() {
  //if (lat_now != lat_old || lon_now != lon_old || state_new != state_old || angle != angle_old) {
    Serial.println("paint.SetWidth");
    paint.SetWidth(104);
    Serial.println("paint.SetHeight");
    paint.SetHeight(212);
    Serial.println("paint.Clear");
    paint.Clear(UNCOLORED);
    Serial.println("Start paint!");
    for (int i=-DISPLAY_Y_PIXEL/2/RESOLUTION; i<DISPLAY_Y_PIXEL/2/RESOLUTION; i++) {
      for (int j=-DISPLAY_X_PIXEL/2/RESOLUTION; j<DISPLAY_X_PIXEL/2/RESOLUTION; j++) {
        Serial.print(j);
        Serial.print(" ");
        Serial.println(i);
        int pixel = pixelState((float)i*DISPLAY_X_LENGTH*RESOLUTION/DISPLAY_X_PIXEL, (float)j*DISPLAY_Y_LENGTH*RESOLUTION/DISPLAY_Y_PIXEL);
        if (pixel == 2){ //IN
          paint.DrawFilledRectangle(j*RESOLUTION+DISPLAY_X_PIXEL/2, i*RESOLUTION+DISPLAY_Y_PIXEL/2, (j+1)*RESOLUTION+DISPLAY_X_PIXEL/2, (i+1)*RESOLUTION+DISPLAY_Y_PIXEL/2, COLORED);
        }
        epd.SetPartialWindowBlack(paint.GetImage(),0,0, paint.GetWidth(), paint.GetHeight());
      }
    }
    lat_old = lat_now;
    lon_old = lon_now;
    angle_old = angle;

    paint.SetWidth(CENTER_SQUARE_SIZE);
    paint.SetHeight(CENTER_SQUARE_SIZE);
  
    if (state_new == 0) { //OUT OF AREA
      paint.Clear(UNCOLORED);
      paint.DrawRectangle(0, 0, CENTER_SQUARE_SIZE-1, CENTER_SQUARE_SIZE-1, COLORED);
      paint.DrawLine(0, 0, CENTER_SQUARE_SIZE-1, CENTER_SQUARE_SIZE-1, COLORED);
      paint.DrawLine(CENTER_SQUARE_SIZE-1, 0, 0, CENTER_SQUARE_SIZE-1, COLORED);
    }
    else if (state_new == 1) { //OUT
      paint.Clear(UNCOLORED);
      paint.DrawRectangle(0, 0, CENTER_SQUARE_SIZE-1, CENTER_SQUARE_SIZE, COLORED);
    }
    else if (state_new == 2){ //IN
      paint.Clear(COLORED);
    }
    epd.SetPartialWindowRed(paint.GetImage(), DISPLAY_X_PIXEL/2-CENTER_SQUARE_SIZE/2, DISPLAY_Y_PIXEL/2-CENTER_SQUARE_SIZE/2, paint.GetWidth(), paint.GetHeight());

    epd.DisplayFrame();
    epd.DisplayFrame(IMAGE_BLACK, IMAGE_RED);
  //}
  state_old = state_new;
}
