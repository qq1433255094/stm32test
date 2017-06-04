#ifndef __CAN1_H__
#define __CAN1_H__

#include"main.h"

void CAN1_Configuration(void);

u8 Can1SendMsg(u8* msg,u8 len,u8 id);

#endif 
