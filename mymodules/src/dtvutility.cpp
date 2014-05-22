/******************************************************************************
 File name: 	dtvutility.cpp
 Copyright (c) 2004 KENVI, Corp. All Rights Reserved
 Proprietary and Confidential
 Purpose:		Include some common functions.
 Note:			phu.nguyen@kenvi.com	
 Version	
 Date		
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dtvutility.h"

void GPSToUTC(RMuint32 uiPGSSec, RMuint32* puiUTC) {
	*puiUTC = uiPGSSec + UTCTOGPS;
}

RMuint32 GetUTCFromGPS(RMuint32 uiPGSSec) {
	return (uiPGSSec + UTCTOGPS);
}

void GPSToUTCDayTime(RMuint32 uiGPSSec, DayTime_dtv* pDT) {
	RMuint32 uiUCTSec;

	GPSToUTC(uiGPSSec, &uiUCTSec);
	UTCToDayTime(uiUCTSec, pDT);
}

void UTCToDayTime(RMuint32 uiSecond, DayTime_dtv* pDT) {
	struct tm sTimeInfo;

	sTimeInfo = *gmtime((time_t*) &uiSecond);
	pDT->ymdw.year = sTimeInfo.tm_year + 1900;
	pDT->ymdw.month = sTimeInfo.tm_mon + 1;
	pDT->ymdw.day = sTimeInfo.tm_mday;
	pDT->ymdw.week_day = sTimeInfo.tm_wday;
	pDT->hms.hour = sTimeInfo.tm_hour;
	pDT->hms.minute = sTimeInfo.tm_min;
	pDT->hms.second = sTimeInfo.tm_sec;
}

void UTCDayTimeToGMTDayTime(DayTime_dtv sUTC, TimeZone_dtv sTZ, RMuint16 uiDS,
		DayTime_dtv* pGMT) {
	ChangeDayTimeZone(sUTC, sTZ, uiDS, pGMT);
}

void PrintDayTime(char* sComment, DayTime_dtv sDT) {
	printf(
			"%s: (DD:MM:YYYY, HH:MM:SS) = %2.2d:%2.2d:%4.4d, %2.2d:%2.2d:%2.2d \n",
			sComment, sDT.ymdw.day, sDT.ymdw.month, sDT.ymdw.year, sDT.hms.hour,
			sDT.hms.minute, sDT.hms.second);

}
void PrintTimeZone(char* sComment, TimeZone_dtv sTZ) {
	printf("%s: (HH:MM), Status = (%2.2d:%2.2d), %d \n", sComment, sTZ.hour,
			sTZ.minute, sTZ.status);

}

void ConvertTmToDaytimeDtv(struct tm pTM, struct DayTime_dtv* pDt) {
	//year
	pDt->ymdw.year		= pTM.tm_year + 1900;		//[1900, ...]
	
	//month
	pDt->ymdw.month		= pTM.tm_mon + 1;		//[0, 11]
	//day
	pDt->ymdw.day		= pTM.tm_mday;		//[1, 31]
	
	//hour
	pDt->hms.hour		= pTM.tm_hour;		//[0, 23]
	//minute
	pDt->hms.minute		= pTM.tm_min;		//[0, 59]
	//second
	pDt->hms.second		= pTM.tm_sec;		//[0, 59]
	//week day
	pDt->ymdw.week_day	= pTM.tm_wday;
	if (pDt->ymdw.week_day == 0) {
		pDt->ymdw.week_day = 7;
	}
}

RMstatus GetTimeZone(char* tz,struct TimeZone_dtv* pTimezone)
{
	int hour = 0;
	int minute = 0;
	char tzone[14];
	if (tz != NULL) {
		char *pch = strstr (tz, "GMT");
		if (pch != NULL) {
			hour = strtol (pch+3, NULL, 10);
			hour = 0 - hour;
		}
	}
	pTimezone->hour = abs(hour);
	
	sprintf(tzone,"%x",minute);
	pTimezone->minute = atoi(tzone);
	
	if (hour > 0) {
		sprintf(pTimezone->acSTimeZoneName, "GMT:+%2.2x:%2.2x", hour, minute);
		pTimezone->status = ADD;
	}
	else
	{
		sprintf(pTimezone->acSTimeZoneName, "GMT:-%2.2x:%2.2x", hour, minute);
		pTimezone->status = MINUS;
	}

	return RM_OK;
}

void GetEventTime(DayTime_dtv stTime, char* pcTime)
{
	if (pcTime != NULL)
	{
		sprintf(pcTime, "%s %d %s%d:%s%d %s",
			aacMonths[stTime.ymdw.month - 1],
			stTime.ymdw.day,
			(stTime.hms.hour == 0 || stTime.hms.hour == 12 || (stTime.hms.hour % 12) == 11 || (stTime.hms.hour % 12) == 10)?"" : "0",
			((stTime.hms.hour%12) == 0)?12:stTime.hms.hour%12,
			(stTime.hms.minute >= 10)?"" : "0",
			stTime.hms.minute,
			((stTime.hms.hour>=12 && stTime.hms.hour<24)?"PM":"AM") );

		
	}
}

void GetEventTime_StartEnd(DayTime_dtv stStartTime, DayTime_dtv stEndTime, char* pcStartEndTime)
{
	sprintf(pcStartEndTime, "%s%d:%s%d %s - %s%d:%s%d %s",
	// StartTime
	(stStartTime.hms.hour == 0 || stStartTime.hms.hour == 12 || (stStartTime.hms.hour%12) == 11 ||(stStartTime.hms.hour%12) == 10)?"" : "0",
	((stStartTime.hms.hour%12) == 0)?12:stStartTime.hms.hour%12,
	(stStartTime.hms.minute >= 10)?"" : "0",
	stStartTime.hms.minute,
	(stStartTime.hms.hour>=12 && stStartTime.hms.hour < 24)?"PM":"AM",
	// EndTime
	(stEndTime.hms.hour == 0 || stEndTime.hms.hour == 12 || (stEndTime.hms.hour%12) == 11 ||(stEndTime.hms.hour%12) == 10)?"" : "0",
	((stEndTime.hms.hour%12) == 0)?12:stEndTime.hms.hour%12,
	(stEndTime.hms.minute >= 10)?"" : "0",
	stEndTime.hms.minute,
	(stEndTime.hms.hour>=12 && stEndTime.hms.hour<24)?"PM":"AM");
}
/**
 * <b>FUNCTION: </b>
 *
 * ConvertUintToString(RMuint8 uiSrc, char *pStrDes)
 *
 * <b>DESCRIPTION: </b>
 *
 * @param
 *   RMuint8 uiSrc, char *pStrDes
 * @return	No
 */
