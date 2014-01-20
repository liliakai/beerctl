// threshold values in degrees F
#define RED_TARGET_1 102 // 155.0  //
#define BLUE_TARGET_1 RED_TARGET_1 + 5.7 // 150.0
#define CORRECTION -6 // it thinks actual 98.3 is 104.3 in 110 out

#define MILLIS_TIL_SECOND_TARGET 60000 * 45 // 60000 = 1 minute
#define RED_TARGET_2 170.0  // 
#define BLUE_TARGET_2 160.0

#define MILLIS_TIL_THIRD_TARGET 60000 * 60 // 60000 = 1 minute
#define RED_TARGET_3 0.0  // above number is TOTAL TIME since start 
#define BLUE_TARGET_3 0.0
#define AVG_CYCLES 50 // how many times to take a reading at a time

// constants for conversion to degrees F

// blue adc = -6.4288x + 1271.529
// (adc - 1271.529) / -6.4288 = temp in F
#define BLUE_OFFSET 1322.95 // 1271.529
#define BLUE_SLOPE -6.4288
// red adc = -6.2782x + 1292.983
#define RED_OFFSET 1311.81 // 1292.983
#define RED_SLOPE -6.2782

// higher temp -> lower ADC value, higher ADC value -> lower temp. if adc is
// too high (temp is too low), assume sensor is disconnected and fail off.
#define ADC_MAX 1000

#define REDPIN A0
#define BLUEPIN A1
#define HEATER 11

#define DOWNTIME 10000 // minimum off time before turning on again

int target = 1;  // which target are we on?
unsigned long timeNow, lastOn = 0;
float red_target, blue_target;

void setup() {
  Serial.begin(9600);
  pinMode(HEATER, OUTPUT);
  pinMode(REDPIN, INPUT);
  pinMode(BLUEPIN, INPUT);
  red_target = RED_TARGET_1;
  blue_target = BLUE_TARGET_1;
}

void loop() {
  timeNow = millis();
  
  float red = readSensor(REDPIN);
  float blue = readSensor(BLUEPIN);
  red += CORRECTION;
  blue += CORRECTION;
  
  Serial.print("RED: ");
  Serial.print(red);
  Serial.print("  BLUE: ");
  Serial.println(blue);

  if (red < -999 || blue < -999 ) {
    Serial.println("ERROR:  one of the sensors is disconnected!");
    digitalWrite(HEATER, LOW); // turn heater off
    lastOn = timeNow;
  }
  else if ((red > red_target) || (blue > blue_target)) {
    Serial.print("00000");
    digitalWrite(HEATER, LOW); // turn heater off
    lastOn = timeNow;
  }
  else if ((red < red_target) && (blue < blue_target)) {
    Serial.print("11111");
    if (timeNow - lastOn > DOWNTIME) {
      Serial.print("turning on heater");
      digitalWrite(HEATER, HIGH); // turn heater on
    }
  }
/*
  if ((millis() > MILLIS_TIL_THIRD_TARGET) && (target == 2)) {
    target = 3;
    red_target = RED_TARGET_3;
    blue_target = BLUE_TARGET_3;
   Serial.print("THIRD target temperatures: ");
   Serial.print(blue_target);
   Serial.print("  ");
   Serial.println(red_target);
  }
  
  if ((millis() > MILLIS_TIL_SECOND_TARGET) && (target == 1)) {
    target = 2;
    red_target = RED_TARGET_2;
    blue_target = BLUE_TARGET_2;
   Serial.print("SECOND target temperatures: ");
   Serial.print(blue_target);
   Serial.print("  ");
   Serial.println(red_target);
  }
*/
  delay(100);
}

float readSensor(int sensor_pin) {
  unsigned long adder = 0;
  for (int i = 0; i < AVG_CYCLES; i++) adder += analogRead(sensor_pin);
  delay(5);
  float val = adder / AVG_CYCLES;
  if (val > ADC_MAX) return -1000.0;
// (adc - 1271.529) / -6.4288 = temp in F
  if (sensor_pin == REDPIN) return (val - RED_OFFSET)/RED_SLOPE;
  if (sensor_pin == BLUEPIN) return (val - BLUE_OFFSET)/BLUE_SLOPE;
}
