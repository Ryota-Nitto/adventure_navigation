File myFile;
char readingChar; //今読み込んできた文字
String MPLAT_RAW; //緯度1度あたりのmの生データ
String MPLON_RAW; //経度1度あたりのmの生データ
String INTERVAL_LENGTH_RAW; //補完後のpoint間距離(m)の生データ
String ORIGIN_RAW[2]; //原点座標の生データ
String TERMINAL_RAW[2];
String trackPoint_raw[2][NUMBER_OF_POINTS];
String H_RAW; //水平距離の生データ
String V_RAW; //垂直距離の生データ
int lineCounter = 0; //何行めを読み込んでいるか
int elementCounter = 0; //何pointめを読み込んでいるか

void readingSD(String output) {
  myFile = SD.open(output, FILE_READ);
  if (myFile) {
    Serial.println(output);
    tft.drawString(output, 100, 80);
 
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      readingChar = myFile.read();
      //Serial.write(readingChar);
      if(readingChar != '\n' && lineCounter == 0){
        MPLAT_RAW += readingChar;
      }
      else if(readingChar == '\n' && lineCounter == 0){
        lineCounter += 1;
      }
      else if(readingChar != '\n' && lineCounter == 1){
        MPLON_RAW += readingChar;
      }
      else if(readingChar == '\n' && lineCounter == 1){
        lineCounter += 1;
      }
      else if(readingChar != '\n' && lineCounter == 2){
        INTERVAL_LENGTH_RAW += readingChar;
      }
      else if(readingChar == '\n' && lineCounter == 2){
        lineCounter += 1;
      }
      else if (readingChar != '\n' && lineCounter == 3){
        ORIGIN_RAW[0] += readingChar;
      }
      else if(readingChar == '\n' && lineCounter == 3){
        lineCounter += 1;
      }
      else if(readingChar != '\n' && lineCounter == 4){
        ORIGIN_RAW[1] += readingChar;
      }
      else if(readingChar == '\n' && lineCounter == 4){
        lineCounter += 1;
      }
      else if(readingChar != '\n' && lineCounter == 5){
        TERMINAL_RAW[0] += readingChar;
      }
      else if(readingChar == '\n' && lineCounter == 5){
        lineCounter += 1;
      }
      else if(readingChar != '\n' && lineCounter == 6){
        TERMINAL_RAW[1] += readingChar;
      }
      else if(readingChar == '\n' && lineCounter == 6){
        lineCounter += 1;
      }
      else if(readingChar != '\n' && lineCounter == 7){
        H_RAW += readingChar;
      }
      else if(readingChar == '\n' && lineCounter == 7){
        lineCounter += 1;
      }
      else if(readingChar != '\n' && lineCounter == 8){
        V_RAW += readingChar;
      }
      else if(readingChar == '\n' && lineCounter == 8){
        lineCounter += 1;
      }
      else if(readingChar != '\n' && lineCounter == 9){
        if (readingChar != ','){
            trackPoint_raw[0][elementCounter] += readingChar;
        }
        else if (readingChar == ','){
            elementCounter += 1;
        }
      } 
      else if(readingChar == '\n' && lineCounter == 9){
        lineCounter += 1;
        elementCounter = 0;
      }
      else if(readingChar != '\n' && lineCounter == 10){
        if (readingChar != ','){
            trackPoint_raw[1][elementCounter] += readingChar;
        }
        else if (readingChar == ','){
            elementCounter += 1;
        }
      }
    }
    // close the file:
    myFile.close();
    Serial.println("end");
    
    MPLAT = MPLAT_RAW.toFloat();
    MPLON = MPLON_RAW.toFloat();
    INTERVAL_LENGTH = INTERVAL_LENGTH_RAW.toFloat();
    ORIGIN[0] = ORIGIN_RAW[0].toFloat();
    ORIGIN[1] = ORIGIN_RAW[1].toFloat();
    TERMINAL[0] = TERMINAL_RAW[0].toFloat();
    TERMINAL[1] = TERMINAL_RAW[1].toFloat();
    H = H_RAW.toFloat();
    V = V_RAW.toFloat();
    for (int i = 0; i < NUMBER_OF_POINTS; i++){
      trackLat[i] = trackPoint_raw[0][i].toFloat();
      trackLon[i] = trackPoint_raw[1][i].toFloat();
    }
    NEARBY_LENGTH = pow((pow(DISPLAY_X_LENGTH,2) + pow(DISPLAY_Y_LENGTH,2)),0.5)/2 + INTERVAL_LENGTH;
    //tft.drawString(MPLAT_RAW,100,120);
    //tft.drawString(MPLON_RAW,100,160);
    //tft.drawString(INTERVAL_LENGTH_RAW,100,120);
    //tft.drawString(trackPoint_raw[0][0], 100, 120);
    //tft.drawString(trackPoint_raw[1][0], 100, 160);
    
  }
  else{
    // if the file didn't open, print an error:
    Serial.println("error opening");
  }
}
