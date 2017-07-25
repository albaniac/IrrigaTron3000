/*
  Controller for the irrigation system in the greenhouse.

  This controller will monitor light and temperature, and only water the plants
  at night. It has built-in frost and overheat protection, and uses the daytime
  temperature to determine whether it's the growing season yet.

  Growing season detection: when the daytime temperature tips over a certain
  value, it's assumed we're in growing season. Otherwise, we don't bother
  watering.

  Overheat protection: when the daytime temperature rises over the preset safe
  value, it will wait until it cools down enough at night before it starts
  watering.

  Frost protection: when the current temperature is below a given value, it
  will not bother watering the plants, even if it otherwise would have. This
  protects the water hoses in spring, when daytime temperatures might tip over
  growing season values, but night temperatures are still near the freezing
  point.
*/

/*
  LDR values for "night" and "day"
*/
#define LIGHT_NIGHT 750
#define LIHT_DAY 850

/*
  Soil moisture sensor values for dry and wet
*/
#define SOIL_DRY 400
#define SOIL_WET 800

/*
  Temperature values for hot, normal, cold and frost.
*/
// When daytime temp goes over this, wait for temperatures to cool below TEMP_NORMAL
// before watering.
#define TEMP_HOT 40
// Cool enough to water the soil without cooking the roots.
#define TEMP_NORMAL 30
// When daytime temperature doesn't go over this, don't bother watering.
#define TEMP_COLD 10
// Frost protection.
#define TEMP_FROST 5

/*
  LED, TMP36, LDR, soil moisture sensor
*/
#define P_LED 13
#define P_TEMP 14
#define P_LIGHT 15
#define P_SOIL 16

/*
  Solenoids, activated through a SainSmart 8-port relay board.
  Pull LOW to activate; HIGH to deactivate.
*/
#define P_SOLENOID0 9
#define P_SOLENOID1 8
#define P_SOLENOID2 7
#define P_SOLENOID3 6
#define P_SOLENOID4 5
#define P_SOLENOID5 4
#define P_SOLENOID6 3
#define P_SOLENOID7 2

#define SERIAL_BAUD 115200

/*
  Set up some global variables that need to persist across loops
*/
bool grow_season = false;
bool overheat = false;
bool day = false;

void setup() {
  pinMode(P_LED, OUTPUT);
  pinMode(P_SOLENOID0, OUTPUT);
  pinMode(P_SOLENOID1, OUTPUT);
  pinMode(P_SOLENOID2, OUTPUT);
  pinMode(P_SOLENOID3, OUTPUT);
  pinMode(P_SOLENOID4, OUTPUT);
  pinMode(P_SOLENOID5, OUTPUT);
  pinMode(P_SOLENOID6, OUTPUT);
  pinMode(P_SOLENOID7, OUTPUT);
  digitalWrite(P_LED, HIGH);
  digitalWrite(P_SOLENOID0, HIGH);
  digitalWrite(P_SOLENOID1, HIGH);
  digitalWrite(P_SOLENOID2, HIGH);
  digitalWrite(P_SOLENOID3, HIGH);
  digitalWrite(P_SOLENOID4, HIGH);
  digitalWrite(P_SOLENOID5, HIGH);
  digitalWrite(P_SOLENOID6, HIGH);
  digitalWrite(P_SOLENOID7, HIGH);
  pinMode(P_TEMP, INPUT);
  pinMode(P_LIGHT, INPUT);

  // For debugging
  Serial.begin(SERIAL_BAUD);
}

void loop() {
  int temperature = readTemp();
  Serial.print("Temp: ");
  Serial.println(temperature);
  int light = readLight();
  Serial.print("Light: ");
  Serial.println(light);
  if (light > LIHT_DAY) {


/*
      Soil moisture sensor values for dry and wet
    */if (!day) {
      #define SOIL_DRY 200
      #define SOIL_WET 600// It's morning; reset grow_season to false. If we
      // don't, it will be grow season for as long as this unit runs.
      grow_season = false;
      day = true;
    }
    // Turn off the valve
    digitalWrite(P_SOLENOID0, HIGH);
    // Set some states.
    if (temperature >= TEMP_COLD) {
      grow_season = true;
    }
    if (temperature >= TEMP_HOT) {
      overheat = true;
    }
  }
  else if (light < LIGHT_NIGHT) {
    day = false;
    if (temperature <= TEMP_NORMAL) {
      overheat = false;
    }
    if (grow_season && !overheat && temperature > TEMP_FROST) {
      digitalWrite(P_SOLENOID0, LOW);
    }
    else {
      digitalWrite(P_SOLENOID0, HIGH);
    }
  }

  digitalWrite(P_LED, LOW);
  delay(1000);
  digitalWrite(P_LED, HIGH);
  delay(1000);
}

int readTemp() {
  // Converts the raw measurement from the
  // TMP36 into a temperature in centigrade.
  // Contrary to what online tutorials claim,
  // the TMP36 outputs 750mV at 25°C, not 500.
  float raw_val = analogRead(P_TEMP);
  return (((((5000*raw_val)/1024) - 750) / 10) + 25);
}

int readLight() {
  // No fancy stuff, just return the raw
  // measurement. It's for consistency with
  // readTemp()
  return analogRead(P_LIGHT);
}
