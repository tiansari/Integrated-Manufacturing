#include <avr/io.h>
#include <Wire.h>
#include <EVShield.h>
#include<EVs_NXTTouch.h>

EVShield evshield (0x34, 0x36);
EVs_NXTTouch touch1, touch2;

float EN_Mtr2_1,EN_Mtr1_1; // Position of Motors at Location A 
float EN_Mtr2_2,EN_Mtr1_2; // Position of Motors at Location B

// This function is to setup motors and Sensors
void setup() {  
  Serial.begin(115200);
  Serial.println("Hello... We are Uranus Inspectors");
  evshield.init(SH_HardwareI2C);
  
  touch1.init(&evshield,SH_BAS1); // Acquires Pick up Location
  touch2.init(&evshield,SH_BAS2); // Acquires Drop off Location
  
  //Resetting Motors on Bank A and B   
     //evshield.bank_a.motorRunDegrees(SH_Motor_2, SH_Direction_Forward, 0,0, SH_Completion_Wait_For, SH_Next_Action_Float);
     //evshield.bank_a.motorRunDegrees(SH_Motor_1, SH_Direction_Forward, 0,0, SH_Completion_Wait_For, SH_Next_Action_Float);
     evshield.bank_a.motorReset();
     evshield.bank_b.motorReset();
     evshield.bank_a.motorResetEncoder(SH_Motor_1);
     evshield.bank_b.motorResetEncoder(SH_Motor_1);
     evshield.bank_b.motorResetEncoder(SH_Motor_2);

  //Prints Encoder Position of BankA Motor2
     Serial.println(evshield.bank_a.motorGetEncoderPosition(SH_Motor_2));
     Serial.println("Press Go Button On EVShield to Continue...");
     evshield.waitForButtonPress(BTN_GO);
     Serial.println("Go Button Pressed... Resetting Motors...");
     
     Serial.println("Robot ready to be Taught");
     Serial.println("Teach the Robot by pressing Touch Sensor. Touch Sensor 1 to define Location A and Touch Sensor 2 to define Location B");
}

// This Function acquires pick up location
void GetCoordinatesA() {
     EN_Mtr2_1=evshield.bank_b.motorGetEncoderPosition(SH_Motor_2);
     EN_Mtr1_1=evshield.bank_a.motorGetEncoderPosition(SH_Motor_1);
     Serial.println("Calculating....");
     delay(2000);
     Serial.println("Rotation for Motor1 and 2 is");
     Serial.println(EN_Mtr1_1);
     Serial.println(EN_Mtr2_1);
     Serial.println("Location A acquired!");
//     if (EN_Mtr2_1<0)
//     evshield.bank_b.motorRunDegrees(SH_Motor_2, SH_Direction_Forward,SH_Speed_Slow,EN_Mtr2_1, SH_Completion_Wait_For, SH_Next_Action_Float);
//     else{
//      evshield.bank_b.motorRunDegrees(SH_Motor_2, SH_Direction_Reverse,SH_Speed_Slow,EN_Mtr2_1, SH_Completion_Wait_For, SH_Next_Action_Float);
//     }        
//     if (EN_Mtr1_1<0)
//     evshield.bank_a.motorRunDegrees(SH_Motor_1, SH_Direction_Forward,SH_Speed_Slow,EN_Mtr1_1, SH_Completion_Wait_For, SH_Next_Action_Float);
//     else{
//      evshield.bank_a.motorRunDegrees(SH_Motor_1, SH_Direction_Reverse,SH_Speed_Slow,EN_Mtr1_1, SH_Completion_Wait_For, SH_Next_Action_Float);
//     }
     evshield.bank_a.motorReset();
     Serial.println("Move to drop off location");

     // Motor and Encoder reset to acquire Location B wrt Location A
     evshield.bank_a.motorReset();
     evshield.bank_b.motorReset();
     evshield.bank_a.motorResetEncoder(SH_Motor_1);
     evshield.bank_b.motorResetEncoder(SH_Motor_1);
     evshield.bank_b.motorResetEncoder(SH_Motor_2);
}

