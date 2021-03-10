/*
 * wiring-skeleton.cpp
 *
 *  Created on: 15.03.2017
 *      Author: niklausd
 */

#include <Arduino.h>

// PlatformIO libraries
#include <SerialCommand.h>  // pio lib install 173, lib details see https://github.com/kroimon/Arduino-SerialCommand
#include <SpinTimer.h>      // pio lib install 11599, lib details see https://github.com/dniklaus/spin-timer
#include <LcdKeypad.h>      // pio lib install 2114, lib details see https://github.com/dniklaus/arduino-display-lcdkeypad


// private libraries
#include <ProductDebug.h>

// local components (lib folder)
#include <Indicator.h>
#include <MyBuiltinLedIndicatorAdapter.h>
#include <Button.h>
#include <DetectorStrategy.h>
#include <ButtonEdgeDetector.h>
#include <MyButtonAdapter.h>
#include <ArduinoDigitalInPinSupervisor.h>

SerialCommand* sCmd = 0;
LcdKeypad* lcdKeypad = 0;

// indicator implementation for built in LED
Indicator* led  = 0;

void setup()
{
  // setup basic debug environment (heap usage printer, trace ports & dbg cli)
  setupProdDebugEnv();

  lcdKeypad = new LcdKeypad();
  lcdKeypad->setBackLightOn(true);
  sCmd = 0;

  // indicator LED
  led = new Indicator("led", "Built in LED.");
  led->assignAdapter(new MyBuiltinLedIndicatorAdapter());

#ifdef USER_BTN
  new Button(new ArduinoDigitalInPinSupervisor(USER_BTN), new ButtonEdgeDetector(), new MyButtonAdapter(led));
#endif
}

void loop()
{
  // file deepcode ignore CppSameEvalBinaryExpressionfalse: sCmd gets instantiated by setupProdDebugEnv()
  if (0 != sCmd)
  {
    sCmd->readSerial();     // process serial commands
  }
  while (Serial.available() > 0)
  {
    char inChar = Serial.read();
    if (0 != lcdKeypad)
    {
      lcdKeypad->print(inChar);
    }
  }
  scheduleTimers();         // process Timers
}
