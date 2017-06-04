#include "pid.h"

/**    
 * @addtogroup PID    
 * @{    
 */

/**    
 * @brief  Initialization function for the floating-point PID Control.   
 * @param[in,out] *S points to an instance of the PID structure.   
 * @param[in]     resetStateFlag  flag to reset the state. 0 = no change in state & 1 = reset the state.   
 * @return none.   
 * \par Description:   
 * \par    
 * The <code>resetStateFlag</code> specifies whether to set state to zero or not. \n   
 * The function computes the structure fields: <code>A0</code>, <code>A1</code> <code>A2</code>    
 * using the proportional gain( \c Kp), integral gain( \c Ki) and derivative gain( \c Kd)    
 * also sets the state variables to all zeros.    
 */

void arm_pid_init_f32(
  arm_pid_instance_f32 * S,
  int32_t resetStateFlag)
{

  /* Derived coefficient A0 */
  S->A0 = S->Kp + S->Ki + S->Kd;

  /* Derived coefficient A1 */
  S->A1 = (-S->Kp) - ((float32_t) 2.0 * S->Kd);

  /* Derived coefficient A2 */
  S->A2 = S->Kd;

  /* Check whether state needs reset or not */
  if(resetStateFlag)
  {
    /* Clear the state buffer.  The size will be always 3 samples */
    S->state[0]=0;
		S->state[1]=0;
		S->state[2]=0;
  }

}


float32_t arm_pid_f32(
arm_pid_instance_f32 * S,
float32_t in)
{
	float32_t out;

	/* y[n] = y[n-1] + A0 * x[n] + A1 * x[n-1] + A2 * x[n-2]  */
	out = (S->A0 * in) +
		(S->A1 * S->state[0]) + (S->A2 * S->state[1]) + (S->state[2]);

	/* Update state */
	S->state[1] = S->state[0];
	S->state[0] = in;
	S->state[2] = out;

	/* return to application */
	return (out);

}


 /**    
 * @addtogroup PID    
 * @{    
 */

/**    
* @brief  Reset function for the floating-point PID Control.   
* @param[in] *S	Instance pointer of PID control data structure.   
* @return none.    
* \par Description:   
* The function resets the state buffer to zeros.    
*/
void arm_pid_reset_f32(
  arm_pid_instance_f32 * S)
{

  /* Clear the state buffer.  The size will be always 3 samples */
		S->state[0]=0;
		S->state[1]=0;
		S->state[2]=0;
}



