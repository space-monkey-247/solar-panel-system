 /********************************************************************/
// First we include the libraries
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include "UbidotsESPMQTT.h"
#include <SolarPanelEnvironment.h>

bool SERIAL_COMMUNICATION_ENABLED = true;


/**** Rooter Bogdan ****************************************************************/
const char* SSID_NAME = "Telekom-rOlKBz"; // Put here your SSID name
const char* SSID_PASS = "36kexrah4e1s"; // Put here your password
/**** Hotspot Tudor ****************************************************************/
// const char* SSID_NAME = "Tudor Hotspot"; // Put here your SSID name
// const char* SSID_PASS = "Tudor123!"; // Put here your password


const char* TOKEN = "A1E-Brpd96xLq77tUwPkpXsXvCHCzdX4dZ";
// device label artan
//const char* DEVICE_LABEL = "wemos-d1-mini";
// device label artan
const char* DEVICE_LABEL = "solar-panel-test-env";
const char* USER_AGENT = "ESP8266";
const char* VERSION = "1.0";
const char* HTTPSERVER = "things.ubidots.com";
int HTTPPORT = 80;

// Pump relay pin
#define PUMP_RELAY_PIN  D7

// http client
WiFiClient clientUbi;
// mqtt cloent
Ubidots mqttClient((char*)TOKEN);

/********************************************************************/
// Data wire is plugged into pin D3 on the Arduino
#define ONE_WIRE_BUS D3

/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

/********************************************************************/
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// LCD + I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// solar panel environment
SolarPanelEnvironment env;

void setup(void) {
  serialSetup();
  oneWireSensorsSetup();
  wifiSetup();
  mqttSetup();
  pumpRelaySetup();
//  lcdSetup();

  env.init(millis());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void mqttSetup() {
  mqttClient.setDebug(true); // Pass a true or false bool value to activate debug messages
  mqttClient.wifiConnection((char *)SSID_NAME, (char *)SSID_PASS);
  mqttClient.begin(callback);
  mqttSubscribeVariables();
}

void mqttSubscribeVariables() {
  mqttClient.ubidotsSubscribe((char *)DEVICE_LABEL, "pump-start");
  mqttClient.ubidotsSubscribe((char *)DEVICE_LABEL, "pump-stop");
}

void mqttPublish() {
  if(!mqttClient.connected()) {
    mqttClient.reconnect();
    mqttSubscribeVariables();
  }
  mqttClient.add("solar-panel-temperature", 10);
  mqttClient.add("boiler-temperature", 20);
  mqttClient.ubidotsPublish((char *)DEVICE_LABEL);
  mqttClient.loop();
}

void serialSetup() {
  // start serial port
  Serial.begin(115200);
  // while (!Serial) {
  //   ; // wait for serial port to connect. Needed for native USB
  // }
}

void pumpRelaySetup() {
  pinMode(PUMP_RELAY_PIN , OUTPUT);
  // turn the pump off by default
  digitalWrite(PUMP_RELAY_PIN , HIGH);
}

void wifiSetup() {
  wifiConnecting(3);
  printWifiStatus();
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  serialPrintF("SSID: %s\n", String(WiFi.SSID()));

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  Serial.println();
}

void wifiConnecting(int retryNo) {
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    WiFi.setAutoReconnect(true);
    serialPrintln("WiFi Connected");
    serialPrintln("");
    return;
  }
  WiFi.begin(SSID_NAME, SSID_PASS);
  Serial.print("Connecting to ");
  Serial.print(SSID_NAME);
  Serial.print(" ");

  int count = 0;
  while (WiFi.status() != WL_CONNECTED && (retryNo == 0 || (count < retryNo))) {
    delay(500);
    Serial.print(".");
    count++;
  }
  Serial.println();
}

void oneWireSensorsSetup() {
  Serial.println("Dallas Temperature IC Control Library Demo");
  // Start up the library
  sensors.begin();
}

//#define BACKLIGHT_PIN     13
void lcdSetup() {
  // initialize the LCD
//  lcd.init();

  // Turn on the backlight.
//  lcd.backlight();
}

void lcdPrint(uint8_t col, uint8_t row, String text) {
//  lcd.setCursor(col, row);
//  lcd.print(text);
}

void lcdPrint(uint8_t col, uint8_t row, char aChar) {
//  lcd.setCursor(col, row);
//  lcd.print(aChar);
}

