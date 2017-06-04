#ifndef __PID_H
#define __PID_H 

#include "main.h"
#define f32_t float32_t

typedef float float32_t;

typedef struct
  {
    f32_t A0;            /**< The derived gain, A0 = Kp + Ki + Kd . */
    f32_t A1;            /**< The derived gain, A1 = -Kp - 2Kd. */
    f32_t A2;            /**< The derived gain, A2 = Kd . */
    f32_t state[3];      /**< The state array of length 3. */
    f32_t Kp;            /**< The proportional gain. */
    f32_t Ki;            /**< The integral gain. */
    f32_t Kd;            /**< The derivative gain. */
  } arm_pid_instance_f32;



void arm_pid_init_f32(
  arm_pid_instance_f32 * S,
  int32_t resetStateFlag);	
float32_t arm_pid_f32(
arm_pid_instance_f32 * S,
float32_t in);
void arm_pid_reset_f32(
  arm_pid_instance_f32 * S);
	
#endif




