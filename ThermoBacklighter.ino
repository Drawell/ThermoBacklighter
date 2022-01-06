#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2
#define DHTTYPE DHT11

#define LED_PIN_RED 11
#define LED_PIN_GREEN 10
#define LED_PIN_BLUE 9

#define BLUE_THRESHOLD 0.0
#define CYAN_THRESHOLD 5.0
#define GREEN_THRESHOLD 10.0
#define YELLOW_THRESHOLD 15.0
#define RED_THRESHOLD 20.0

void calc_color(float temperature, int *red, int *green, int *blue)
{
  if (temperature > RED_THRESHOLD) // red
  {
    *red = 255;
    *green = 0;
    *blue = 0;
  }
  else if (temperature > YELLOW_THRESHOLD) // orange, yellow to red
  {
    *red = 255;
    // green is fading
    *green = (int)(255. * (RED_THRESHOLD - temperature) / (RED_THRESHOLD - YELLOW_THRESHOLD));
    *blue = 0;
  }
  else if (temperature > GREEN_THRESHOLD) // green to yellow
  {
    // red is growing
    *red = (int)(255. * (temperature - GREEN_THRESHOLD) / (YELLOW_THRESHOLD - GREEN_THRESHOLD));
    *green = 255;
    *blue = 0;
  }
  else if (temperature > CYAN_THRESHOLD) // cyan to green
  {
    *red = 0;
    *green = 255;
    // blue is fading
    *blue = (int)(255. * (GREEN_THRESHOLD - temperature) / (GREEN_THRESHOLD - CYAN_THRESHOLD));
  }
  else if (temperature > BLUE_THRESHOLD) // blue to cyan
  {
    *red = 0;
    // green is growing
    *green = (int)(255. * (temperature - BLUE_THRESHOLD) / (CYAN_THRESHOLD - BLUE_THRESHOLD));
    *blue = 255;
  }
  else
  {
    *red = 0;
    *green = 0;
    *blue = 255;
  }
}

void test(float temperature)
{
  int red, green, blue;
  calc_color(temperature, &red, &green, &blue);
  Serial.print(temperature);
  Serial.print(F(" "));
  Serial.print(red);
  Serial.print(F(" "));
  Serial.print(green);
  Serial.print(F(" "));
  Serial.print(blue);
  Serial.println(F(" "));
}

DHT_Unified dht(DHTPIN, DHTTYPE);

void setup()
{
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);
  pinMode(LED_PIN_BLUE, OUTPUT);

  Serial.begin(9600);
  Serial.println("DHTxx test!");

  dht.begin();
}

void loop()
{
  int red, green, blue;
  float temperature;

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    Serial.println(F("Error reading temperature!"));
    analogWrite(LED_PIN_GREEN, 0);
    analogWrite(LED_PIN_BLUE, 0);

    analogWrite(LED_PIN_RED, 255);
    delay(2000);
    analogWrite(LED_PIN_RED, 0);
    delay(2000);
  }
  else
  {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));

    temperature = (float)event.temperature;
    calc_color(temperature, &red, &green, &blue);
    analogWrite(LED_PIN_RED, red);
    analogWrite(LED_PIN_GREEN, green);
    analogWrite(LED_PIN_BLUE, blue);    
  }
}
