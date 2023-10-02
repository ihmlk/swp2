// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12   // sonar sensor TRIGGER
#define PIN_ECHO 13   // sonar sensor ECHO

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25    // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300.0   // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL) // coefficent to convert duration to distance

unsigned long last_sampling_time;   // unit: msec       


void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);  // sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);   // sonar ECHO
  digitalWrite(PIN_TRIG, LOW);  // turn-off Sonar 
  
  // initialize serial port
  Serial.begin(57600);
}

void loop() {
  float distance;

  //wait untill next sampling time.
  //millis() returns the number of milliseconds since the program started.
  

  if (millis() < (last_sampling_time + INTERVAL))                              
    return;
  distance = USS_measure(PIN_TRIG, PIN_ECHO); // read distance

  if ((distance <= _DIST_MIN) || (distance > _DIST_MAX)){
      analogWrite(PIN_LED, 255);    
        
  }else if (distance > _DIST_MIN && distance < 200){
      analogWrite(PIN_LED, -(255/100)*(distance-100));
          
  }else if(distance == 200){    
      analogWrite(PIN_LED, 0);
                 
  }else if(distance > 200 && distance < _DIST_MAX ){    
      analogWrite(PIN_LED, (255/100)*(distance-300));              
  }


  Serial.print("Min:");        Serial.print(_DIST_MIN);
  Serial.print(",distance:");  Serial.print(distance);
  Serial.print(",Max:");       Serial.print(_DIST_MAX);
  Serial.println("");
  
  //update last smapling time
  last_sampling_time += INTERVAL;
}

  // get a distance reading from USS. return value is in millimeter.
float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE;
}