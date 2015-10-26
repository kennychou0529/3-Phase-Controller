/* 
 * File:   main.cpp
 * Author: btacklind
 *
 * Created on Nov 28, 2014, 2:02 PM
 */

#include <util/delay.h>
#include <avr/wdt.h>
#include "Board.h"

#include "ThreePhaseController.h"
#include "MLX90363.h"
#include "TwillBotInterface.h"
#include "MotorControl.h"
#include "Debug.h"
#include "Clock.h"

/**
 * All the init functions should go in here.
 * gcc will automatically call it for us because of the constructor attribute.
 */
void init() __attribute__((constructor));

void init() {
 wdt_reset();
 wdt_disable();
 Debug::init();
 
 ::Clock::init();
 
 TwillBotInterface::init();
 
 ThreePhaseController::init();
 
 MotorControl::init();
 
 // End of init
 Board::LED.off();
}

/*
 * 
 */
void main() {
 sei();
 
 ThreePhaseDriver::setAmplitude(10);
 MLX90363::prepareGET1Message(MLX90363::MessageType::Alpha);
 
// MLX90363::startTransmitting();
// while (MLX90363::isTransmitting());

 while (1) {
  _delay_us(20);
  ThreePhaseDriver::advance();
 }
 
 
 ThreePhaseDriver::advanceTo(40);
 MotorControl::setInitialPosition(40);
 
 TwillBotInterface::reserveNextReadBuffer();
 
 Board::LED.on();

 while(1) {
    
    //Do motor stuff
    MotorControl::advance();
    
    if (false /*Timer::getSince(lastTimeSPICheck) > timeBetweenSPIChecks*/){
        MLX90363::startTransmitting();
        while (MLX90363::isTransmitting());
//        lastTimeSPICheck = Timer::getCurTime();

        TwillBotInterface::releaseNextWriteBuffer();
        u2 * const buff = (u2 * const)TwillBotInterface::getOutgoingWriteBuffer();

        buff[0] = MLX90363::getAlpha();
        buff[1] = MotorControl::getTimer();
        buff[2] = MLX90363::getRoll();
        buff[3] = MLX90363::getErr();
        buff[4] = MotorControl::getStep();
        
    }
    
    if (TwillBotInterface::hasReceivedBlock()){
        TwillBotInterface::reserveNextReadBuffer();
        if (*TwillBotInterface::getIncomingReadBuffer() == 0x33){
            MotorControl::goDistance(1);
            //*TwillBotInterface::getIncomingReadBuffer() = 0;
        }
        else if (*TwillBotInterface::getIncomingReadBuffer() == 0x20){
            MotorControl::goAt(10);
            //*TwillBotInterface::getIncomingReadBuffer() = 0;
        }
        else if (*TwillBotInterface::getIncomingReadBuffer() == 0x21){
            MotorControl::goAt(TwillBotInterface::getIncomingReadBuffer()[1]);
            //*TwillBotInterface::getIncomingReadBuffer() = 0;
            //TwillBotInterface::getIncomingReadBuffer()[1] = 0;
        }
        else if (*TwillBotInterface::getIncomingReadBuffer() == 0x35){
            MotorControl::goDistance(-1);
        }
    }
 }
 
 while(1);
}

