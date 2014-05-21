/******************************************************************************
*
* File name:	dtvdefs_sysinfo.h	
* Copyright 
* Created:	
* Author:		
* Purpose:	
* Note:				
*
******************************************************************************/
#ifndef __DTV_DEFS_SYSINFO__
#define __DTV_DEFS_SYSINFO__

#include "defs_dtv.h"
#include "defs_dtv_chaninfo.h"

#define MAX_TUNER_NUM	1
#define MAX_SATELLITE  10

typedef struct SAutoScanParams 
{
	RMuint8 	uiBandwidth;
	RMuint32 	uiStartFreq;
	RMuint32 	uiEndFreq;
	RMuint32 	uiNitFreq;
	RMuint8 	uiModulation;
	RMuint16	uiSymbolRate;
	
}SAutoScanParams;

typedef struct SSettingOTA 
{
	RMuint8 	uiOTAMode;
	RMuint32 	uiOTAFreq;
	RMuint32 	uiOTASymbolRate;
	RMuint8 	uiOTAModulation;
	
}SSettingOTA;

typedef struct SDChanInfo
{
	DigitalChannel	 	astDChannel[MAX_DIGITAL_CHANNEL];
	MGTInfo				aMgt[ATSC_BCAST_LIST_SIZE];
	SAutoScanParams		sAutoScanParams;
	SSettingOTA			sSettingOTA;
	RMuint8				scan_type;
	RMuint8				uiTunerType;
	RMuint16		 	currentDChannel;
	RMuint16			numDChannel;
    RMuint16			numDChannelOrg;
	RMuint16			numHDChannel;
	RMuint16 			lastChanIdx;
	RMuint16 			firstChanIdx;
    RMbool              resetChannel;
	RMuint8				focusSortChannel;
	//dvbc
    RMuint32            dvbcBandWidth;
    RMuint32            dvbcFreq;
	RMuint32            dvbcSymbolRate;
	RMuint8 			dvbcQAM;
	RMbool 				bIsStandby;
	//Identify video channel or audio channel
	RMuint16    		videoChanNum;
	RMuint16			aVideoChansIndex[MAX_DIGITAL_CHANNEL];
    RMuint16    		radioChanNum;
	RMuint16			aRadioChansIndex[MAX_DIGITAL_CHANNEL];
    
    //save status subtitle
    RMint16 	        statusSubLang;
	//parental control
	RMuint8				parentalCtrlOnOff;
	RMuint8				parentalCtrlAge;

	SNITInfo			sNit;
	SSDTInfo			sSdt;
	//update fw
	char 				version_fw[120];
	char				date_release[40];	
	//volume mode
	RMuint8				modeVolume;	
	RMuint8				volume; //volume for all channel		

	//favor
	RMuint8				fvGroupIdx;
	SFavorGroup			sFavorGroup[MAX_FAVOR_GROUP];
}SDChanInfo;

typedef struct SDtvParams 
{
	RMuint8 	uiTunerNum;
	RMuint8 	uiCurTunerID;
	SDChanInfo  sChanInfo[MAX_TUNER_NUM];	
	RMuint64 	ulClockId;
	RMuint8 	uiTimezone;
	
}SDtvParams;


#endif //__DTV_DEFS_SYSINFO__

/******************************************************************************
	End
******************************************************************************/
