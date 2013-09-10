// threshold values in degrees F
#define SENSOR_1_TARGET_1 150.0
#define SENSOR_2_TARGET_1 160.0
#define SENSOR_1_TARGET_2 160.0
#define SENSOR_2_TARGET_2 170.0

#define MILLIS_TIL_NEXT_TARGET 1800000 // 30 min = 1800000 ms

// constants for conversion to degrees F
#define ADC_SCALE 1.0
#define ADC_OFFSET 0.0

// higher temp -> lower ADC value, higher ADC value -> lower temp. if adc is
// too high (temp is too low), assume sensor is disconnected and ignore.
#define ADC_MAX 1000
#define valid_gt(val, limit) ((val > 0) && (val > limit))
#define valid_lt(val, limit) ((val > 0) && (val < limit))

#define SENSOR_1 A0
#define SENSOR_2 A1
#define HEATER 11

float sensor1_target, sensor2_target;

void setup() {
  Serial.begin(9600);
  pinMode(HEATER, OUTPUT);
  pinMode(SENSOR_1, INPUT);
  pinMode(SENSOR_2, INPUT);
  sensor1_target = SENSOR_1_TARGET_1;
  sensor2_target = SENSOR_2_TARGET_1;
}

void loop() {
  float sensor1 = read(SENSOR_1);
  float sensor2 = read(SENSOR_2);

  if (sensor1 < 0 || sensor2 < 0 ) {
    digitalWrite(HEATER, LOW); // turn heater off
  }
  else if (valid_gt(sensor1, sensor1_target) ||
    valid_gt(sensor2, sensor2_target) ) {
    digitalWrite(HEATER, LOW); // turn heater off
  }
  else if (valid_lt(sensor1, sensor1_target) ||
    valid_lt(sensor2, sensor2_target) ) {
    digitalWrite(HEATER, HIGH); // turn heater on
  }

  Serial.print(sensor1);
  Serial.print(sensor2);
  Serial.println("----");

  if (millis() > MILLIS_TIL_NEXT_TARGET) {
    sensor1_target = SENSOR_1_TARGET_2;
    sensor2_target = SENSOR_2_TARGET_2;
    Serial.println("Switched target temperature!");
  }
}

float read(int sensor_pin) {
  int val = analogRead(sensor_pin);
  if (val > ADC_MAX) return -1;
  return ADC_SCALE*val + ADC_OFFSET;
}
