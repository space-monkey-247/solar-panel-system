#include "SolarPanelEnvironment.h"

SolarPanelEnvironment::SolarPanelEnvironment() {
}

float SolarPanelEnvironment::getSolarPanelTemperature() {
  return solarPanelTemp.getFloatValue() + alterSolarPanelTemp.getFloatValue();
}

float SolarPanelEnvironment::getBoilerTemperature() {
  return boilerTemp.getFloatValue() + alterBoilerTemp.getFloatValue();
}

void SolarPanelEnvironment::init(unsigned long currentMillis) {
  startUpMillis = currentMillis;

  // default values
  switch (systemMode.getIntValue()) {
    case 0: // OFF
      break;
    case 1: // ON
      break;
    case 2: // Vacation
      break;
    case 3: // Auto I
      startPump.setIntValue(60);
      stopPump.setIntValue(30);
      break;
    case 4: // Auto II
      startPump.setIntValue(20);
      stopPump.setIntValue(5);
      break;
    case 5: // Auto II
      alterTargetDeltaValue.setIntValue(20);
      startPump.setIntValue(20);
      stopPump.setIntValue(5);
      break;
  }
}