//This Function acquires drop off location
void GetCoordinatesB() {
     EN_Mtr2_2=evshield.bank_b.motorGetEncoderPosition(SH_Motor_2);
     EN_Mtr1_2=evshield.bank_a.motorGetEncoderPosition(SH_Motor_1);
     Serial.println("Calculating....");
     delay(2000);
     Serial.println("Rotation for Motor1 and 2 is");
     Serial.println(EN_Mtr1_2);
     Serial.println(EN_Mtr2_2);
     Serial.println("Location B acquired!");

     //evshield.bank_a.motorRunDegrees(SH_Motor_2, SH_Direction_Reverse,SH_Speed_Slow, 0, SH_Completion_Wait_For, SH_Next_Action_Float);
}

//This function is called to execute taught sequence
void execute (){
     Serial.println("Executing...");

     //Open Claw
     evshield.bank_b.motorRunDegrees(SH_Motor_1, SH_Direction_Reverse,SH_Speed_Slow,90, SH_Completion_Wait_For, SH_Next_Action_BrakeHold);
     Serial.println("Claw opened"); 
        
     if (EN_Mtr1_2<0)
     evshield.bank_a.motorRunDegrees(SH_Motor_1, SH_Direction_Forward,SH_Speed_Slow,EN_Mtr1_2, SH_Completion_Wait_For, SH_Next_Action_Float);
     else{
      evshield.bank_a.motorRunDegrees(SH_Motor_1, SH_Direction_Reverse,SH_Speed_Slow,EN_Mtr1_2, SH_Completion_Wait_For, SH_Next_Action_Float);
     }
     if (EN_Mtr2_2<0)
     evshield.bank_b.motorRunDegrees(SH_Motor_2, SH_Direction_Forward,SH_Speed_Slow,EN_Mtr2_2, SH_Completion_Wait_For, SH_Next_Action_Float);
     else{
      evshield.bank_b.motorRunDegrees(SH_Motor_2, SH_Direction_Reverse,SH_Speed_Slow,EN_Mtr2_2, SH_Completion_Wait_For, SH_Next_Action_Float);
     }     
     //evshield.bank_a.motorReset();
     delay(2000);
     //Close Claw
     evshield.bank_b.motorRunDegrees(SH_Motor_1, SH_Direction_Forward,SH_Speed_Slow,90, SH_Completion_Wait_For, SH_Next_Action_BrakeHold);
     Serial.println("Object Picked...");
     delay(2000);
      
     Serial.println("Dropping now...");
        
     if (EN_Mtr1_2<0)
     evshield.bank_a.motorRunDegrees(SH_Motor_1, SH_Direction_Reverse,SH_Speed_Slow,EN_Mtr1_2, SH_Completion_Wait_For, SH_Next_Action_Float);
     else{
      evshield.bank_a.motorRunDegrees(SH_Motor_1, SH_Direction_Forward,SH_Speed_Slow,EN_Mtr1_2, SH_Completion_Wait_For, SH_Next_Action_Float);
     }
     if (EN_Mtr2_2<0)
     evshield.bank_b.motorRunDegrees(SH_Motor_2, SH_Direction_Reverse,SH_Speed_Slow,EN_Mtr2_2, SH_Completion_Wait_For, SH_Next_Action_Float);
     else{
      evshield.bank_b.motorRunDegrees(SH_Motor_2, SH_Direction_Forward,SH_Speed_Slow,EN_Mtr2_2, SH_Completion_Wait_For, SH_Next_Action_Float);
     }
     
     //Open Claw
     evshield.bank_b.motorRunDegrees(SH_Motor_1, SH_Direction_Reverse,SH_Speed_Slow,90, SH_Completion_Wait_For, SH_Next_Action_BrakeHold);
     Serial.println("Object dropped"); 
     delay(2000);
     //Close Claw
     //evshield.bank_b.motorRunDegrees(SH_Motor_1, SH_Direction_Reverse,SH_Speed_Slow,90, SH_Completion_Wait_For, SH_Next_Action_Float);
     Serial.println("Execution Complete!");     
}

//Main Function
void loop() {
     
    if (touch1.isPressed()==true) GetCoordinatesA();
    if (touch2.isPressed()==true) GetCoordinatesB();

    //Execution of Taught Locations
    if (evshield.getButtonState(BTN_RIGHT)) execute();
   
}
  

