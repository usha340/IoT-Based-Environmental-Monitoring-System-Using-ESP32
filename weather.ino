#define  BLYNK_TEMPLATE_ID "TMPL3Fgpui4vD"
#define BLYNK_TEMPLATE_NAME "Weather Monitoring system"

//Include the library files
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <BlynkSimpleEsp32.h>
#include <wifi.h>
#include <DHT.h>
#include <SFE_BMP180.h>

#define LDR 26
#define TH 29
#define Rain 3


//Create three variables for pressure
double T, P;
char status;

//Initialize the LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Create an object for the BMP180 sensor
SFE_BMP180 bmp;

DHT dht(TH, DHT11);
BlynkTimer timer;

// Enter your Auth token
char auth[] = "2rBjFCMa_mYVWncc-scTODiUse0tHS9w";

//Enter your WIFI SSID and password
char ssid[] = "Redmi Note 11 Pro+ 5G";
char pass[] = "nani 1234";

void setup() {
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  bmp.begin();
  dht.begin();
  lcd.init();
  lcd.backlight();
  pinMode(LDR, INPUT);
  pinMode(Rain, INPUT);
  analogReadResolution(12);

  lcd.setCursor(0, 0);
  lcd.print("System");
  lcd.setCursor(4, 1);
  lcd.print("Loading..");
  delay(4000);
  lcd.clear();

}

//Get the DHT11 sensor values
void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(t);

  lcd.setCursor(8, 0);
  lcd.print("H:");
  lcd.print(h);

}

//Get the rain sensor values
void rainSensor() {
  int Rvalue = analogRead(Rain);
  Rvalue = map(Rvalue, 0, 4095, 0, 100);
  Rvalue = (Rvalue - 100) * -1;
  Blynk.virtualWrite(V2, Rvalue);

  lcd.setCursor(0, 1);
  lcd.print("R:");
  lcd.print(Rvalue);
  lcd.print(" ");
  Serial.println(Rvalue);
}

//Get the pressure values
void pressure() {
  status =  bmp.startTemperature();
  if (status != 0) {
    delay(status);
    status = bmp.getTemperature(T);

    status = bmp.startPressure(3);// 0 to 3
    if (status != 0) {
      delay(status);
      status = bmp.getPressure(P, T);
      if (status != 0) {

      }
    }
  }

  Blynk.virtualWrite(V3, P);
  lcd.setCursor(8, 1);
  lcd.print("P:");
  lcd.print(P);
}

//Get the LDR sensor values
void LDRsensor() {
  bool value = digitalRead(LDR);
  if (value == 1) {
    WidgetLED LED(V4);
    LED.on();
  } else {
    WidgetLED LED(V4);
    LED.off();
  }

}

void loop() {
  DHT11sensor();
  rainSensor();
  pressure();  
  LDRsensor();
  Blynk.run();//Run the Blynk library
}