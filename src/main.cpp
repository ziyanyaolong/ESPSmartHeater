#include <Arduino.h>

#define ZLHS_DEBUG_MODE_ON
#include <ZLHeatingStage.h>

#include <Peripheral/ZL_CS1238/ZL_CS1238.h>
#include <Peripheral/Heater/Heater.h>
#include <Peripheral/DisplayDriver/DisplayDriver.h>
#include <Peripheral/LVGLDriver/LVGLDriver.h>
#include <Peripheral/EncoderDriver/EncoderDriver.h>
#include <Peripheral/ZL_INA219/ZL_INA219.h>
#include <Peripheral/ZL_Buzzer/ZL_Buzzer.h>

#include <Software/TemperatureControl/TemperatureControl.h>
#include <Software/BlinkerManager/BlinkerManager.h>
#include <Software/LVGLManager/LVGLManager.h>

// #include <TFT_eSPI.h>

// TFT_eSPI tft = TFT_eSPI();

void setup()
{
  // 初始化串口
  Serial.begin(115200);

  delay(1000);
  disableCore0WDT();
  disableCore1WDT();
  pinMode(ZLHS_FAN_PIN, OUTPUT);
  digitalWrite(ZLHS_FAN_PIN, LOW);
  // ledcSetup(5, 2000, 8);
  // ledcAttachPin(ZLHS_FAN_PIN, 5);
  // ledcWrite(5, pow(2, 8) / 2.0f);
  // // 启动系统
  ZLHeatingStage::start();

  // 把硬件驱动和软件程序注册至系统中
  // 硬件驱动
  ZLHeatingStage::regNew<ZL_CS1238>();
  ZLHeatingStage::regNew<Heater>();
  ZLHeatingStage::regNew<ZL_INA219>();
  ZLHeatingStage::regNew<ZL_Buzzer>();
  ZLHeatingStage::regNew<DisplayDriver>();
  ZLHeatingStage::regNew<LVGLDriver>();
  ZLHeatingStage::regNew<EncoderDriver>();

  // 软件程序
  // ZLHeatingStage::regNew<BlinkerManager>();
  ZLHeatingStage::regNew<TemperatureControl>();
  ZLHeatingStage::regNew<LVGLManager>();
  // tft.begin();
  // delay(500);
  delay(500);
}

void loop()
{
  // 删除Arduino的线程(默认在Core 1核上)
  // vTaskDelete(nullptr);

  vTaskDelay(200);

  if (Serial.available())
  {
    if (Serial.readString().equals("p"))
    {
      ESP.restart();
    }
  }
}