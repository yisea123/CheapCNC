/*
 * Motion.h
 *
 *  Created on: 26 ���. 2018 �.
 *      Author: Sales
 */

#ifndef MC_MOTIONS_MOTION_HPP_
#define MC_MOTIONS_MOTION_HPP_

#include <stdint.h>
#include "MC/Enums.h"
#include "MC/Action.hpp"

class Motion : public Action {
protected:
	MOTION_PHASE phase;

	int64_t wayLength;
	int64_t wayLengthCurrent;
	int64_t wayLengthAcceleration;
	int64_t wayLengthDeceleration;
	int64_t currentDistanceToTarget;

	int64_t relEndPosX;
	int64_t relEndPosY;
	int64_t relCurrentPosX;
	int64_t relCurrentPosY;
	int64_t startAbsPosX;
	int64_t startAbsPosY;

	int32_t stepSizeCurrent;
	int32_t stepSizeBeforeAcceleration;
	int32_t stepSizeAfterDeceleration;
	int32_t stepSizeConstantVelocity;

public:
	Motion( double _relEndPosX,
		    double _relEndPosY,
		    MOTION_VELOCITY velocity,
		    double startVel,
		    double endVel);
	virtual ~Motion();
	virtual void Init();
	virtual bool IterateForward();
	virtual bool IterateBackward();
protected:
	void CalcWayLength();
	virtual void OnIteration();
};

#endif /* MC_MOTIONS_MOTION_HPP_ */
