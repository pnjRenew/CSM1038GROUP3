
//  Example code to control the robot with functions
//  By Ryan Hughes 2019
//  Adapted by Group 3 of CSM1038 in October-December 2021
// ----------------------------------------------------------------------------------------------------------- //

//  This example covers:
//    > Writing functions
//    > Calling functions

// ----------------------------------------------------------------------------------------------------------- //

//Define I/O pins (input/output)
#define motorRight 6                         //  As with the previous example, definitions are set to clear up 
#define motorLeft 5                         //  the main body of the code.

#define directionRightLow   7
#define directionRightHigh  8
#define directionLeftLow    9
#define directionLeftHigh   10

#define avoidDistance 15

//--------------------------------------------------------------------------

// definitions for ultrasonic sensor connections
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 // attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement



//---------------------------------------------------------------------------
// Function: setup
// ----------------------------
//   Allocates pin modes and begins serial communications
//
//   no parameters passed in
//
//   returns: none
//  adapted by Group 3 of CSM1038 Oct-Dec 2021
//---------------------------------------------------------------------------

void setup() {                               // The setup routine begins (running only once).

  pinMode(motorRight, OUTPUT);
  pinMode(motorLeft, OUTPUT);                // The previously defined pins are then set to OUTPUTs to control the speed.
  pinMode(directionRightHigh, OUTPUT);
  pinMode(directionRightLow, OUTPUT);
  pinMode(directionLeftHigh, OUTPUT);
  pinMode(directionLeftLow, OUTPUT);         // And to control the direction.

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT for ultrasonic use
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT for ultrasonic use

  Serial.begin(9600);                        // The Serial communication line is initiated to help check for issues or "debug"


}                                            // End of the setup routine.


//---------------------------------------------------------------------------
// Function: loop
// ----------------------------
//   Moves the robot forwards, then left and then right, with 1 second intervals
//
//   no parameters passed in
//
//   returns: none
//  adapted by Group 3 of CSM1038 Oct-Dec 2021
//---------------------------------------------------------------------------
void loop() {                                // The loop routine beigns (running over and over, continuously).

  moveForward();                             // Here, we are calling the "moveForward function". This tells the Arduino to
                                             // run a set of code that is written below. 
                                             
  delay(50);                               // "delay" is another example of a function, however this is included
                                             // in the Arduino system, so it knows what to do already.
                                             

   // take an initial ultrasonic distance measurement
   // Clears the ultrasonic trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");



  // now in each run of the loop, check ultrasonically for something ahead before looping to drive forward again
  while (distance < avoidDistance) {
    delay(0);   // slow down and stop for 1 second by delaying - or maybe not
    turnOnSpotLeft();
    delay(500);   // how much to turn? keep turning-left-on-spot for 500mS
    Serial.println("Turning left to avoid");
    Serial.println("Distance: ");
    Serial.println(distance);
    distance = findDistance(trigPin, echoPin);    // call function to check distance ultrasonically
  }

}                                            // End of the loop routine.


//---------------------------------------------------------------------------
// Function: moveForward
// ----------------------------
//   Moves the robot forward
//
//   no parameters passed in
//
//   returns: none
//---------------------------------------------------------------------------
void moveForward(){                          // This is the start of the "moveForward" function.
                                             // This function is a "void" as it does not return a value to the loop routine.

  Serial.println("Forward");                 // This will tell you that the "moveForward" function is being called
                                             // in the serial monitor (Tools > Serial monitor).

  digitalWrite(directionRightHigh, HIGH);    
  digitalWrite(directionRightLow, LOW);      // This tells the motors to go forward.
  analogWrite(motorRight, 182);              // This sets the speed. - was 200 - can be altered to try to 'trim' vehicle motion

  digitalWrite(directionLeftHigh, HIGH);
  digitalWrite(directionLeftLow, LOW);
  //analogWrite(motorLeft, 200);               //
  analogWrite(motorLeft, 200);               //
  
}

//---------------------------------------------------------------------------
// Function: moveBackward
// ----------------------------
//   Moves the robot backward
//
//   no parameters passed in
//
//   returns: none
//---------------------------------------------------------------------------
void moveBackward(){                          // This is the start of the "moveForward" function.
                                             // This function is a "void" as it does not return a value to the loop routine.

  Serial.println("Backward");                 // This will tell you that the "moveForward" funtion is being called
                                             // in the serial monitor (Tools > Serial monitor).

  digitalWrite(directionRightHigh, LOW);    
  digitalWrite(directionRightLow, HIGH);      // This tells the motors to go forward.
  analogWrite(motorRight, 200);              // This sets the speed.

  digitalWrite(directionLeftHigh, LOW);
  digitalWrite(directionLeftLow, HIGH);     // opposite high/low on both motors, from moveForward
  analogWrite(motorLeft, 200);               // (Q1) Roughly what voltage will this line output, and to what pin number?
  
}

