#include "State_Machine.h"
#include "utility.h"

stateMachine_t stateMachine;

void stateMachineTask(void *argument);
uint32_t startTime;

int stateMachineInit() {
	stateMachine.currentState = eSMInit;
	stateMachine.nextState = eSMInit;
	stateMachine.upAxis = eAxisUndef;
	// Initialise thread
}

void stateMachineTask(void *argument) {
	
	while(1) {
		stateMachine.currentState = stateMachine.nextState;
		startTime = millis();
		
		switch(stateMachine.nextState) {
			case eSMInit:
				stateMachine.nextState = checkSMInit();
				break;
			case eSMIdleOnPad:
				stateMachine.nextState = checkSMIdleOnPad();
				break;
			case eSMLaunch:
				stateMachine.nextState = checkSMLaunch();
				break;
			case eSMBurnout:
				stateMachine.nextState = checkSMBurnout();
				break;
			case eSMApogee:
				stateMachine.nextState = checkSMApogee();
				break;
			case eSMMainDeploy:
				break;
			case eSMLanded:
				break;
			case eSMError:
				SMError();
				break;
			case eSMDisarm:
				break;
		}
	}
}

stateMachineStates_t checkSMInit() {
	taskStates.stateMachineTaskState = eTaskRunning;
	
	if(millis() - startTime > SM_INIT_TIMEOUT_MS) {
		return eSMError;
	}
	
	if(sensors_initialised && SD.sd_good && ekf_init) {
		return eSMIdleOnPad;
	}
	else {
		return eSMInit;
	}
}

stateMachineStates_t checkSMIdleOnPad() {
	float angleFromVertical = getAngleFromVertical();
	vec3_t accVec = getValidAccelData();
	if(!accVec.isValid) return eSMIdleOnPad;
	
	if (detectLaunchAccel(accVec.x, accVec.y, accVec.z, angleFromVertical, millis()))) {
		system_state.launchTimeMs = millis();
		system_state.launchAltitude = ekf.state_vec_data[6];
		return eSMLaunch;
	}
	return eSMIdleOnPad;
}

stateMachineStates_t checkSMLaunch() {
	// TODO: Enable data streaming
	
	vec3_t accelVec = getValidAccData();
	if(!accVec.isValid) return eSMLaunch;
	
	if(getUpAxis()) {
		// Check for burnout
		if(detectBurnoutAccel(accelVec.x, accelVec.y, accelVec.z, millis())) {
			system_state.burnoutTimeMs = millis();
			system_state.burnoutAltitudeAGL = getAltitudeAGL();
			return eSMBurnout;
		}
	}
	
	// Check for apogee
	float verticalVelocity = ekf.state_vec_data[9];
	if(checkApogee(verticalVelocity, accVec.x, accVec.y, accVec.z)) {
		return eSMApogee;
	}
	
	return eSMLaunch;
}

stateMachineStates_t checkSMBurnout() {
	// Check for apogee
	float verticalVelocity = ekf.state_vec_data[9];
	if(checkApogee(verticalVelocity, accVec.x, accVec.y, accVec.z)) {
		return eSMApogee;
	}
	return eSMBurnout;
}

void SMError() {
	Non_Blocking_Error_Handler(taskStates.stateMachineTaskState);
}

float getAngleFromVertical() {
	
}

vec3_t accVecSum = { 0 };
int upAxisLoopCounter = 0;

bool getUpAxis() {
	if(stateMachine.upAxis != eAxisUndef) {
		accVecSum = { 0 };
		upAxisLoopCounter = 0;
		return true;
	}
	
	vec3_t accVec = getValidAccData();
	if(!accVec.isValid) {
		return false;
	}
	
	accVecSum.x += accVec.x;
	accVecSum.y += accVec.y;
	accVecSum.z += accVec.z;
	upAxisLoopCounter++;
	
	if(upAxisLoopCounter >= UP_AXIS_LOOP_COUNT) {
		accVec.x = accVecSum.x/upAxisLoopCounter;
		accVec.y = accVecSum.y/upAxisLoopCounter;
		accVec.z = accVecSum.z/upAxisLoopCounter;
		
		// Determine the axis with largest absolute value
		float maxAcceleration = max(max(fabs(accVec.x), fabs(accVec.y)), fabs(accelVec.z));
		
		switch(maxAcceleration) {
			case accVec.x:
				stateMachine.upAxis = ePosX;
				break;
			case -accVec.x:
				stateMachine.upAxis = eNegX;
				break;
			case accVec.y:
				stateMachine.upAxis = ePosY;
				break;
			case -accVec.y;
				stateMachine.upAxis = eNegX;
				break;
			case accVec.z:
				stateMachine.upAxis = ePosZ;
				break;
			case -accVec.z
				stateMachine.upAxis = eNegZ;
				break;
			default:
				break;
		}
		accVecSum = { 0 };
		upAxisLoopCounter = 0;
		return true;
	}
	else return false;
}

bool checkApogee(float verticalVelocity, float ax, float ay, float az) {
	if(detectApogeeEKF(verticalVelocity, ax, ay, az)) {
		system_state.apogeeTimeMs = millis();
		system_state.apogeeAltitudeAGL = getAltitudeAGL();
		return true;
	}
	return false;
}

bool detectApogeeEKF(float verticalVelocity, float ax, float ay, float az) {
	float accelMagnitude = magnitude(ax, ay, az);
	return accelMagnitude < ACCEL_APOGEE_THRESHOLD && verticalVelocity < VELOCITY_APOGEE_THRESHOLD;
}