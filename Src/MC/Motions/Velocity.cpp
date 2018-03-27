/*
 * Velocity.cpp
 *
 *  Created on: 27 ����. 2018 �.
 *      Author: Sales
 */

#include "MC/General.h"
#include "MC/Position.hpp"
#include <MC/Motions/Velocity.hpp>
#include "MC/MotionController.hpp"
#include "MC/Settings/Settings.hpp"

Velocity::Velocity(float initialValue){
	this->stepSize = GetStep4Velocity(initialValue);
}

Velocity::~Velocity(){
}

void Velocity::Set(float newValue){
	this->SetStepSize(newValue);
}

float Velocity::Get(){
	return GetVelocity4Step(this->stepSize);
}

void Velocity::SetStepSize(float newValue){
	this->stepSize = GetStep4Velocity(newValue);
}

uint32_t Velocity::GetStepSize(){
	return this->stepSize;
}

float Velocity::GetVelocity4Step(uint32_t stepSize){
	return  60.0*stepSize
			*GetOneBitLengthMM()
			*motionController->GetTimerFrequency();
}

uint32_t Velocity::GetStep4Velocity(float velocity){
	float mmInTick = velocity/60.0/motionController->GetTimerFrequency();
	return (uint32_t)(mmInTick/GetOneBitLengthMM());
}

float Velocity::GetMinVelocity(){
	return  GetVelocity4Step(1L);
}

float Velocity::GetMaxVelocity(){
	return  60.0
			*motionController->GetTimerFrequency()
			*(N_OF_TOOTH*TOOTH_STEP)/STEP_PER_ROTATION/2;
}

FreeRunVelocity::FreeRunVelocity():
Velocity(0.0),
_value(&(motionController->settings.freeRunVelocity)){
	this->SetStepSize(this->_value->Get());
}

FreeRunVelocity::~FreeRunVelocity() {
}

void FreeRunVelocity::Set(float newVelocity) {
	this->_value->Set(newVelocity);
	this->SetStepSize(this->Get());
};

float FreeRunVelocity::Get() {
	return this->_value->Get();
};

RelativeVelocity::RelativeVelocity(FloatSetting* currentSetting, FreeRunVelocity* frv):
Velocity(0.0),
_freeRunVelocity(frv),
_ratio(currentSetting){
	this->Set(this->_freeRunVelocity->Get()*this->_ratio->Get());
}

RelativeVelocity::~RelativeVelocity(){
}

void RelativeVelocity::Set(float newVelocity){
	float tmpRatio = newVelocity/this->_freeRunVelocity->Get();
	if(tmpRatio < 0.0) tmpRatio = 0.0;
	if(tmpRatio > 1.0) tmpRatio = 1.0;
	this->_ratio->Set(tmpRatio);
	this->SetStepSize(this->Get());
}

float RelativeVelocity::Get(){
	return (this->_freeRunVelocity->Get() * this->_ratio->Get());
}

WorkingVelocity::WorkingVelocity(FreeRunVelocity* frv):
RelativeVelocity(&(motionController->settings.workingVelocity), frv),
_autoLimitStepSize(0),
_autoLimitRatio(&(motionController->settings.autoLimitRatio)) {
	SetAutoLimitStepSize();
}

void WorkingVelocity::SetAutoLimitStepSize(){
	this->_autoLimitStepSize = this->GetStepSize()*this->_autoLimitRatio->Get();
}

void WorkingVelocity::SetLimit(float newLimitRatio){
	this->_autoLimitRatio->Set(newLimitRatio);
	this->SetAutoLimitStepSize();
}

float WorkingVelocity::GetLimit(){
	return 	this->_autoLimitRatio->Get();
}

uint32_t WorkingVelocity::GetAutoLimitStepSize(){
	return this->_autoLimitStepSize;
}
