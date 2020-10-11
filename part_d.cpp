
#include "Aria.h"
ArRobot robot;
bool checkSonarFront(void){
    int sonarNum = robot.getNumSonar();
    for(int i = 0; i < sonarNum; i++){
    	ArSensorReading* sonarReading = robot.getSonarReading(i);
	unsigned int range = sonarReading->getRange();
	double theta = sonarReading->getSensorTh();
	if(theta <= 45 && theta >= -45 && range <= 525){
		return false;
	}
    }
    return true;
}

bool checkSonarBack(void){
    int sonarNum = robot.getNumSonar();
    for(int i = 0; i < sonarNum; i++){
    	ArSensorReading* sonarReading = robot.getSonarReading(i);
	unsigned int range = sonarReading->getRange();
	double theta = sonarReading->getSensorTh();
	if((theta <= -135 || theta >= 135) && range <= 525){
		return false;
	}
    }
    return true;
}

void forward(void)
{
    if(checkSonarFront()){
        robot.setVel(150);
    }
    else{
        double vel;
        if(vel = robot.getVel() > 0){
	    if(vel >= 30){
	        robot.setVel(vel - 30);
	    }
	    else{
	        robot.setVel(0);	
	    }
        }
    }
}

void backward(void)
{
    if(checkSonarBack()){
        robot.setVel(-150);
    }
    else{
	double vel;
        if(vel = robot.getVel() < 0){
	    if(vel <= -30){
	        robot.setVel(vel + 30);
	    }
	    else{
	        robot.setVel(0);	
	    }
        }
    }
}

void upward(void)
{
  robot.setRotVel(30);
}

void downward(void)
{
  robot.setRotVel(-30);
}

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

	// TODO: control the robot

	// Start of controling

	// 1. Lock the robot
	robot.lock();

	// 2. Write your control code here, 
	//    e.g. robot.setVel(150);  
	ArGlobalFunctor forwardCB(&forward);
 	ArGlobalFunctor backwardCB(&backward);
	ArGlobalFunctor upwardCB(&upward);
	ArGlobalFunctor downwardCB(&downward);
 	keyHandler.addKeyHandler(ArKeyHandler::RIGHT, &forwardCB);
 	keyHandler.addKeyHandler('d', &forwardCB);
 	keyHandler.addKeyHandler(ArKeyHandler::LEFT, &backwardCB);
  	keyHandler.addKeyHandler('a', &backwardCB);
	keyHandler.addKeyHandler(ArKeyHandler::UP, &upwardCB);
 	keyHandler.addKeyHandler('w', &upwardCB);
	keyHandler.addKeyHandler(ArKeyHandler::DOWN, &downwardCB);
 	keyHandler.addKeyHandler('s', &downwardCB);
	
	// 3. Unlock the robot
	robot.unlock();

	// 4. Sleep a while and let the robot move
	while(true){
		//printf("%f %f %f\n", robot.getX(), robot.getY(), robot.getTh());
		ArUtil::sleep(300);
		robot.setVel(0);
		robot.setRotVel(0);
	}

	// End of controling


	Aria::shutdown();

	Aria::exit(0);
}