void ConvertUintToString(RMuint16 uiSrc, char *pStrDes) {
	char s[15] = { 0, };
	sprintf(s, "%d", uiSrc);
	//pStrDes = (char *) malloc(15);	
	strcpy(pStrDes, s);
}

RMuint32 GetNumSecondFromDefaultDate(struct DayTime_dtv stDt) {
	RMuint32 uiRet = 0;
	RMuint16 i;

	//Year
	for (i = 1970; i < stDt.ymdw.year; i++) {
		uiRet += GetNumSecondOfYear(i);
	}

	//Month
	for (i = 1; i < stDt.ymdw.month; i++) {
		uiRet += GetNumSecondOfMonth(stDt.ymdw.year, i);
	}

	//Day
	uiRet += (stDt.ymdw.day - 1) * 24 * 60 * 60;

	//Hour
	uiRet += (stDt.hms.hour) * 60 * 60;

	//Minute
	uiRet += (stDt.hms.minute) * 60;

	//Second
	uiRet += stDt.hms.second;

	return uiRet;

}

RMint16 ChangeDayTimeZone(struct DayTime_dtv dt1, TimeZone_dtv stTzone,
		RMuint32 daylight, struct DayTime_dtv *dt2) {

	RMint16 odd = 0, total = 0;
	RMint16 tz_hour, tz_minute;
	RMuint16 num_day;
//	printf("inc : hms = [%d:%d] Daylight=[%ld]\n",stTzone.hour,stTzone.minute,daylight);

	if (stTzone.status == ADD) {
		tz_hour = stTzone.hour;
		tz_minute = stTzone.minute;
	} else //stTzone.status == MINUS)
	{
		tz_hour = -stTzone.hour;
		tz_minute = -stTzone.minute;
	}

	//Second
	dt2->hms.second = dt1.hms.second;
	//printf(" second %d >> %d (+%d)\t", total, dt2->hms.second, odd_minute);

	//Minute
	total = dt1.hms.minute + tz_minute;
	if (total >= 60) {
		dt2->hms.minute = (RMuint8)(total % 60);
		odd = 1;
	} else if (total >= 0) {
		dt2->hms.minute = (RMuint8) total;
		odd = 0;
	} else { //total <= 0
		dt2->hms.minute = (RMuint16) total + 60;
		odd = -1;
	}
	//printf(" minute %d >> %d (+%d)\t", total, dt2->hms.minute, odd_hour);

	//Hours
	if (daylight == 0) {
		total = dt1.hms.hour + tz_hour + odd;
	} else {
		total = dt1.hms.hour + tz_hour + odd + 1;
	}
	if (total >= 24) {
		dt2->hms.hour = (RMuint8)(total % 24);
		odd = 1;
	} else if (total >= 0) {
		dt2->hms.hour = (RMuint8)(total);
		odd = 0;
	} else { //total <= 0
		dt2->hms.hour = (RMuint8)(total + 24);
		odd = -1;
	}
	//printf(" hour %d >> %d (+%d)\n", total, dt2->hms.hour, odd_day);	

	//DayWeek
	total = dt1.ymdw.week_day + odd;
	if (total > 7) {
		dt2->ymdw.week_day = 1;
	} else if (total > 0) {
		dt2->ymdw.week_day = (RMuint8) total;
	} else { // total <= 0
		dt2->ymdw.week_day = (RMuint8)(total + 7);
	}
	//printf(" week_day %d >> %d \t", total, dt2->ymdw.week_day);

	//Day Month
	num_day = GetNumDayOfMonth(dt1.ymdw.year, dt1.ymdw.month);

	//Day
	total = dt1.ymdw.day + odd;
	if (total > num_day) {
		dt2->ymdw.day = 1;
		odd = 1;
	} else if (total > 0) {
		dt2->ymdw.day = (RMuint16) total;
		odd = 0;
	} else { //total <= 0
			 //set day later
		odd = -1;
	}

	//Month
	total = dt1.ymdw.month + odd;
	if (total > 12) {
		dt2->ymdw.year = dt1.ymdw.year + 1;
		dt2->ymdw.month = 1;
	} else if (total > 0) {
		dt2->ymdw.year = dt1.ymdw.year;
		dt2->ymdw.month = (RMuint16) total;
	} else // total == 0
	{
		dt2->ymdw.year = dt1.ymdw.year - 1;
		dt2->ymdw.month = 12;
	}

	if (odd == -1) {
		dt2->ymdw.day = GetNumDayOfMonth(dt2->ymdw.year, dt2->ymdw.month);
	}

	//	printf("dt1 : hms = %d:%d:%d\t dayweek, dmy = %d, %d %d %d\n",dt1.hms.hour,dt1.hms.minute,dt1.hms.second,dt1.ymdw.week_day, dt1.ymdw.day,dt1.ymdw.month,dt1.ymdw.year);
	//	printf("dt2 : hms = %d:%d:%d\t dayweek, dmy = %d, %d %d %d\n",dt2->hms.hour,dt2->hms.minute,dt2->hms.second,dt2->ymdw.week_day, dt2->ymdw.day,dt2->ymdw.month,dt2->ymdw.year);

	return 0;
}