void loop(void)
{

  // try to reconnecting for 3 times
  wifiConnecting(3);
  // get variables from ubidots
  //downloadAndUpdateEnvironmentVariables();
  // read temperature of wired devices
  //readTemperatures();
  // pump switch on/off based on the environment variables
  //runSystemComputations();
  //updateThePumpStatus();
  // send temperatures to ubidots
  //sendValuesToServer();
  
  // mqtt
  mqttPublish();
  
  serialPrintln("");
  serialPrintln(F("********************************************************************"));

  // sleep 5 secs
  delay(env.DEFAULT_READ_INTERVAL);
}

void serialPrintln(String strValue) {
  if (!SERIAL_COMMUNICATION_ENABLED) {
    return;
  }
  Serial.println(strValue);
}

void serialPrint(String strValue) {
  if (!SERIAL_COMMUNICATION_ENABLED) {
    return;
  }
  Serial.print(strValue);
}

void downloadAndUpdateEnvironmentVariables() {
  if (env.firstRun == false) {
    updateVariable(&env.controlPanelVariablesChanged);
  } else {
    env.firstRun = false;
  }

  // reset the index if the flag is true
  if (env.controlPanelVariablesChanged.getIntValue() == true) {
    env.variableDownloadIndex = 0;
    //env.messages.setStringValue(String("[Variables update] Starting ..."));
  }
  // exit if the iteration has finised updateing all the vars
  if (env.controlPanelVariablesChanged.getIntValue() == false &&
      env.variableDownloadIndex == 0) {
    return;
  }

  // Update environment variables one by one
  Variable& variable = *env.downloadVariables[env.variableDownloadIndex];
  updateVariableByRef(variable);
//  env.messages.setStringValue(String("[Variables update] " + variable.getLabel() + ": done" ));

  env.variableDownloadIndex++;
  // reset index if reaching the end of the array
  if (env.variableDownloadIndex == env.downloadVariablesSize) {
    env.variableDownloadIndex = 0;
//    env.messages.setStringValue(String("[Variables update] Done"));
  }
}

void updateVariableByRef(Variable& variable) {
  String stringValue = getServerValue(variable.getLabel());
  serialPrintCalculatedValue("Old value", variable.getStringValue());
//  Serial.println(variable.getLabel() + " before: " + variable.getStringValue());

  if (stringValue != NULL && stringValue.length() > 0) {
    variable.setStringValue(stringValue);
//    Serial.println(variable.getLabel() + " after1: " + variable.getStringValue());
//    Serial.println(variable.getLabel() + " after2: " + String(variable.getFloatValue()));
  }
}

void updateVariable(Variable* variable) {
  String stringValue = getServerValue(variable->getLabel());
  serialPrintCalculatedValue("Old value", variable->getStringValue());
//  Serial.println(variable->getLabel() + " before: " + variable->getStringValue());

  if (stringValue != NULL && stringValue.length() > 0) {
    variable->setStringValue(stringValue);
//    Serial.println(variable->getLabel() + " after1: " + variable->getStringValue());
//    Serial.println(variable->getLabel() + " after2: " + String(variable->getFloatValue()));
  }
}

void updateVariableValue(Variable variable) {
  String stringValue = getServerValue(variable.getLabel());
  Serial.println(variable.getLabel() + " before: " + variable.getStringValue());

  if (stringValue != NULL && stringValue.length() > 0) {
    variable.setStringValue(stringValue);
    Serial.println(variable.getLabel() + " after1: " + variable.getStringValue());
    Serial.println(variable.getLabel() + " after2: " + String(variable.getFloatValue()));
  }
}

void runSystemComputations() {
  serialPrintln("");
  serialPrintln(F("Computing:"));
  printControlPanelVariables();

  switch (env.systemMode.getIntValue()) {
    case 0:
      // OFF
      env.pumpON = false;
      break;
    case 1:
      // ON
      env.pumpON = true;
      break;
    case 2:
      // Vacation
      env.pumpON = true;
      break;
    case 3:
      // Auto I
      runAutoISystemModeComputations();
      break;
    case 4:
      // Auto II
      runAutoIISystemModeComputations();
      break;
    case 5:
      // Auto III = II + I
      runAutoIIISystemModeComputations();
      break;
  }

  env.checkPumpONState();
  printErrorMessages();

  serialPrintln(" Computed values: ");
  printEnvironmentComtutedValues();
}

void serialPrintFloat(const char* formatedMessage, float floatValue) {
  if (!SERIAL_COMMUNICATION_ENABLED) {
    return;
  }
  char* str = (char *) malloc(sizeof(char) * 255);
  char str_val[30];
  dtostrf(floatValue, 4, 2, str_val);
  sprintf(str, formatedMessage, str_val);
  Serial.printf(str);
  free(str);
}

