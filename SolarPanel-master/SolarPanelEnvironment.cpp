#include "SolarPanelEnvironment.h"

SolarPanelEnvironment::SolarPanelEnvironment() {
}

float SolarPanelEnvironment::getSolarPanelTemperature() {
  return solarPanelTemp.getFloatValue() + alterSolarPanelTemp.getFloatValue();
}

float SolarPanelEnvironment::getBoilerTemperature() {
  return boilerTemp.getFloatValue() + alterBoilerTemp.getFloatValue();
}

bool SolarPanelEnvironment::isPanelSafetyON() {
  return getSolarPanelTemperature() >= solarPanelMaxTemp.getFloatValue() ||
  		getSolarPanelTemperature() <= solarPanelMinTemp.getFloatValue();
}

void SolarPanelEnvironment::checkPumpONState() {
  if ((isPanelSafetyON() == true) || (
		  (getSolarPanelTemperature() >= getBoilerTemperature() + start) &&
      (getSolarPanelTemperature() >= minRunningTemperature.getFloatValue())
    )) {
    // getSolarPanelTemperature() >= 45 + 22.75 = 67.75
		pumpON = true;
	}

	if (!isPanelSafetyON() && pumpON &&
		(getSolarPanelTemperature() <= getBoilerTemperature() + stop ||
     getBoilerTemperature() > targetBoilerTemp.getFloatValue())) {
    // getSolarPanelTemperature() <= 45 + 5.25 = 50.25
		pumpON = false;
	}

  exceptionHandling();
}

void SolarPanelEnvironment::exceptionHandling() {
  // exception handling
  float wireSensorErrorTemp = -127;
  if (wireSensorErrorTemp == solarPanelTemp.getFloatValue() ||
      wireSensorErrorTemp == boilerTemp.getFloatValue()) {
    messages.setStringValue(String("[Error] Some sensor wires are disconnected."));
    pumpON = true;
    return;
  }
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