RMint32 GetDateFromSecond(RMuint32 uiSecond, RMint16 iTimeZone,
		struct DayTime_dtv* pDt) {

	RMuint16 i = 0;
	RMuint32 uiTemp = 0;
	RMuint32 uiTempSec = 0;
	RMuint32 MJD = 0;
	RMuint16 Y, M, D, L;
	if (pDt == NULL) {
		printf("getDateFromSecond: pDt == NULL \n");
		return RM_ERROR;
	}
	//Set TimeZone
	uiSecond += iTimeZone * 60 * 60;

	//year
	uiTempSec = 0;
	uiTemp = 0;
	i = 1970;
	while (uiTemp <= uiSecond) {
		uiTempSec = uiTemp;
		uiTemp += GetNumSecondOfYear(i);
		i++;
	}
	pDt->ymdw.year = i - 1;
	uiSecond -= uiTempSec;

	//Month
	uiTemp = 0;
	uiTempSec = 0;
	i = 1;
	while (uiTemp <= uiSecond) {
		uiTempSec = uiTemp;
		uiTemp += GetNumSecondOfMonth(pDt->ymdw.year, i);
		i++;
	}
	pDt->ymdw.month = i - 1;
	uiSecond -= uiTempSec;

	//day
	uiTemp = 0;
	uiTempSec = 0;
	i = 1;
	while (uiTemp <= uiSecond) {
		uiTempSec = uiTemp;
		uiTemp += 24 * 60 * 60;
		i++;
	}
	pDt->ymdw.day = i - 1;
	uiSecond -= uiTempSec;

	//Hour
	uiTemp = 0;
	uiTempSec = 0;
	i = 0;
	while (uiTemp <= uiSecond) {
		uiTempSec = uiTemp;
		uiTemp += 60 * 60;
		i++;
	}
	pDt->hms.hour = i - 1;
	uiSecond -= uiTempSec;

	//Minute
	uiTemp = 0;
	uiTempSec = 0;
	i = 0;
	while (uiTemp <= uiSecond) {
		uiTempSec = uiTemp;
		uiTemp += 60;
		i++;
	}
	pDt->hms.minute = i - 1;
	uiSecond -= uiTempSec;

	//Second
	pDt->hms.second = uiSecond;

	//MDJ

	/*
	 If M = 1 or M = 2, then L = 1; else L = 0 
	 MJD = 14 956 + D + int [ (Y - L) × 365,25] + int [ (M + 1 + L × 12) × 30,6001 ] 
	 
	 */
	Y = pDt->ymdw.year;
	M = pDt->ymdw.month;
	D = pDt->ymdw.day;
	if (M == 1 || M == 2) {
		L = 1;
	} else {
		L = 0;
	}
	MJD = 14956 + D + (RMuint16)((Y - L) * 365.25)
			+ (RMuint16)((M + 1 + L * 12) * 30.6001);

	pDt->ymdw.week_day = ((MJD + 2) % 7) + 1; //Mon = 1, Sun = 7.

	return RM_OK;

}

