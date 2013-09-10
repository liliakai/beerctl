// threshold values in degrees F
#define SENSOR_1_UPPER_THRESH 120.0
#define SENSOR_2_UPPER_THRESH 120.0
#define SENSOR_1_LOWER_THRESH 100.0
#define SENSOR_2_LOWER_THRESH 100.0

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

void setup() {
  Serial.begin(9600);
  pinMode(HEATER, OUTPUT);
  pinMode(SENSOR_1, INPUT);
  pinMode(SENSOR_2, INPUT);
}

void loop() {
  float sensor1 = read(SENSOR_1);
  float sensor2 = read(SENSOR_2);

  if (valid_gt(sensor1, SENSOR_1_UPPER_THRESH) ||
      valid_gt(sensor2, SENSOR_2_UPPER_THRESH) ) {
    digitalWrite(HEATER, LOW); // turn heater off
  }

  if (valid_lt(sensor1, SENSOR_1_LOWER_THRESH) ||
      valid_lt(sensor2, SENSOR_2_LOWER_THRESH) ) {
    digitalWrite(HEATER, HIGH); // turn heater on
  }
  Serial.print(sensor1);
  Serial.print(sensor2);
  Serial.println('----');
}

float read(int sensor_pin) {
  int val = analogRead(sensor_pin);
  if (val > ADC_MAX) return -1;
  return ADC_SCALE*val + ADC_OFFSET;
}
