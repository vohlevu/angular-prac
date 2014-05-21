/******************************************************************************
*
* File name		: zenotimer.h
* Copyright (c) 2008, Kenvi.  All rights reserved.
* Created Date	: 27-May-2008
* Autho	r		: y.vo@kenvi.com
* Purpose		: 
* Note			: 
*
******************************************************************************/


#ifndef __ZENO_CTIMER_H__
#define __ZENO_CTIMER_H__

#include <stdio.h>
#include <time.h>
#include <windows.h>
//////////////////////////////////////////////////////////////////////////
// Define structures, enums
//////////////////////////////////////////////////////////////////////////

/* IN UNIX the use of the timezone struct is obsolete;
 I don't know why you use it. See http://linux.about.com/od/commands/l/blcmdl2_gettime.htm
 But if you want to use this structure to know about GMT(UTC) diffrence from your local time
 it will be next: tz_minuteswest is the real diffrence in minutes from GMT(UTC) and a tz_dsttime is a flag
 indicates whether daylight is now in use
*/
struct timezone2 
{
  __int32  tz_minuteswest; /* minutes W of Greenwich */
  bool  tz_dsttime;     /* type of dst correction */
};

struct timeval2 {
__int32    tv_sec;         /* seconds */
__int32    tv_usec;        /* microseconds */
};

const __int64 DELTA_EPOCH_IN_MICROSECS= 11644473600000000;
//////////////////////////////////////////////////////////////////////////
int gettimeofday(struct timeval2 *tv/*in*/, struct timezone2 *tz/*in*/);
// Define class
//////////////////////////////////////////////////////////////////////////
class CTimer
{
//variables
private:
public:
    
//Methods
private:
    void Init();
public:
    CTimer();
    ~CTimer();
	
	int		GetSystemUnixTime(struct tm& pDt);
	int		GetSystemTime(struct tm& pDt);
};

#endif //__ZENO_CTIMER_H__
/* end */