RMuint32 GetNumSecondOfYear(RMuint16 uiYear) {
	RMuint32 uiRet = 0;
	RMuint16 uiNumDay = 0;

	if ((uiYear % 100 != 0 && uiYear % 4 == 0) || (uiYear % 400 == 0)) {
		uiNumDay = 366;
	} else {
		uiNumDay = 365;
	}

	uiRet += uiNumDay * 24 * 60 * 60;
	return uiRet;
}

RMuint32 GetNumSecondOfMonth(RMuint16 uiYear, RMuint16 uiMonth) {
	RMuint32 uiRet = 0;
	RMuint8 uiNumDay = 0;

	switch (uiMonth) {
	case 4:
	case 6:
	case 9:
	case 11:
		uiNumDay = 30;
		break;
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		uiNumDay = 31;
		break;

	case 2:
		if ((uiYear % 100 != 0 && uiYear % 4 == 0) || (uiYear % 400 == 0)) {
			uiNumDay = 29;
		} else {
			uiNumDay = 28;
		}
		break;

	default:
		uiNumDay = 0;
		;
	}

	uiRet = uiNumDay * 24 * 60 * 60;
	return uiRet;
}
/**
 * <b>FUNCTION: </b>
 *
 * 	RMuint16 ChangeDayTime(struct DayTime_dtv dt1, struct HourMinuteSecond inc, struct DayTime_dtv *dt2)
 *
 * <b>DESCRIPTION: </b>
 *
 * @param:
 * @return :
 */
