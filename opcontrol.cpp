#include "main.h"
#include "okapi/api.hpp"
using namespace okapi;

//assign motors to groups
auto leftMotorGroup = MotorGroup{1,2};
auto rightMotorGroup = MotorGroup{9,10};
Motor armMotor = 8_rmtr;

const double liftkP = 0.001;
const double liftkI = 0.0001;
const double liftkD = 0.0001;
// Chassis Controller - lets us drive the robot around with open- or closed-loop control
auto drive = ChassisControllerFactory::create(
  leftMotorGroup, rightMotorGroup,
  AbstractMotor::gearset::green,
  {4_in, 11.5_in}
);

auto lift = AsyncControllerFactory::posPID(armMotor, liftkP, liftkI, liftkD);

void opcontrol() {
  // Joystick to read analog values for tank or arcade control
  // Master controller by default
  Controller masterController;
  // Arm related objects
  ADIButton armLimitSwitch('H');
  ControllerButton armUpButton(ControllerDigital::A);
  ControllerButton armDownButton(ControllerDigital::B);


  // Button to run our sample autonomous routine
  ControllerButton runAutoButton(ControllerDigital::X);

  while (true) {
    // Tank drive with left and right sticks
    drive.tank(masterController.getAnalog(ControllerAnalog::leftY),
               masterController.getAnalog(ControllerAnalog::rightY));
    /*for(int n = 0; n < 5; n++) {
      printf("%d\n", n);
      if(n==1){
        drive.moveDistance(10_in); //drive 10 in. forward
      }
      else if(n==2){
        drive.moveDistance(100); //drive 100 motor deg forward
      }
      else if(n==3){
        drive.turnAngle(90); //rotate 90 deg
      }
    }
    */
/*
    //arm code
    // Don't power the arm if it is all the way down
    if (armLimitSwitch.isPressed()) {
      armMotor.moveVoltage(0);
    } else {
      // Else, the arm isn't all the way down
      if (armUpButton.isPressed()) {
        armMotor.moveVoltage(12000);
      } else if (armDownButton.isPressed()) {
        armMotor.moveVoltage(-12000);
      } else {
        armMotor.moveVoltage(0);
      }
    }
*/
    // Run the test autonomous routine if we press the button
    if (runAutoButton.changedToPressed()) {
      // Drive the robot in a square pattern using closed-loop control
      for (int i = 0; i < 4; i++) {
        drive.moveDistance(12_in); // Drive forward 12 inches
        drive.turnAngle(90_deg);   // Turn in place 90 degrees
      }
    }

    // Wait and give up the time we don't need to other tasks.
    // Additionally, joystick values, motor telemetry, etc. all updates every 10 ms.
    pros::delay(10); //should be ten usually
  }
}
