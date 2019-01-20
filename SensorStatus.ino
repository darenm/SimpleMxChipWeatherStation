#include "AZ3166WiFi.h"
#include "Sensor.h"
#include "SystemVersion.h"
#include "http_client.h"
#include "telemetry.h"

// Insert the Azure IoT SDK header files here
#include "AzureIotHub.h"
#include "AZ3166WiFi.h"
#include "DevK

DevI2C *ext_i2c;

LIS2MDLSensor *magnetometer;


int axes[3];
char headingBuffer[128];
double windSpeedMs = 5.2;

// Add required member variables



void setHeadingBuffer(int heading)
{
  if (heading > 338 || heading < 22)
  {
    snprintf(headingBuffer, 128, "NORTH");
  }

  if (heading > 22 && heading < 68)
  {
    snprintf(headingBuffer, 128, "NORTH-EAST");
  }

  if (heading > 68 && heading < 113)
  {
    snprintf(headingBuffer, 128, "EAST");
  }

  if (heading > 113 && heading < 158)
  {
    snprintf(headingBuffer, 128, "SOUTH-EAST");
  }

  if (heading > 158 && heading < 203)
  {
    snprintf(headingBuffer, 128, "SOUTH");
  }

  if (heading > 203 && heading < 248)
  {
    snprintf(headingBuffer, 128, "SOUTH-WEST");
  }

  if (heading > 248 && heading < 293)
  {
    snprintf(headingBuffer, 128, "WEST");
  }

  if (heading > 293 && heading < 338)
  {
    snprintf(headingBuffer, 128, "NORTH-WEST");
  }

}

void showMagneticSensor()
{
  magnetometer->getMAxes(axes);

  int heading=atan2(axes[0], axes[1])/0.0174532925; //Calculate the degree using X and Y parameters with this formulae
  //Convert result into 0 to 360
  if(heading < 0) {
    heading+=360;
  }
  heading = 360-heading;

  setHeadingBuffer(heading);

  char buff[128];
  snprintf(buff, 128, "Wind Data \r\n Heading: %d \r\n %s \r\n Speed: %f.2 m/s", heading, headingBuffer, windSpeedMs);
  Screen.print(buff);
}

bool IsButtonClicked(unsigned char ulPin)
{
    pinMode(ulPin, INPUT);
    int buttonState = digitalRead(ulPin);
    if(buttonState == LOW)
    {
        return true;
    }
    return false;
}


void setup() {
  pinMode(LED_WIFI, OUTPUT);
  pinMode(LED_AZURE, OUTPUT);
  pinMode(LED_USER, OUTPUT);

  ext_i2c = new DevI2C(D14, D15);
  
  magnetometer = new LIS2MDLSensor(*ext_i2c);
  magnetometer->init(NULL);
}


void loop() {
  // put your main code here, to run repeatedly:
  showMagneticSensor();
	delay(100);

  if (IsButtonClicked(USER_BUTTON_A)) {
    windSpeedMs -= 0.2;
    if (windSpeedMs < 0) {
      windSpeedMs = 0;
    }
  }
  
  if (IsButtonClicked(USER_BUTTON_B)) {
    windSpeedMs += 0.2;
    if (windSpeedMs > 50) {
      windSpeedMs = 50;
    }
  }

  // Insert Event Upload to IoT Hub Code here

  // #FF3EDA: Describe what the code does

  delay(100);
}
