/*
 * MoveTorchUp.cpp
 *
 *  Created on: 23 ���. 2018 �.
 *      Author: Sales
 */

#include <MC/FixedPositionActions/MoveTorchUp.hpp>
#include "MC/Settings/Settings.hpp"

MoveTorchUp::MoveTorchUp():
WaitForTimeout(&(settings.torchUpTime)),
OutputSignal(TorchUp_Output_GPIO_Port, TorchUp_Output_Pin){
}

bool MoveTorchUp::IterateForward(){
	if(this->WaitForTimeout::IterateForward()){
		this->On();
		return true;
	} else {
		this->Off();
		return false;
	}
}