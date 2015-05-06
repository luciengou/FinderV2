#ifndef	__Loop_H__

#define __Loop_H__

typedef enum {
	SERVO_CHECK ,
	SERVO_HIGH_MID_LOW_CHECK ,
	SERVO_MOVE_STABLE ,
	MAIN_POWER_CHECK ,
	TOTAL_TIME_CHECK ,
} eWork_State;

typedef enum {
	HIGH_LOW ,
	MOVING_STABLE
} Finder_State ;

static eWork_State Work_State_G = SERVO_CHECK  ;
Finder_State Finder_Mode  ;

void loop(void);

#endif