void serialPrintInt(const char* formatedMessage, int intValue) {
  if (!SERIAL_COMMUNICATION_ENABLED) {
    return;
  }
  char* str = (char *) malloc(sizeof(char) * 255);
  sprintf(str, formatedMessage, intValue);
  Serial.printf(str);
  free(str);
}

void serialPrintF(const char* formatedMessage) {
  if (!SERIAL_COMMUNICATION_ENABLED) {
    return;
  }
//  Serial.print(F(formatedMessage));
  char* str = (char *) malloc(sizeof(char) * 255);
  sprintf(str, formatedMessage);
  Serial.printf(str);
  free(str);
}

void serialPrintF(const char* formatedMessage, String strValue) {
  if (!SERIAL_COMMUNICATION_ENABLED) {
    return;
  }
//  Serial.print(F(formatedMessage));
  char* str = (char *) malloc(sizeof(char) * 255);
  sprintf(str, formatedMessage, strValue.c_str());
  Serial.printf(str);
  free(str);
}

void serialPrintVariable(Variable variable) {
  char* str = (char *) malloc(sizeof(char) * 255);
  sprintf(str, "  %s: ", variable.getLabel().c_str());
  serialPrint(str);
  serialPrintln(variable.getStringValue());
  free(str);
}

void serialPrintCalculatedValue(const char* variableLabel, String value) {
  char* str = (char *) malloc(sizeof(char) * 255);
  sprintf(str, "  %s: ", variableLabel);
  serialPrint(str);
  serialPrintln(value);
  free(str);
}

void printControlPanelVariables() {
  serialPrintln(" Variables: ");
  serialPrintVariable(env.systemRunningTime);
  for (int idx = 0; idx < env.downloadVariablesSize; idx++) {
    serialPrintVariable(*env.downloadVariables[idx]);
  }
}

void runAutoISystemModeComputations() {
  // start & stop are constants
  // start = 10;
  env.start = env.startPump.getFloatValue();
  // stop = 5;
  env.stop = env.stopPump.getFloatValue();
}

void runAutoIISystemModeComputations() {
  // start & stop represent a percentage from targetDelta
  // targetDelta = 65 + 15 - 45 = 80 - 45 = 35
  float targetDelta = env.targetBoilerTemp.getFloatValue()
                    + env.alterTargetDeltaValue.getFloatValue()
                    - env.getBoilerTemperature();
  if (targetDelta < 0) {
    targetDelta = 0;
  }
  // start = 35 * 65 / 100 = 22.75
  env.start = (targetDelta * env.startPump.getFloatValue()) / 100;
  // stop = 35 * 15 / 100 = 5.25
  env.stop = (targetDelta * env.stopPump.getFloatValue()) / 100;
}

void runAutoIIISystemModeComputations() {
  // targetDelta = 65 + 15 - 45 = 80 - 45 = 35
  float targetDelta = env.targetBoilerTemp.getFloatValue()
                    - env.getBoilerTemperature();
  if (targetDelta < 0) {
    targetDelta = 0;
  }
  // start = 35 * 65 / 100 = 22.75
  env.start = env.startPump.getFloatValue() + (targetDelta * env.alterTargetDeltaValue.getFloatValue()) / 100;
  // stop = 35 * 15 / 100 = 5.25
  env.stop = env.stopPump.getFloatValue() + (targetDelta * env.alterTargetDeltaValue.getFloatValue()) / 100;
}

void runAutoIVSystemModeComputations() {
  serialPrintln(" System mode: Auto IV");
  printControlPanelVariables();

  // targetDelta = 65 + 15 - 45 = 80 - 45 = 35
  float targetDelta = env.targetBoilerTemp.getFloatValue()
                    - env.getBoilerTemperature();
  if (targetDelta < 0) {
    targetDelta = 0;
  }
  // start = 35 * 65 / 100 = 22.75
  env.start = env.startPump.getFloatValue()
    + (targetDelta * env.alterTargetDeltaValue.getFloatValue()) / 100;
  // stop = 35 * 15 / 100 = 5.25
  env.stop = env.startPump.getFloatValue()
    + (targetDelta * env.alterTargetDeltaValue.getFloatValue()) / 100;

  env.checkPumpONState();
  printErrorMessages();

  serialPrintln(" Computed values: ");
  printEnvironmentComtutedValues();
}

