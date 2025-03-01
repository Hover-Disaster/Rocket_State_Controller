

typedef enum {
	eSMInit,
	eSMIdleOnPad,
	eSMLaunch,
	eSMBurnout,
	eSMApogee,
	eSMMainDeploy,
	eSMLanded,
	eSMError,
	eSMDisarmed
} stateMachineStates_t;

typedef enum {
	ePosX,
	eNegX,
	ePosY,
	eNegY,
	ePosZ,
	eNegZ,
	eAxisUndef,
} axisDefinition_t;

typedef struct {
	stateMachineStates_t currentState;
	stateMachineStates_t nextState;
	axisDefinition_t upAxis;
} stateMachine_t;

#define SM_INIT_TIMEOUT_MS 5000
#define PITCH_OVER_ANGLE_THRESHOLD 30 // deg
#define UP_AXIS_LOOP_COUNT 20

extern stateMachine_t stateMachine;