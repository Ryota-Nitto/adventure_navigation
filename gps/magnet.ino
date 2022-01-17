void BMX055_Init()
{
 //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Mag);
  Wire.write(0x4B);  // Select Mag register
  Wire.write(0x83);  // Soft reset
  Wire.endTransmission();
  delay(100);
  //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Mag);
  Wire.write(0x4B);  // Select Mag register
  Wire.write(0x01);  // Soft reset
  Wire.endTransmission();
  delay(100);
  //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Mag);
  Wire.write(0x4C);  // Select Mag register
  Wire.write(0x00);  // Normal Mode, ODR = 10 Hz
  Wire.endTransmission();
 //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Mag);
  Wire.write(0x4E);  // Select Mag register
  Wire.write(0x84);  // X, Y, Z-Axis enabled
  Wire.endTransmission();
 //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Mag);
  Wire.write(0x51);  // Select Mag register
  Wire.write(0x04);  // No. of Repetitions for X-Y Axis = 9
  Wire.endTransmission();
 //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Mag);
  Wire.write(0x52);  // Select Mag register
  Wire.write(0x16);  // No. of Repetitions for Z-Axis = 15
  Wire.endTransmission();
}

void BMX055_Mag()
{
  int data[8];
  for (int i = 0; i < 8; i++)
  {
    Wire.beginTransmission(Addr_Mag);
    Wire.write((0x42 + i));    // Select data register
    Wire.endTransmission();
    Wire.requestFrom(Addr_Mag, 1);    // Request 1 byte of data
    // Read 6 bytes of data
    // xMag lsb, xMag msb, yMag lsb, yMag msb, zMag lsb, zMag msb
    if (Wire.available() == 1)
      data[i] = Wire.read();
  }
  // Convert the data
  mag[0] = ((data[1] <<8) | (data[0]>>3));
  if (mag[0] > 4095)  mag[0] -= 8192;
  mag[1] = ((data[3] <<8) | (data[2]>>3));
  if (mag[1] > 4095)  mag[1] -= 8192;
}

void fix(){
  /*int max_buffer[2];
  int min_buffer[2];
  for(int i=0;i<30;i++) {
    BMX055_Mag();
    for(int v=0;v<2;v++) {
      int tmp = mag[v];
      if (i == 0) {
        max_buffer[v] = tmp;
        min_buffer[v] = tmp;
      }
      else {
        if (tmp > max_buffer[v]) {
          max_buffer[v] = tmp;
        }
        if (tmp < min_buffer[v]) {
          min_buffer[v] = tmp;
        }
      }
    }
    delay(500);
    tft.drawString("Last"+String(15-0.5*i)+"sec", 100, 120);
    Serial.print("Last");
    Serial.print(15-0.5*i);
    Serial.println("sec");
  }*/
  for (int i=0;i<2;i++) {
    fix_value[i] = (max_buffer[i] + min_buffer[i])/2;
    fix_max[i] = max_buffer[i] - fix_value[i];
    fix_min[i] = min_buffer[i] - fix_value[i];
  }
}

double compass(int xMag, int yMag) {
  if (yMag-fix_value[1] == 0 && xMag-fix_value[0] == 0) {
    return 0;
  }
  else{
    double theta = atan2((double)(yMag-fix_value[1])/(double)(fix_max[1]-fix_min[1]),
                       (double)(xMag-fix_value[0])/(double)(fix_max[0]-fix_min[0])); //theta(rad)
    return theta;
  }
}
