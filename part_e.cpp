
#include "Aria.h"
#include <iostream>
#include <cmath>
using namespace std;

ArRobot robot;

int main(int argc, char **argv)
{
	ArSonarDevice sonar;

	robot.addRangeDevice(&sonar);

	Aria::init();
	
	ArSimpleConnector connector(&argc,argv);

	if (!connector.connectRobot(&robot)){
		printf("Could not connect to robot... exiting\n");
		Aria::shutdown();
		Aria::exit(1);
	}

	robot.comInt(ArCommands::ENABLE, 1);

	robot.runAsync(false);

  // Used to perform actions when keyboard keys are pressed
  ArKeyHandler keyHandler;
  Aria::setKeyHandler(&keyHandler);

  // ArRobot contains an exit action for the Escape key. It also 
  // stores a pointer to the keyhandler so that other parts of the program can
  // use the same keyhandler.
  robot.attachKeyHandler(&keyHandler);
  printf("You may press escape to exit\n");
	
	double x, y, theta;
	cin >> x >> y >> theta;
	x *= 1000;
	y *= 1000;
	ArPose objPos(x, y, theta);
	//printf("obj = %f %f %f\n", x, y, theta);
	double rotateAngle = atan(y / x);
	if(x < 0){
	    rotateAngle += M_PI;
	}

	//printf("rotateAngle = %f\n", rotateAngle);
	ArPose initPos(5090, 3580, -146.03);
	robot.moveTo(initPos);
	//printf("init = %f %f %f\n", robot.getX(), robot.getY(), robot.getTh());
	robot.setHeading(180 * rotateAngle / M_PI);
	while(true){
	    if(robot.isHeadingDone()){
	    	//printf("init = %f %f %f\n", robot.getX(), robot.getY(), robot.getTh());
		break;
	    }
	}
	// TODO: control the robot

	// Start of controling

	// 1. Lock the robot
	robot.lock();

	// 2. Write your control code here, 
	//    e.g. robot.setVel(150);  

	double distance = sqrt(pow(x - 5090, 2) + pow(y - 3580, 2));
	double vel = min(distance / 2, robot.getAbsoluteMaxTransVel());
	robot.resetTripOdometer();
	robot.setVel(vel);
	// 3. Unlock the robot
	robot.unlock();

	// 4. Sleep a while and let the robot move
	while(true){
		double traveledDis = robot.getTripOdometerDistance();
		if(traveledDis >= distance / 2 && traveledDis <= distance * 3/4){
			robot.setVel(distance / 4);
		}
		else if(traveledDis >= distance / 4 && traveledDis <= distance * 9/10){
			robot.setVel(distance / 10);
		}
		else if(traveledDis > distance * 9/10 && traveledDis <= distance - 150){
			robot.setVel(10);		
		}
		else if(traveledDis >= distance - 150){
			robot.setVel(0);
			//printf("final : %f %f %f\n", robot.getX(), robot.getY(), robot.getTh());
			//printf("obj = %f %f %f\n", x, y, theta);
			break;	
		}
		//printf("%f %f %f\n", robot.getX(), robot.getY(), robot.getTh());
	}
	//printf("final : %f %f %f\n", robot.getX(), robot.getY(), robot.getTh());
	if(robot.getVel() > 0){
		robot.setVel(0);	
	}
	robot.setHeading(180 * theta / M_PI);
	while(true){
	    if(robot.isHeadingDone()){
	    	//printf("final = %f %f %f\n", robot.getX(), robot.getY(), robot.getTh());
		break;
	    }
	}
	while(true){
	}
	// End of controling


	Aria::shutdown();

	Aria::exit(0);
}
