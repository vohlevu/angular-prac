/******************************************************************************
*
* File name		: zenoctimer.cpp
* Copyright (c) 2008, Kenvi.  All rights reserved.
* Created		: Date 27-May-2008
* Author		: y.vo@kenvi.com
* Purpose		:		
* Note			:			
*
******************************************************************************/
#include "../include/zenotimer.h"

/**
* <b>FUNCTION: </b>
*
* CTimer()
*
* <b>DESCRIPTION: </b>
* 
* Constructor
* 
* @param
*       
* @return No
*/
CTimer::CTimer()
{
    printf("Constructor\n");
    Init();
}

/**
* <b>FUNCTION: </b>
*
* ~CTimer()
*
* <b>DESCRIPTION: </b>
* 
* Destructor
* 
* @param    No
*       
* @return	No
*/
CTimer::~CTimer()
{
	printf("Destructor\n");
}

/**
* <b>FUNCTION: </b>
*
* Init()
*
* <b>DESCRIPTION: </b>
* 
* Init data
* 
* @param    No
*       
* @return	No
*/
void CTimer::Init()
{
	
}

/**
* <b>FUNCTION: </b>
*
* GetSystemTime(struct SDateTime* pDt)
*
* <b>DESCRIPTION: </b>
*
* Get datetime from system and save into SDateTime struct
*
* @param
*	struct SDateTime* pDt: current time getting from system
*
* @return
*	RM_OK if success. Otherwise RM_ERROR
*/
int CTimer::GetSystemUnixTime(struct tm& pDt)
{
	struct timeval2 tv;
	struct timezone2 tz;
	time_t time1;

	ZeroMemory(&tv,sizeof(tv));
	ZeroMemory(&tz,sizeof(tz));

	gettimeofday(&tv, &tz); // call gettimeofday()
	time1=tv.tv_sec;
	pDt = *localtime(&time1);
	return 1;
}

int CTimer::GetSystemTime(struct tm& pDt)
{
	time_t rawtime;
	time ( &rawtime );
	pDt = *localtime ( &rawtime );
	return 1;
}

int gettimeofday(struct timeval2 *tv/*in*/, struct timezone2 *tz/*in*/)
{
	FILETIME ft;
	__int64 tmpres = 0;
	TIME_ZONE_INFORMATION tz_winapi;
	int rez=0;

	ZeroMemory(&ft,sizeof(ft));
	ZeroMemory(&tz_winapi,sizeof(tz_winapi));

	GetSystemTimeAsFileTime(&ft);

	tmpres = ft.dwHighDateTime;
	tmpres <<= 32;
	tmpres |= ft.dwLowDateTime;

	/*converting file time to unix epoch*/
	tmpres /= 10;  /*convert into microseconds*/
	tmpres -= DELTA_EPOCH_IN_MICROSECS; 
	tv->tv_sec = (__int32)(tmpres*0.000001);
	tv->tv_usec =(tmpres%1000000);


	//_tzset(),don't work properly, so we use GetTimeZoneInformation
	rez=GetTimeZoneInformation(&tz_winapi);
	tz->tz_dsttime=(rez==2)?true:false;
	tz->tz_minuteswest = tz_winapi.Bias + ((rez==2)?tz_winapi.DaylightBias:0);

	return 0;
}
/*================================= end of file =================================*/