//---------------------------------------------------------------------------
// Function: moveLeft
// ----------------------------
//   Moves the robot to bear left
//
//   no parameters passed in
//
//   returns: none
//---------------------------------------------------------------------------
void moveLeft(){

  Serial.println("Left");                    // This will tell you that the "moveLeft" funtion is being called.

  digitalWrite(directionRightHigh, HIGH);    // (Q2) What voltage is outputted to both the directionRightHigh and 
  digitalWrite(directionRightLow, LOW);      //      directionRightLow pins? What number are these pins?
  analogWrite(motorRight, 200);

  digitalWrite(directionLeftHigh, HIGH);     // Sets the left motor direction.
  digitalWrite(directionLeftLow, LOW);
  analogWrite(motorLeft, 100);                // Sets the left motor speed.
  
}


//---------------------------------------------------------------------------
// Function: moveRight
// ----------------------------
//   Makes the robot bear right
//
//   no parameters passed in
//
//   returns: none
//---------------------------------------------------------------------------
void moveRight(){

  Serial.println("Right");                   // Prints "Right" to the serial monitor

  digitalWrite(directionRightHigh, HIGH);
  digitalWrite(directionRightLow, LOW);      // (Q3) How would you reverse the motors directions?
  analogWrite(motorRight, 100);               // Sets right motor speed.

  digitalWrite(directionLeftHigh, HIGH);
  digitalWrite(directionLeftLow, LOW);       // Sets left motor direction.
  analogWrite(motorLeft, 200);               // Sets left motor speed.
  
}

//---------------------------------------------------------------------------
// Function: turnOnSpotLeft
// ----------------------------
//   Moves the robot turn on the spot by counter rotating motors
//
//   no parameters passed in
//
//   returns: none
//   Group 3 of CSM1038 - Oct-Dec 2021
//---------------------------------------------------------------------------
void turnOnSpotLeft(){                          // This is the start of the "turnOnSpotLeft" function.
                                             // This function is a "void" as it does not return a value to the loop routine.

  Serial.println("Turning on spot Left");                 // This will tell you that the "turnOnSpotLeft" function is being called
                                             // in the serial monitor (Tools > Serial monitor).

  digitalWrite(directionRightHigh, HIGH);    
  digitalWrite(directionRightLow, LOW);      // This tells the motor to go forward.
  analogWrite(motorRight, 200);              // This sets the speed.

  digitalWrite(directionLeftHigh, LOW);
  digitalWrite(directionLeftLow, HIGH);     // change only this motor direction to turn on spot
  analogWrite(motorLeft, 200);               
  
}

// CHALLENGE: Create your own functions to move the robot in different ways, e.g. Stop, backwards or rotate.
//            When ending a line of code, remember to use a ";".

// ANSWERS: 
//  Q1 -> 5v * (100/255) = 1.96V, outputted to pin number 10
//  Q2 -> HIGH = 5V, LOW = 0V, directionRightHigh = pin 5, directionRightLow = pin 4
//  Q3 -> Make directionRightHigh = 0V (LOW) and directionRightLow = 5V (HIGH)

//---------------------------------------------------------------------------
// Function: findDistance
// ----------------------------
//   Measure the distance via 
//
//  Parameters:
//   int tPin - trigger pin number on Arduino
//   int ePin - echo pin number on Arduino
//
//   returns: distance as an integer
//   Group 3 of CSM1038 - Oct-Dec 2021
//---------------------------------------------------------------------------
int findDistance(int tPin, int ePin)
{
  long duration;
  int distance;
  
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (ultrasonic signal to go out and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  return distance;      // return the distance calculated by Arduino
  
}

//---------------------------------------------------------------------------
// Function: driveInSquare
// ----------------------------
//   Drive in a square, turning left, to end on start point hopefully 
//
//    Parameters: none
//   returns: nothing
//   Group 3 of CSM1038 - Oct-Dec 2021
//---------------------------------------------------------------------------
void driveInSquare()
{
  moveForward();  // go forward
  delay(2000);
  turnOnSpotLeft(); // turn on spot (90°)
  delay(500);   // how much to turn? - turn for 500mS
  moveForward(); // go forwarrd again
  delay(2000);
  turnOnSpotLeft(); // turn on spot (180°)
  delay(500);   // how much to turn? - turn for 500mS
  moveForward(); // go forward 
  delay(2000);
  turnOnSpotLeft(); // turn on spot (270°)
  delay(500);   // how much to turn? - turn for 500mS
  moveForward(); // go forward
  delay(2000);
}