RMint16 ChangeDayTime(struct DayTime_dtv dt1, struct HourMinuteSecond_dtv inc,
		struct DayTime_dtv *dt2) {
	RMuint8 num_day = 0;
	RMuint8 odd_hour = 0, odd_minute = 0;
	RMuint8 odd_year = 0, odd_month = 0, odd_day = 0, odd_week;

	//	printf("inc : hms = %d:%d:%d\n",inc.hour,inc.minute,inc.second);
	//Second

	RMuint8 total;

	total = dt1.hms.second + inc.second;
	odd_minute = (total >= 60);
	dt2->hms.second = total % 60;
	//	printf(" second %d >> %d (+%d)\t", total, dt2->hms.second, odd_minute);

	//Minute
	total = dt1.hms.minute + inc.minute + odd_minute;
	odd_hour = (total >= 60);
	dt2->hms.minute = total % 60;
	//	printf(" minute %d >> %d (+%d)\t", total, dt2->hms.minute, odd_hour);

	//Hours
	total = dt1.hms.hour + inc.hour + odd_hour;
	odd_day = (total >= 24);
	dt2->hms.hour = total % 24;
	//	printf(" hour %d >> %d (+%d)\n", total, dt2->hms.hour, odd_day);

	//DayWeek
	total = dt1.ymdw.week_day + odd_day;
	odd_week = (total > 7);
	if (odd_week > 0)
		dt2->ymdw.week_day = 1;
	else
		dt2->ymdw.week_day = total;
	//	printf(" week_day %d >> %d \t", total, dt2->ymdw.week_day);

	//Day Month

	num_day = GetNumDayOfMonth(dt1.ymdw.year, dt1.ymdw.month);
	//Day
	total = dt1.ymdw.day + odd_day;
	odd_month = total > num_day;
	if (odd_month > 0)
		dt2->ymdw.day = 1;
	else
		dt2->ymdw.day = total;
	//	printf(" day %d >> %d (+%d)\t", total, dt2->ymdw.day, odd_month);

	//Month
	total = dt1.ymdw.month + odd_month;
	odd_year = (total > 12);
	if (odd_year)
		dt2->ymdw.month = 1;
	else
		dt2->ymdw.month = total;

	//	printf(" month %d >> %d (+%d)\t", total, dt2->ymdw.month, odd_year);

	//Year
	dt2->ymdw.year = dt1.ymdw.year + odd_year;
	//	printf(" year %d \n", dt2->ymdw.year);

	//	printf("dt1 : hms = %d:%d:%d\t dayweek, dmy = %d, %d %d %d\n",dt1.hms.hour,dt1.hms.minute,dt1.hms.second,dt1.ymdw.week_day, dt1.ymdw.day,dt1.ymdw.month,dt1.ymdw.year);
	//	printf("dt2 : hms = %d:%d:%d\t dayweek, dmy = %d, %d %d %d\n",dt2->hms.hour,dt2->hms.minute,dt2->hms.second,dt2->ymdw.week_day, dt2->ymdw.day,dt2->ymdw.month,dt2->ymdw.year);

	return 0;
}

/**
 * <b>FUNCTION: </b>
 *
 * 	RMuint16 GetNumDayOfMonth(RMuint16 uiYear, RMuint16 uiMonth)
 *
 * <b>DESCRIPTION: </b>
 *
 *	Get Number Day Of 1 Month
 *
 * @param:
 *		RMuint16 uiYear				:	value of year
 *
 *		RMuint16 uiMonth			:	value of month
 * @return :
 *	number day of uiMonth
 */
RMuint16 GetNumDayOfMonth(RMuint16 uiYear, RMuint16 uiMonth) {
	RMuint16 uiNumDay = 0;
	//Day Month
	switch (uiMonth) {
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		uiNumDay = 31;
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		uiNumDay = 30;
		break;
	case 2:
		if (((uiYear % 100 != 0) && (uiYear % 4 == 0)) || (uiYear % 400 == 0))
			uiNumDay = 29;
		else
			uiNumDay = 28;

		break;
	}
	return uiNumDay;
}

/**
 * <b>FUNCTION: </b>
 *
 * 	RMint16 CompareDayTime(struct DayTime_dtv dt1, struct DayTime_dtv dt2)
 *
 * <b>DESCRIPTION: </b>
 *
 *	Compare DayTime
 *
 * @param:
 *		struct DayTime_dtv dt1
 *
 *		struct DayTime_dtv dt2
 * @return :
 *		-1:		dt1 < dt2
 *		0:		dt1 = dt2
 *		1:		dt1 > dt2
 */
RMint16 CompareDayTime(struct DayTime_dtv dt1, struct DayTime_dtv dt2) {
	//Year
	if (dt1.ymdw.year > dt2.ymdw.year)
		return 1;
	else if (dt1.ymdw.year < dt2.ymdw.year)
		return -1;
	//Month
	else if (dt1.ymdw.month > dt2.ymdw.month)
		return 1;
	else if (dt1.ymdw.month < dt2.ymdw.month)
		return -1;
	//Day
	else if (dt1.ymdw.day > dt2.ymdw.day)
		return 1;
	else if (dt1.ymdw.day < dt2.ymdw.day)
		return -1;
	//Hour
	else if (dt1.hms.hour > dt2.hms.hour)
		return 1;
	else if (dt1.hms.hour < dt2.hms.hour)
		return -1;
	//Minute
	else if (dt1.hms.minute > dt2.hms.minute)
		return 1;
	else if (dt1.hms.minute < dt2.hms.minute)
		return -1;
	//Second
	else if (dt1.hms.second > dt2.hms.second)
		return 1;
	else if (dt1.hms.second < dt2.hms.second)
		return -1;

	return 0;
}