void printEnvironmentComtutedValues() {
  serialPrintCalculatedValue("targetDelta", String(env.targetDelta));
  serialPrintCalculatedValue("start", String(env.start));
  serialPrintCalculatedValue("stop", String(env.stop));
  serialPrintCalculatedValue("env.getSolarPanelTemperature()", String(env.getSolarPanelTemperature()));
  serialPrintCalculatedValue("env.getBoilerTemperature()", String(env.getBoilerTemperature()));
  serialPrintCalculatedValue("isPanelSafetyON", String(env.isPanelSafetyON()));
  serialPrintCalculatedValue("env.pumpON", String(env.pumpON));
}

void printErrorMessages() {
  if (env.messages.isEmpty() == false) {
    serialPrintF("");
    serialPrintF(env.messages.getStringValue().c_str());
  }
}

void updateThePumpStatus() {
  if (env.pumpON) {
    serialPrintF(" Pump is ON\n");
    digitalWrite(PUMP_RELAY_PIN , LOW);
  } else {
    serialPrintF(" Pump is OFF\n");
    digitalWrite(PUMP_RELAY_PIN , HIGH);
  }
  env.pumpStatus.setIntValue(env.pumpON);
}

void readTemperatures() {
  serialPrintF("\n");
  serialPrintF("Requesting temperatures...\n");
  sensors.requestTemperatures();
  //Serial.println("DONE");

  /*** boiler *****************************************************************/
  float temp1 = sensors.getTempCByIndex(0);
  env.boilerTemp.setFloatValue(temp1);
//  serialPrintFloat(" Boiler temperature is: %.2f\n", temp1);
  serialPrint(" Boiler temperature is: ");
  serialPrintln(String(temp1));

  lcdPrint(0, 0, "Boiler:   " + String(temp1) + " C");
  // celsius sign
  lcdPrint(15, 0, (char)223);

  /*** solar panel *************************************************************/
  float temp2 = sensors.getTempCByIndex(1);
  env.solarPanelTemp.setFloatValue(temp2);
//  serialPrintFloat(" Solar panel temperature is: %.2f\n", temp2);
  serialPrint(" Solar panel temperature is: ");
  serialPrintln(String(temp2));

  lcdPrint(0, 1, "Panel:    " + String(temp2) + " C");
  // celsius sign
  lcdPrint(15, 1, (char)223);
}
void prepareSystemUpTime() {
  // prepare systemRunningTime in minutes
  int oneMinute = 60000;
  unsigned long currentMillis = millis();
  unsigned long interval = currentMillis - env.previousMillis;
  if (currentMillis < env.previousMillis) {
    interval = currentMillis;
    // loosing the interval from the previousMillis until the rollover
  }
  env.previousMillis += interval;
  env.upTime += interval;
  env.cycleNo++;
  env.cycles.setStringValue(String(env.cycleNo));

  unsigned long currentTime = env.upTime / oneMinute;
  env.systemRunningTime.setStringValue(String(currentTime, DEC));
}

char* preparePayload() {
  // Transforms the values of the sensors to char type
  char str_val_1[30];
  char str_val_2[30];
  dtostrf(env.getBoilerTemperature(), 4, 2, str_val_1);
  dtostrf(env.getSolarPanelTemperature(), 4, 2, str_val_2);

  prepareSystemUpTime();

  /* Builds the payload with structure:{"inside-temperature":24.75,"outside-temperature":19.19} */
  char* payload = (char *) malloc(sizeof(char) * 500);
  sprintf(payload, "{\"");
  sprintf(payload, "%s%s\":%s", payload, env.boilerTemp.getLabel().c_str(), str_val_1);
  sprintf(payload, "%s,\"%s\":%s", payload, env.solarPanelTemp.getLabel().c_str(), str_val_2);
//  if ((env.pumpOnPreviousValue > env.pumpON) || (env.pumpOnPreviousValue < env.pumpON)) {
    sprintf(payload, "%s,\"%s\":%s", payload, env.pumpStatus.getLabel().c_str(), env.pumpStatus.getStringValue().c_str());
//    env.pumpOnPreviousValue = env.pumpON;
//  }
//  if (env.messages.isEmpty() == false) {
//    String quotedMessage = String("\"") + env.messages.getStringValue() + String("\"");
//    sprintf(payload, "%s,\"%s\":%s", payload, env.messages.getLabel().c_str(), quotedMessage.c_str());
//    env.messages.setStringValue("");
//  }
  if (env.controlPanelVariablesChanged.getIntValue() == true) {
    env.controlPanelVariablesChanged.setIntValue(false);
    sprintf(payload, "%s,\"%s\":%s", payload, env.controlPanelVariablesChanged.getLabel().c_str(), env.controlPanelVariablesChanged.getStringValue().c_str());
  }
  sprintf(payload, "%s,\"%s\":%s", payload, env.systemRunningTime.getLabel().c_str(), env.systemRunningTime.getStringValue().c_str());
  sprintf(payload, "%s,\"%s\":%s", payload, env.cycles.getLabel().c_str(), env.cycles.getStringValue().c_str());
  sprintf(payload, "%s}", payload);
  return payload;
}

