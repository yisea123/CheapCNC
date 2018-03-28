/*
 * VelocityProfile.cpp
 *
 *  Created on: 27 ���. 2018 �.
 *      Author: Sales
 */

#include "MC/Motions/VelocityProfile.hpp"
#include "MC/Settings/Settings.hpp"
#include "MC/Motions/Velocity.hpp"
#include "MC/MotionController.hpp"
#include "MC/GPIO/OutputSignal.hpp"
#include "MC/Position.hpp"

VelocityProfile::VelocityProfile():
freeRunVelocity(),
workingVelocity(&freeRunVelocity),
startVelocity(&(motionController->startVelocitySetting), &freeRunVelocity),
adjustmentVelocity(&(motionController->adjustmentVelocitySetting), &freeRunVelocity),
acceleration(),
currentStepSize(startVelocity.GetStepSize()){
}

uint32_t VelocityProfile::GetStepSize(MOTION_VELOCITY t) {
	switch (t){
		case FREE_RUN: return freeRunVelocity.GetStepSize();
		case WORKING: return workingVelocity.GetStepSize();
		case START: return startVelocity.GetStepSize();
		case ADJUSTMENT: return adjustmentVelocity.GetStepSize();
		default: return 0;
	}
}

float VelocityProfile::GetVelocity(MOTION_VELOCITY t){
	switch (t){
		case FREE_RUN: return freeRunVelocity.Get();
		case WORKING: return workingVelocity.Get();
		case START: return startVelocity.Get();
		case ADJUSTMENT: return adjustmentVelocity.Get();
		default: return 0;
	}
}

uint32_t VelocityProfile::GetCurrentStepSize(){
	return this->currentStepSize;
};

void VelocityProfile::SetCurrentStepSize(uint32_t newStepSIze){
	this->currentStepSize = newStepSIze;
	if(this->currentStepSize >= this->workingVelocity.GetAutoLimitStepSize()) THC_AutoOn();
	else THC_AutoOff();
}

float VelocityProfile::GetCurrentVelocity() { // millimeters in minute
	return (float)(60.0
			     *GetDoubleMMFor64bit(this->currentStepSize)
				 * motionController->GetTimerFrequency());
}
