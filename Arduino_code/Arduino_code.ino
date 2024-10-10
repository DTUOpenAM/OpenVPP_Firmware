
// Include the AccelStepper Library
#include <AccelStepper.h>
#include <elapsedMillis.h>

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data
int counter = 4;
boolean newData = false;
char receivedChar;
String subset_1;
String subset_2;
String subset_3;
float z = 0;
float f = 0;
long currentPos = 0;

//instrucciones
String initi = "G21\n";
String M999 = "M999"; 
String G28 = "G28 Z";
String G1Z = "G1";
String M114 = "M114";
String M18 = "M18";

//Variables movement
int sp = 0;
int spindle_pitch_microns=4000; 
int steps_per_revolution=4000;

//Variables test
bool test = false;

//Variables control
bool recibeNum = false;
int endStopRead = 1;

//Endstop sensor
const int endStopPort = 5;

// Motor Connections (constant current, step/direction bipolar motor driver)
const int fiveV = 2;
const int dirPin = 3;
const int stepPin = 4;
AccelStepper myStepper(AccelStepper::DRIVER, stepPin, dirPin); 
elapsedMillis printTime;
bool com_control = false;
void(* resetFunc) (void) = 0;

int y = 0; 
int controla = 0;

void setup() {
  Serial.begin(115200);
  pinMode(fiveV,OUTPUT); //5V on digital port
  digitalWrite(fiveV, HIGH);
  pinMode(endStopPort, INPUT);
  currentPos = 0;
  myStepper.setCurrentPosition(0);
  myStepper.setMaxSpeed(1000.0);
  myStepper.setAcceleration(1000.0);

}

void loop() {
    if(!com_control){
      stablish_com();
    }
    recvWithEndMarker();
    analyzeData();
}

void stablish_com(){
    while(!com_control){
    delay(2000);
    Serial.println("echo:  M301");
    recvWithEndMarker();
    if (compArrChar(initi,receivedChars)){
          com_control = true;
          Serial.println("comunicado");
          char endMarker = '\n';
          Serial.println(endMarker);
        }
  }
}

void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    while (Serial.available() > 0) {
        rc = Serial.read();
        //Serial.println(rc);
        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            //Serial.println(receivedChars);
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            counter = ndx;
            ndx = 0;
            newData = true;
        }
    }
}

void analyzeData() {
  if (newData == true) {
    subset_1 = receivedChars[0];
    subset_2 = receivedChars[1];
    subset_3 = receivedChars[2];

    if (compArrChar(M999, receivedChars)) {
      Serial.println("Resetting in 5 seconds...");
      delay(5000);
      resetFunc();
    } else if (compArrChar(G28, receivedChars)) {
      g28();
    } else if (receivedChars[0] == 'G' && receivedChars[1] == '1' && receivedChars[3] == 'Z') {
      float pos = atof(&receivedChars[4]); // convert input to float
      long steps = pos * 1000; // calculate steps based on desired position.
      moveMotor(steps); // move motor to the desired position
      newData = false; // reset newData flag
    } else if (compArrChar(M114, receivedChars)) {
      Serial.print("X:");
      Serial.print("0.0");
      Serial.print(myStepper.currentPosition() / 1000.000);
      Serial.print(" Y:0.0");
      Serial.print(" Z:");
      Serial.print((myStepper.currentPosition() / 1000.000));
      Serial.print(" E:");
      Serial.print("0.0");
      Serial.println();
    } else if (compArrChar(M18, receivedChars)) {
      //resetFunc();
      m18();
    } else if (receivedChars[0] == 'd' && receivedChars[1] == 'e' && receivedChars[2] == 'l') {
      // Extract the delay value from the command
      if (strlen(receivedChars) > 4) {
        unsigned long delayMillis = atol(&receivedChars[6]);
        Serial.print("Applying delay of ");
        Serial.print(delayMillis);
        Serial.println(" ms");
        delay(delayMillis); // Apply the delay
      } else {
        Serial.println("Invalid delay command");
      }
    }
    newData = false;
  }
}

bool compArrChar(String  a, String  b){
  int i = 0;
  bool control = true;
  while(i<counter && control){
    if(a[i]==b[i]){
      i++;
    }else{
      control = false;
    }
  }
  return control;
}

void moveMotor(float mm) {
  bool endstopcondition = false;
  float targetPos_mm = currentPos * spindle_pitch_microns / steps_per_revolution + mm;
  long targetPos_steps = targetPos_mm * steps_per_revolution / spindle_pitch_microns;

  if (targetPos_steps >= 0) {
    myStepper.moveTo(targetPos_steps);
    while (myStepper.distanceToGo() != 0) {
      myStepper.run();
      if (digitalRead(endStopPort) == LOW) {
        Serial.println("Endstop hit");
        break;
      }
    }
    currentPos = myStepper.currentPosition();
    Serial.println("Position reached");
    Serial.println("ok");
  } else {
    Serial.println("Can't move above endstop");
    Serial.println("ok");
  }
}

void g28(){
  int pos = -500;
  bool endStop = false;
  myStepper.setMaxSpeed(3000.0);
  myStepper.setAcceleration(1000.0);
  //Serial.println("Homecoming");
  while(!endStop) {
    myStepper.move(-1000);
    myStepper.run();
   // Serial.println( );
    pos = pos-500;
    if(digitalRead(endStopPort) == LOW) {
      endStop = true;
    }
  }
  while(endStop) {
    myStepper.move(100);
    myStepper.run();
   // Serial.println(myStepper.currentPosition());
    pos = pos-500;
    if(digitalRead(endStopPort) == HIGH) {
      currentPos = 0;
      myStepper.setCurrentPosition(0);
      endStop = false;
    }
  }
  Serial.println("ok");
  myStepper.setMaxSpeed(5000.0);
  myStepper.setAcceleration(5000.0);
  //moveDown(); // call moveDown function to move down
 // Serial.println("No way home");
}

void m18(){
  myStepper.stop();
  Serial.println("Stopping");
  Serial.println("ok");
}

void applyDelay(unsigned long delayMillis) {
  unsigned long startTime = millis();
  while (millis() - startTime < delayMillis) {
    // Wait for the specified delay time
  }
  Serial.println("ok");
}
