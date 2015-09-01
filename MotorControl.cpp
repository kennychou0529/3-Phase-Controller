/* 
 * File:   MotorControl.cpp
 * Author: btacklind
 * 
 * Created on March 28, 2015, 4:30 PM
 */

#include "MotorControl.h"
#include "ThreePhaseDriver.h"

void MotorControl::init(){
  
}

u2 MotorControl::lastStep;
u2 MotorControl::timeLastStep;

void MotorControl::setInitialPosition(u4 pos){
    
    lastStep = getStepFromLocation(pos);
    timeLastStep = getTimer();
}

u2 MotorControl::getStepFromLocation(u4 magData){
    return magData;
}

void MotorControl::go(s1 force) {
    
}

void MotorControl::goAt(s2 speed) {
    
}


void MotorControl::advance(){
    u2 currTime = getTimer();
    u2 timeDiff = Timer::getSince(timeLastStep);
    //if (currTime > timeLastStep)
    //    timeDiff = currTime - timeLastStep;
    //else
    //    timeDiff = currTime + (0xFFFF - timeLastStep);
    
    s2 stepSize = -(timeDiff / 40);
    
    if (stepSize != 0){
        s2 nextStep = lastStep + stepSize;
    
        if (nextStep > 0x2FF) nextStep -= 0x2FF;
        else if (nextStep < 0) nextStep += 0x2FF;
    
        timeLastStep = currTime;
        lastStep = nextStep;
        ThreePhaseDriver::advanceTo(nextStep);
    }
}