void sendValuesToServer() {
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }
  char* payload = preparePayload();
  char* request = (char *) malloc(sizeof(char) * 1000);

  /* Builds the HTTP request to be POST */
  sprintf(request, "POST /api/v1.6/devices/%s/?force=true HTTP/1.1\r\n", DEVICE_LABEL);
  sprintf(request, "%sHost: things.ubidots.com\r\n", request);
  sprintf(request, "%sUser-Agent: %s/%s\r\n", request, USER_AGENT, VERSION);
  sprintf(request, "%sX-Auth-Token: %s\r\n", request, TOKEN);
  sprintf(request, "%sConnection: close\r\n", request);
  sprintf(request, "%sContent-Type: application/json\r\n", request);
  sprintf(request, "%sContent-Length: %d\r\n\r\n", request, strlen(payload));
  sprintf(request, "%s%s\r\n", request, payload);

  /* Connecting the client */
  clientUbi.connect(HTTPSERVER, HTTPPORT);
  serialPrintF("\n");
  if (clientUbi.connected()) {
    /* Sends the request to the client */
    clientUbi.print(request);
//    Serial.println("Ubidots - POST Request:");
//    Serial.println(request);
  } else {
    serialPrintF("Connection Failed to Ubidots\n");
    // force reconnecting
    WiFi.disconnect();
  }

  /* While the client is available read the response of the server */
  while (clientUbi.available()) {
    char c = clientUbi.read();
    Serial.write(c);
    //Serial.print(c); // Uncomment this line to visualize the response on the Serial Monitor
  }

  /* Free memory */
  free(request);
  /* Stop the client */
  clientUbi.stop();

  serialPrintF("Variables sent to server: \n");
  serialPrintF(payload);
  serialPrintF("\n");

  free(payload);
}

String getServerValue(String variableLabel) {
  if (WiFi.status() != WL_CONNECTED) {
    return "";
  }
  serialPrintF("Geting %s variable\n", variableLabel);

  // http://things.ubidots.com/api/v1.6/devices/{LABEL_DEVICE}/{VARIABLE_LABEL}/lv

  /* Builds the HTTP request to be POST */
  char* request = (char *) malloc(sizeof(char) * 1000);
  sprintf(request, "GET /api/v1.6/devices/%s/%s/lv HTTP/1.1\r\n", DEVICE_LABEL, variableLabel.c_str());
  sprintf(request, "%sHost: things.ubidots.com\r\n", request);
  sprintf(request, "%sUser-Agent: %s/%s\r\n", request, USER_AGENT, VERSION);
  sprintf(request, "%sX-Auth-Token: %s\r\n", request, TOKEN);
  sprintf(request, "%sConnection: close\r\n", request);
  sprintf(request, "%s\r\n", request);

  /* Connecting the client */
  clientUbi.connect(HTTPSERVER, HTTPPORT);
  if (clientUbi.connected()) {
    /* Sends the request to the client */
    clientUbi.print(request);
//    Serial.println("Ubidots - GET Request:");
//    Serial.println(request);
  } else {
    Serial.println("Connection Failed to Ubidots");
    // force reconnecting
    WiFi.disconnect();
  }

    int timeout = 0;
    while (!clientUbi.available() && timeout < 999999) {
        timeout++;
    }
    String response;
    serialPrintInt("  Server timeout: %d\n", timeout);
    while (clientUbi.available()) {
//      Serial.println("Reading response. ");
      response = clientUbi.readString();
    }

//    Serial.println("response1 {");
//    Serial.println(response);
//    Serial.println("}");

    uint8_t bodyPosinit = response.indexOf("\r\n\r\n");
    response = response.substring(bodyPosinit);

//    Serial.println("response2 {");
//    Serial.println(response);
//    Serial.println("}");

    bodyPosinit = 7 + response.indexOf("\r\n");
    response = response.substring(bodyPosinit);
    uint8_t bodyPosend = response.indexOf("\r\n");
    String value = response.substring(0, bodyPosend);
    serialPrintF("  Retrieved value: %s\n", value);
    clientUbi.flush();
    clientUbi.stop();

    free(request);

    return value;
}
