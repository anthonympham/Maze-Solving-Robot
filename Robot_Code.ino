#include <QTRSensors.h>

// create an object for your type of sensor (RC or Analog)
// in this example we have three sensors on analog inputs 0 - 2, a.k.a. digital pins 14 - 16
QTRSensorsRC qtr((char[]) {A5, A4, A3, A2, A1, A0 }, 4);

int motorSpeed = 50;
int turnSpeed = 100;
int counter = 0;
int threshold = 750;
int increment[7];
int average[6];

unsigned int sensors[6];
//right wheel
int a = 3;
int b = 4;
int c = 5;

//left wheel
int e = 11;
int f = 10;
int g = 9;

void forward() {
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  analogWrite(a, motorSpeed);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
  analogWrite(e, motorSpeed);
}

void backward() {
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  analogWrite(a, turnSpeed);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
  analogWrite(e, turnSpeed);
}

void left() {
  forward();
  delay(50);
  while(sensors[2] < threshold && sensors[3] < threshold){
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  analogWrite(a, turnSpeed);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
  analogWrite(e, turnSpeed);
  int position = qtr.readLine(sensors);
  }
}

void right() {
 forward();
  delay(50);
  while(sensors[2] < threshold && sensors[3] < threshold){
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  analogWrite(a, turnSpeed);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
  analogWrite(e, turnSpeed);
  int position = qtr.readLine(sensors);
  }
}


void stop() {
  analogWrite(a, 0);
  analogWrite(e, 0);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // optional: wait for some input from the user, such as  a button press
  // then start calibration phase and move the sensors over both
  // reflectance extremes they will encounter in your application:
  int i;
  for (i = 0; i < 250; i++)  // make the calibration take about 5 seconds
  {
    qtr.calibrate();
    delay(20);
    Serial.println(i);
  }
  Serial.println("Calibration complete!");


  //This variable will provide us the # of sets of data the sensor has read
  increment[0] = average[0] = 0;
  increment[1] = average[1] = 0;
  increment[2] = average[2] = threshold;
  increment[3] = average[3] = threshold;
  increment[4] = average[4] = 0;
  increment[5] = average[5] = 0;
  //increment 6 just records the # of cycles
  increment[6] = 0;


  // optional: signal that the calibration phase is now over and wait for further
  // input from the user, such as a button press
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
}

void loop() {
  //counter is the no. of loops
  counter = counter + 1;
  // put your main code here, to run repeatedly: 
  for (int h = 0; h < 6; h++) {
    increment[h] = increment[h] + sensors[h];
  }

  //for debugging
  /*
  for(int i = 0; i < 4; i++){
  Serial.print(sensors[i]);
  Serial.print(" ");
  }
  Serial.println(" ");
  */
  //Every 3rd loop
  increment[6] = counter % 3;
  /*
  Serial.println(counter);
  Serial.println(increment[4]);
  */
  if (increment[6] == 0) {
    for (int i = 0; i < 6; i++) {
      average[i] = increment[i] / 3;
      increment[i] = threshold;
    }
    if (average[0] > threshold || average[1] > threshold){
      //if actual turn, record. Else just adjust
	  if (average[0] > threshold)
		Serial.print("True Turn");
	  right();
	}
    else if (average[2] < threshold || average[3] < threshold){
		if (average[5] > threshold)
			Serial.print ("True Turn);
		left();
	}
    //Serial.print("                          ");
    /*
    for (int k = 0; k < 4; k++) {
      Serial.print(average[k]);
      Serial.print(" ");
    }
    Serial.println(" ");
    */
  }
  // put your main code here, to run repeatedly:
  // get calibrated sensor values returned in the sensors array, along with the line position
  // position will range from 0 to 2000, with 1000 corresponding to the line over the middle 
  // sensor.
  //sensors is an array
  int position = qtr.readLine(sensors);
  forward();
}


