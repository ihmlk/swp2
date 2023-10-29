#include <Servo.h>

// Arduino pin assignment
#define PIN_LED   9   // LED active-low
#define PIN_TRIG  12  // sonar sensor TRIGGER
#define PIN_ECHO  13  // sonar sensor ECHO
#define PIN_SERVO 10  // servi motor

// configurable parameters for sonar
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25      // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300.0   // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL) // coefficent to convert duration to distance

#define _EMA_ALPHA 0.5    // EMA weight of new sample (range: 0 to 1)
                          // Setting EMA to 1 effectively disables EMA filter.

// Target Distance
#define _TARGET_LOW  180.0
#define _TARGET_HIGH 360.0

// duty duration for myservo.writeMicroseconds()
// NEEDS TUNING (servo by servo)
 
#define _DUTY_MIN 400 // servo full clockwise position (0 degree)
#define _DUTY_NEU 1480 // servo neutral position (90 degree)                           //여기 보정
#define _DUTY_MAX 2500 // servo full counterclockwise position (180 degree)

// global variables
float  dist_ema; 
float dist_prev = _DIST_MAX; // unit: mm
unsigned long last_sampling_time;       // unit: ms

Servo myservo;
float USS_measure(int TRIG, int ECHO);
void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);    // sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);     // sonar ECHO
  digitalWrite(PIN_TRIG, LOW);  // turn-off Sonar 

  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);

  // initialize USS related variables
  // raw distance output from USS (unit: mm)

  // initialize serial port
  Serial.begin(57600);
}

void loop() {
  float  dist_raw;
  
  // wait until next sampling time. 
  if (millis() < (last_sampling_time + INTERVAL))
    return;
 
  dist_raw = USS_measure(PIN_TRIG, PIN_ECHO); // read distance

  if (dist_raw < _TARGET_LOW) {
    dist_raw = dist_prev;
    digitalWrite(PIN_LED, 1);       // LED ON
  } else if (dist_raw > _TARGET_HIGH){
     dist_raw = dist_prev;
     digitalWrite(PIN_LED, 1);
  } else {  
       digitalWrite(PIN_LED, 0);
  }

  // Apply ema filter here  
//  dist_ema = dist_raw;
   dist_ema = _EMA_ALPHA * dist_raw + (1-_EMA_ALPHA) * dist_ema;
   
  // adjust servo position according to the USS read value
  // add your code here!


  if (dist_raw < _TARGET_LOW ){   //거리 18cm이하
    myservo.writeMicroseconds(_DUTY_MIN);
  
  }else if(dist_raw > _TARGET_HIGH){ //36 이상
    myservo.writeMicroseconds(_DUTY_MAX);
  }else{
     int angle = map(dist_raw, _TARGET_LOW, _TARGET_HIGH, _DUTY_MIN, _DUTY_MAX);
    myservo.writeMicroseconds(angle);
  }

//analogWrite(PIN_LED, -(255/100)*(distance-100));

  // output the distance to the serial port
  Serial.print("Min:");    Serial.print(_DIST_MIN);
  Serial.print(",dist:");  Serial.print(dist_raw);
  Serial.print(",ema:");   Serial.print(dist_ema);
  Serial.print(",Servo:"); Serial.print(myservo.read());  
  Serial.print(",Max:");   Serial.print(_DIST_MAX);
  Serial.println("");
 
 
  
  dist_prev = dist_raw;
   // update last sampling time
  last_sampling_time += INTERVAL;
}

// get a distance reading from USS. return value is in millimeter.
float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}
