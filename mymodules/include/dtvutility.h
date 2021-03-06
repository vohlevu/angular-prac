/******************************************************************************
 File name: 	utility.cpp
 Copyright (c) 2004 KENVI, Corp. All Rights Reserved
 Proprietary and Confidential
 Purpose:		Include some common functions.
 Note:			phu.nguyen@kenvi.com	
 Version	
 Date		
******************************************************************************/
#ifndef __DTV_UTILITY_H__
#define __DTV_UTILITY_H__

#include "dtv/defs_dtv_sysinfo.h"

#ifndef STRING_HYPHEN
#define STRING_HYPHEN					"|"
#endif

#ifndef STRING_DOUBLE_HYPHEN
#define STRING_DOUBLE_HYPHEN			"||"
#endif

#ifndef STRING_TRIO_HYPHEN
#define STRING_TRIO_HYPHEN				"|||"
#endif

#define UTCTOGPS	315964811
#define TOTAL_MONTH_AYEAR	12
static char aacMonths[TOTAL_MONTH_AYEAR][4]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", 
								"Aug", "Sep", "Oct", "Nov", "Dec"};

RMstatus 	GetTimeZone(char* tz,struct TimeZone_dtv* pTimezone);
void		ConvertTmToDaytimeDtv(struct tm pTM, struct DayTime_dtv* pDt);
void		GetEventTime(DayTime_dtv stTime, char* pcTime);
void 		GetEventTime_StartEnd(DayTime_dtv stStartTime, DayTime_dtv stEndTime, char* pcStartEndTime);
void		ConvertUintToString(RMuint16 uiSrc, char *pStrDes);
RMuint32 	GetNumSecondFromDefaultDate(struct DayTime_dtv stDt);
RMint32 	GetDateFromSecond(RMuint32 uiSecond, RMint16 iTimeZone, struct DayTime_dtv* pDt);

RMint16 	ChangeDayTimeZone(struct DayTime_dtv dt1, TimeZone_dtv stTzone, RMuint32 daylight, struct DayTime_dtv *dt2);

RMuint32 	GetNumSecondOfYear(RMuint16 uiYear);
RMuint32 	GetNumSecondOfMonth(RMuint16 uiYear, RMuint16 uiMonth);

void		GPSToUTC(RMuint32 uiPGSSec, RMuint32* puiUTC);
RMuint32	GetUTCFromGPS(RMuint32 uiPGSSec);

void		UTCToDayTime(RMuint32 uiSecond, DayTime_dtv* pDT);
void		GPSToUTCDayTime(RMuint32 uiGPSSec, DayTime_dtv* pDT);
void		UTCDayTimeToGMTDayTime(DayTime_dtv sUTC, TimeZone_dtv sTZ, RMuint16 uiDS, DayTime_dtv* pGMT);
void		PrintDayTime(char* sComment, DayTime_dtv sDT);
void		PrintTimeZone(char* sComment, TimeZone_dtv sTZ);
RMint16     ChangeDayTime(struct DayTime_dtv dt1, struct HourMinuteSecond_dtv inc, struct DayTime_dtv *dt2);
RMuint16    GetNumDayOfMonth(RMuint16 uiYear, RMuint16 uiMonth);
RMint16	    CompareDayTime(struct DayTime_dtv dt1, struct DayTime_dtv dt2);
RMint16     ParseHMS(RMuint8 *p, struct HourMinuteSecond_dtv *pHms);
RMint16     ParseYMDW(RMuint8 *p, struct YearMontDayWeek_dtv *pYmdw);
RMint16     ParseYMDW_HMS(RMuint8 *p, struct DayTime_dtv *pDt);
void		ConvertISO8859ToUTF8(unsigned char* in, char* out);
#endif //__DTV_UTILITY_H__
//end of file

