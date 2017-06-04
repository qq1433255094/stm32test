#ifndef __CAN2_H__
#define __CAN2_H__

#include "main.h"

void CAN2_Configuration(void);

u8 Can2SendMsg(u8* msg,u8 len,u8 id);
#endif 
