void repeat() {
  // put your main code here, to run repeatedly:
  Serial.println("paint.Clear...");
  paint.Clear(COLORED);
  Serial.println("paint.DrawStringAt...");
  paint.DrawStringAt(8, 2, "Nitto Ryota", &Font12, UNCOLORED);
  Serial.println("epd.SetPartialWindowBlack...");
  epd.SetPartialWindowBlack(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());

  Serial.println("paint.Clear...");
  paint.Clear(UNCOLORED);
  Serial.println("paint.DrawStringAt...");
  paint.DrawStringAt(8, 2, "Hello world", &Font12, COLORED);
  Serial.println("epd.SetPartialWindowBlack...");
  epd.SetPartialWindowBlack(paint.GetImage(), 0, 106, paint.GetWidth(), paint.GetHeight());

  Serial.println("epd.DisplayFrame...");
  epd.DisplayFrame();
  Serial.println("epd.DisplayFrame...");
  epd.DisplayFrame(IMAGE_BLACK, IMAGE_RED);

  Serial.println("delay1");
  delay(30000);

  Serial.println("paint.Clear...");
  paint.Clear(UNCOLORED);
  Serial.println("paint.DrawStringAt...");
  paint.DrawStringAt(8, 2, "Nitto Ryota", &Font12, COLORED);
  Serial.println("epd.SetPartialWindowBlack...");
  epd.SetPartialWindowBlack(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());

  Serial.println("paint.Clear...");
  paint.Clear(COLORED);
  Serial.println("paint.DrawStringAt...");
  paint.DrawStringAt(8, 2, "Hello world", &Font12, UNCOLORED);
  Serial.println("epd.SetPartialWindowBlack...");
  epd.SetPartialWindowBlack(paint.GetImage(), 0, 106, paint.GetWidth(), paint.GetHeight());

  Serial.println("epd.DisplayFrame...");
  epd.DisplayFrame();
  Serial.println("epd.DisplayFrame...");
  epd.DisplayFrame(IMAGE_BLACK, IMAGE_RED);

  Serial.println("delay2");
  delay(30000);
}