/**
 * <b>FUNCTION: </b>
 *
 *       RMint16 ParseHMS(RMuint8 *p, struct HourMinuteSecond
 *        pHms)
 *
 * <b>DESCRIPTION: </b>
 *
 *	parse 3 bytes buffer to HourMinuteSecond struct.
 *
 * @param:
 *	- RMuint8 *p					: 3 bytes buffer.
 *
 *	- struct HourMinuteSecond *pHms	: a pointer to HMS struct.
 *
 * @return : 
 *	- RM_OK		: if success.
 *	- RM_ERROR	: otherwise
 */
RMint16 ParseHMS(RMuint8 *p, struct HourMinuteSecond_dtv *pHms) {
	//a) To find Y, M, D from MJD
	//	Y' = int [ (MJD - 15 078,2) / 365,25 ]
	//	M' = int { [ MJD - 14 956,1 - int (Y' × 365,25) ] / 30,6001 }
	//	D = MJD - 14 956 - int (Y' × 365,25) - int (M' × 30,6001 )
	//	If M' = 14 or M' = 15, then K = 1; else K = 0
	//	Y = Y' + K
	//	M = M' - 1 - K × 12
	//b) To find MJD from Y, M, D
	//	If M = 1 or M = 2, then L = 1; else L = 0
	//	MJD = 14 956 + D + int [ (Y - L) × 365,25] + int [ (M + 1 + L × 12) × 30,6001 ]
	//c) To find WD from MJD
	//	WD = [ (MJD + 2) mod 7 ] + 1
	//d) To find MJD from WY, WN, WD
	//	MJD = 15 012 + WD + 7 × { WN + int [ (WY × 1 461 / 28) + 0,41] }
	//e) To find WY, WN from MJD
	//	W = int [ (MJD / 7) - 2 144,64 ]
	//	WY = int [ (W × 28 / 1 461) - 0,0079]
	//	WN = W - int [ (WY × 1 461 / 28) + 0,41]
	//EXAMPLE: MJD = 45 218 W = 4 315
	//	 Y = (19)82 WY = (19)82
	//	 M = 9 (September) N = 36
	//	 D = 6 WD = 1 (Monday)
	//NOTE: These formulas are applicable between the inclusive dates 1900 March 1 to 2100 February 28.
	//printf("===== p[0] = %d", p[0]);	
	//Phu Nguyen add
	if (p == NULL || pHms == NULL) {
		printf("\t*** ParseHMS: invalid arguments \n");
		return RM_ERROR;
	}
	pHms->hour = p[0] >> 4;
	pHms->hour *= 10;
	pHms->hour += p[0] & 0x0F;

	pHms->minute = p[1] >> 4;
	pHms->minute *= 10;
	pHms->minute += p[1] & 0x0F;

	pHms->second = p[2] >> 4;
	pHms->second *= 10;
	pHms->second += p[2] & 0x0F;

	//hack
	if (pHms->hour >= 24)
		pHms->hour = 0;

	if (pHms->minute >= 60)
		pHms->minute = 0;

	if (pHms->second >= 60)
		pHms->second = 0;
	return RM_OK;
}

/**
 * <b>FUNCTION: </b>
 *
 *       RMint16 ParseYMDW(RMuint8 *p, struct YearMontDayWeek
 *        pYmdw)
 *
 * <b>DESCRIPTION: </b>
 *
 *	parse 2 bytes buffer to YearMontDayWeek struct.
 *
 * @param:
 *	- RMuint8 *p					: 3 bytes buffer.
 *
 *	- struct YearMontDayWeek *pYmdw	: a pointer to pYmdw struct.
 *
 * @return :
 *	- RM_OK		: if success.
 *	- RM_ERROR	: otherwise
 */
