#ifndef _CS_LB_BOOT_TICK_R_VO_H_
#define _CS_LB_BOOT_TICK_R_VO_H_

#include "type.h"

struct CS_LB_BOOT_TICK_R_Vo
{
    Int     Enable;         //< 0:Disable, 1:Enable
Dword       tmSlice;        //< Time to send tick
};

#endif //_CS_LB_BOOT_TICK_R_VO_H_
