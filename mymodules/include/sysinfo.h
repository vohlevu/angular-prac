/******************************************************************************
 File name: 	SysInfo.h
 Copyright (c) 2007 KENVI, Corp. All Rights Reserved
 Proprietary and Confidential
 Purpose:		
 Note:			
******************************************************************************/
/**
* @file   SysInfo.h
* @brief  User configuration
*/
#ifndef __SYS_INFO__
#define __SYS_INFO__

#ifndef NO_DTV_SUPPORT
#include "dtv/defs_dtv_sysinfo.h"
#endif

class CSysInfo
{
//Variables
private:
	
public:

//Funtions
public:
	CSysInfo();
	~CSysInfo();
#ifndef NO_DTV_SUPPORT
    RMstatus Savedtv(SDtvParams *p_SDtvParams);
	RMstatus Readdtv(SDtvParams *p_SDtvParams);
#endif
	void ResetSysInfo();
};

#endif //__SYS_INFO__

/******************************************************************************
	End
******************************************************************************/