RMint16 ParseYMDW(RMuint8 *p, struct YearMontDayWeek_dtv *pYmdw) {
	//a) To find Y, M, D from MJD
	//	Y' = int [ (MJD - 15 078,2) / 365,25 ]
	//	M' = int { [ MJD - 14 956,1 - int (Y' × 365,25) ] / 30,6001 }
	//	D = MJD - 14 956 - int (Y' × 365,25) - int (M' × 30,6001 )
	//	If M' = 14 or M' = 15, then K = 1; else K = 0
	//	Y = Y' + K
	//	M = M' - 1 - K × 12
	//b) To find MJD from Y, M, D
	//	If M = 1 or M = 2, then L = 1; else L = 0
	//	MJD = 14 956 + D + int [ (Y - L) × 365,25] + int [ (M + 1 + L × 12) × 30,6001 ]
	//c) To find WD from MJD
	//	WD = [ (MJD + 2) mod 7 ] + 1
	//d) To find MJD from WY, WN, WD
	//	MJD = 15 012 + WD + 7 × { WN + int [ (WY × 1 461 / 28) + 0,41] }
	//e) To find WY, WN from MJD
	//	W = int [ (MJD / 7) - 2 144,64 ]
	//	WY = int [ (W × 28 / 1 461) - 0,0079]
	//	WN = W - int [ (WY × 1 461 / 28) + 0,41]
	//EXAMPLE: MJD = 45 218 W = 4 315
	//	 Y = (19)82 WY = (19)82
	//	 M = 9 (September) N = 36
	//	 D = 6 WD = 1 (Monday)
	//NOTE: These formulas are applicable between the inclusive dates 1900 March 1 to 2100 February 28.

	RMuint16 MJD = 0;
	RMuint16 K, M1, Y1;

	//Phu Nguyen add
	if (p == NULL || pYmdw == NULL) {
		printf("\t*** ParseYMDW: invalid arguments \n");
		return RM_ERROR;
	}
	MJD = p[0];
	MJD <<= 8;
	MJD |= p[1];
	//	MJD &= 0xFFFF;
	Y1 = (RMuint16)((MJD - 15078.2) / 365.25);
	//	Y1 &= 0x0000FFFF;
	//	printf("Y1[%x]\n", Y1);
	M1 = (RMuint16)((MJD - 14956.1 - (Y1 * 365.25)) / 30.6001);
	//	M1 &= 0xFFFF;
	//	printf("M1[%x]\n", M1);
	pYmdw->day = MJD - 14956 - (RMuint16)(Y1 * 365.25)
			- (RMuint16)(M1 * 30.6001);
	//	printf("Day[%x]\n", pYmdw->day);
	if ((M1 == 14) || (M1 == 15))
		K = 1;
	else
		K = 0;
	pYmdw->year = 1900 + Y1 + K;
	pYmdw->month = M1 - 1 - K * 12;

	pYmdw->week_day = ((MJD + 2) % 7) + 1; //[1:Monday - 7: Sunday]
	//hack
	if (pYmdw->month > 12)
		pYmdw->month = 1;

	if (pYmdw->day > 31)
		pYmdw->day = 1;
	if (pYmdw->year < 1970) {
		pYmdw->year = 1970;
	}
	if (pYmdw->week_day > 7) {
		pYmdw->week_day = 1;
	}
	return RM_OK;
}

/**
 * <b>FUNCTION: </b>
 *
 * 		RMint16 ParseYMDW_HMS(RMuint8 *p, struct DayTime *pDt)
 *
 * <b>DESCRIPTION: </b>
 *
 *	parse 5 bytes buffer to DayTime struct.
 *
 * @param:
 *	- RMuint8 *p			: 3 bytes buffer.
 *
 *	- struct DayTime *pDt	: a pointer to DayTime struct.
 * @return :
 *	- RM_OK		: if success.
 *	- RM_ERROR	: otherwise
 */
RMint16 ParseYMDW_HMS(RMuint8 *p, struct DayTime_dtv *pDt) {
	if (p == NULL || pDt == NULL) {
		printf("\t*** ParseYMDW_HMS: invalid arguments \n");
		return RM_ERROR;
	}
	ParseYMDW(p, &pDt->ymdw);
	ParseHMS(p + 2, &pDt->hms);

	return RM_OK;
}

void ConvertISO8859ToUTF8(unsigned char* in, char* out) {
	int i = 0;
	strcpy(out, "");
	while (*in) {
		if (*in < 128) {
			*out = *in;
			out++;
			i++;
			in++;
		} else {
			*out++ = 0xc2 + (*in > 0xbf);
			*out++ = (*in++ & 0x3f) + 0x80;
			i += 2;
		}
	}
	*out = '\0';
	out -= i;
}
