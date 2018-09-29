#include "Variable.h"

using namespace std;

class SolarPanelEnvironment {

  public:

    int DEFAULT_READ_INTERVAL = 5000;

    SolarPanelEnvironment();

    char* deviceLabel;
    bool pumpON = false;
    float targetDelta;
    float start;
    float stop;
    bool pumpOnPreviousValue = false;
    unsigned long startUpMillis = 0;
    unsigned long previousMillis = 0;
    unsigned long upTime = 0;
    unsigned long cycleNo = 0;

    // current status
    Variable boilerTemp = Variable("boiler-temperature");
    Variable solarPanelTemp = Variable("solar-panel-temperature");
    Variable pumpStatus = Variable("pump-status", "0");

    // control panel
    bool firstRun = true;
    Variable controlPanelVariablesChanged = Variable("control-panel-variables-changed", "1");

    /**
    SystemMode {
       0 OFF      #ff2500
       1 ON       #4BA651
       2 Vacation #ffec02
       3 Auto I   #169c8d
       4 Auto II  #169c8d
       5 Auto III  #169c8d
    }
    */
    Variable systemMode = Variable("system-mode", "5");
    Variable systemRunningTime = Variable("system-running-time", "0");
    Variable cycles = Variable("cycle-number", "0");
    Variable messages = Variable("messages");

    // running
    Variable targetBoilerTemp = Variable("target-boiler-temperature", "65");
    Variable minRunningTemperature = Variable("min-running-temperature", "40");
    Variable startPump = Variable("pump-start");
    Variable stopPump = Variable("pump-stop");

    // alter values
    Variable alterSolarPanelTemp = Variable("alter-solar-panel-temperature", "0");
    Variable alterTargetDeltaValue = Variable("alter-delta-value", "15");
    Variable alterBoilerTemp = Variable("alter-boiler-temperature", "0");

    // safety
    Variable solarPanelMaxTemp = Variable("solar-panel-max-temperature", "98");
    Variable solarPanelMinTemp = Variable("solar-panel-min-temperature", "2");

    Variable* downloadVariables[10] = {
      &systemMode,
      &targetBoilerTemp,
      &minRunningTemperature,
      &startPump,
      &stopPump,
      &alterBoilerTemp,
      &alterSolarPanelTemp,
      &alterTargetDeltaValue,
      &solarPanelMaxTemp,
      &solarPanelMinTemp
    };

	  int downloadVariablesSize = sizeof(downloadVariables) / sizeof(downloadVariables[0]);
    int variableDownloadIndex = 0;

    void init(unsigned long currentMillis);
    bool isPanelSafetyON();
    void checkPumpONState();
    void exceptionHandling();

    float getSolarPanelTemperature();
    float getBoilerTemperature();

};
