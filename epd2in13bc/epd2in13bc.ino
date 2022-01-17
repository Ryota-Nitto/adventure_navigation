/**
 *  @filename   :   epd2in13b-demo.ino
 *  @brief      :   2.13inch e-paper display (B) demo
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     July 17 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <SPI.h>
#include "epd2in13b.h"
#include "imagedata.h"
#include "epdpaint.h"

#define COLORED     0
#define UNCOLORED   1

Epd epd;
unsigned char image[3120];
Paint paint(image, 104, 30);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
//  Epd epd;
//  Serial.println("Epd epd");

  if (epd.Init() != 0) {
    Serial.print("e-Paper init failed");
    return;
  }

  /* This clears the SRAM of the e-paper display */
  epd.ClearFrame();
  Serial.println("epd.ClearFrame");

  /**
    * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
    * In this case, a smaller image buffer is allocated and you have to 
    * update a partial display several times.
    * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
    */
 unsigned char image[11024];
  
 Paint paint(image, 104, 30);    //width should be the multiple of 8 

 paint.Clear(UNCOLORED);
 Serial.println("paint.Clear");
 paint.DrawStringAt(8, 2, "Nitto Ryota", &Font12, COLORED);
 Serial.println("paint.DrawStringAt");
 epd.SetPartialWindowBlack(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
 Serial.println("epd.SetPartialWindowBlack");

 paint.Clear(COLORED);
 Serial.println("paint.Clear");
 paint.DrawStringAt(8, 2, "Hello world", &Font12, UNCOLORED);
 Serial.println("paint.DrawStringAt");
 epd.SetPartialWindowBlack(paint.GetImage(), 0, 106, paint.GetWidth(), paint.GetHeight());
 Serial.println("epd.SetPartialWindowBlack");
  
 paint.SetWidth(64);
 paint.SetHeight(64);

 paint.Clear(UNCOLORED);
 paint.DrawRectangle(0, 0, 40, 50, COLORED);
 paint.DrawLine(0, 0, 40, 50, COLORED);
 paint.DrawLine(40, 0, 0, 50, COLORED);
 epd.SetPartialWindowBlack(paint.GetImage(), 8, 72, paint.GetWidth(), paint.GetHeight());
 
 paint.Clear(UNCOLORED);
 paint.DrawCircle(16, 16, 15, COLORED);
 epd.SetPartialWindowBlack(paint.GetImage(), 64, 72, paint.GetWidth(), paint.GetHeight());

 paint.Clear(UNCOLORED);
 paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
 epd.SetPartialWindowRed(paint.GetImage(), 8, 144, paint.GetWidth(), paint.GetHeight());

 paint.Clear(UNCOLORED);
 paint.DrawFilledCircle(16, 16, 15, COLORED);
 epd.SetPartialWindowRed(paint.GetImage(), 64, 144, paint.GetWidth(), paint.GetHeight());

 /* This displays the data from the SRAM in e-Paper module */
 epd.DisplayFrame();
 Serial.println("epd.DisplayFrame");

 /* This displays an image */
 epd.DisplayFrame(IMAGE_BLACK, IMAGE_RED);
 Serial.println("epd.DisplayFrame");

 /* Deep sleep */
 epd.Sleep();
 Serial.println("epd.Sleep");
}

void loop() {
  repeat();
}
