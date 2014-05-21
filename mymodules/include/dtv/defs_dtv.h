#ifndef _CDEFS_DTV_H_
#define _CDEFS_DTV_H_

#include "defs_dtv_general.h"
#include "defs_dtv_descriptors.h"
#include "defs_dtv_tablesinfo.h"


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////



/**************************************************************************************
 Channel info
 **************************************************************************************/


typedef struct  
{
	RMuint32		freq;
	SProgramInfo*	rgProgram;
	RMuint16		numProgram;
	RMuint32  		symbolrate;
	RMuint16  		QAM;
//	ChanInfoDvbs*	pSDvbs;
	RMbool			isAddCableDelivery;
	RMbool			isRemoveCableDelivery;
	RMuint16 		transport_stream_id;
	RMuint8	 		version_number;

#ifdef DTV_SMK_CAS_SUPPORT
	RMuint16 		emmPid;
#endif //DTV_SMK_CAS_SUPPORT

}ChannelInfo;


typedef struct {	
	RMuint16 numProgram;
	RMuint16 major_channel_number;	//Don't set this variable, it is set when call GetChanInfo
	RMuint32 freq;					//Don't set this variable, it is set when call GetChanInfo
	RMuint16 transport_stream_id;
	RMuint8	 version_number;
} SChanSetInfo;

typedef struct {
	RMuint16 chanIdx;
}SChanParam;

typedef struct  {
	RMuint16 pidAudio[MAX_AUDIO_STREAMS];//13 bits
	RMuint16 streamTypeAudio[MAX_AUDIO_STREAMS];
	RMuint8 numberOfAudioStreams;
}SAudioStreamInfo;

typedef struct  {
	RMuint16 pidVideo;//13 bits
	RMuint16 streamTypeVideo;	
}SVideoStreamInfo;

typedef struct  {
	RMbool isValid;
	SAudioStreamInfo audioStream;
	SVideoStreamInfo videoStream;
	RMuint16 pidPCR;
}SChannelInfo;


/**************************************************************************************
 Tuner and Scan info
 **************************************************************************************/
typedef struct
{
	RMuint32 frequency;
	//for DVBS
//	RMuint32 tpfreq;
//	RMuint32 lnbfreq;
	RMuint32 symbolrate;
	RMuint8  standard;
	//end of DVBS
	RMuint8 signalStrength;
	RMuint8 signalQuality;
	RMbool  isLocked;
	RMuint8 uiBandwidth;	
	//dvb-c
	RMuint16 QAM;
	
}STunerInfo;

typedef struct{
	RMuint16 numProgram;
	RMuint16 percentDone;
	RMuint16 chanIdx;
	ChannelInfo* pChanInfo;
	MGTInfo	*pMgt;
}SScanInfo;

typedef struct  
{
	//general
	RMuint32 freqMax;
	RMuint32 freqMin;
	RMbool 	 isResetChannel;
	RMuint8  scanMode;

	//dvbt, dvbc
	RMuint32 channelSpacing;
	RMuint32 bandWidth;
	RMbool 	 isGetFreqFrNIT;
	
	//dvbc
	RMuint32 symbolRate;

	//dvbc
	RMuint16 QAM;
	RMuint32 freq;
}SDtvScanParam;


typedef struct SScanStateInfo

{

    RMuint32 	frequency;
	RMuint16    channelFound;
	RMuint8 	signalStrength;
	RMuint8 	signalQuality;
	RMuint8 	scanPercent;
	RMbool	 	isLocked;
}SScanStateInfo;


/**************************************************************************************
 Program and Play Params
 **************************************************************************************/
typedef struct {
	RMuint16 chanIdx;
	RMuint16 progIdx;
	RMuint16 vidIdx;
	RMuint16 audIdx;
	RMuint16 program_map_PID;	
}SProgParam;


typedef struct {
	RMuint8		lock;
	RMuint8		fav;
	RMuint8		skip;	
	RMbool		isHD;
	char		videoType[10];
	char		audioType[10];
	SESInfo 	rgVidEs[MAX_ES_NUMBER];
	RMuint16 	numVidEs;
	SESInfo 	rgAudEs[MAX_ES_NUMBER];
	RMuint16 	numAudEs;	
	SESInfo 	rgTeletextEs[MAX_ES_NUMBER];
	RMuint16 	numTeletextEs;	
	SESInfo 	rgSubtitleEs[MAX_ES_NUMBER];
	RMuint16 	numSubtitleEs;
	SNITInfo	sNit;
	SSDTInfo	sSdt;

	STTLang		sttLang[MAX_ES_NUMBER];  
	RMuint16 	numSubLang;  

	RMuint16 	pcr_pid;
	RMuint8		pmt_index;
	RMuint16	source_id;

	RMuint8 	short_name[SHORT_NAME_LENGTH];

	RMuint16 	logChanNum;
	RMuint16	program_map_PID;	
	
	RMuint16	major_channel_number;
	RMuint16	minor_channel_number;
	RMuint16	program_number;
	RMuint8		free_CA_mode;
    SParentalRating	rgParentalRating;
    RMuint16	service_type; 			//for identifying one-seg programs

//	RMuint32  	tpfreq;
//	RMuint32  	lnbfreq;
    RMuint32  	symbolrate;
	RMuint8  	standard;
	RMuint32	uiFreq;
	RMuint16	QAM;
//	RMuint8 	dvbsStandard;

	RMuint16	lastChanIdx;
	RMuint16 	firstChanIdx;

#ifdef DTV_SMK_CAS_SUPPORT
	RMuint16	ecmPid;
	RMuint16 	emmPid;
#endif //DTV_SMK_CAS_SUPPORT

}SProgSetInfo;


typedef struct  {
	RMuint32 	frequency;
	RMuint32	bandWidth;
	RMuint32 	tpfreq;
	RMuint32  	lnbfreq;
	RMuint32  	symbolrate;
	RMuint16  	QAM;
	RMuint8  	standard;
//	RMuint8 	dvbsStandard;

	RMuint16 	pidPMT;
	RMuint16	pidAudio;
	RMuint16	streamTypeAudio;
	RMuint16 	pidVideo;//13 bits
	RMuint16	streamTypeVideo;
	RMuint16	pidPCR;
}SDtvPlayParam;

typedef struct {
	RMuint16 uiChanIdx;			
	RMuint16 uiProgIdx;
	const char* outputFilePath;
	RMuint32 size_file;  		// size file record : ext3 size = 16G 
}SDtvPlayDVRParam;

typedef struct {
	RMuint32 frequency;
	RMuint16 chanIdx;
	SChannelInfo channelInfoList[MAX_CHANNELS];
	RMuint8 numberOfChannels;
}SStreamInfoDtv;


typedef struct  {
	RMuint8 numberOfStreams;
	SStreamInfoDtv streams[MAX_STREAMS_DTV];
}SDtvScanResult;

typedef struct 
{
	void*	pThiz;
	int		dtvIdx;
}thread_proc_arg;

#ifdef WITH_PVR
typedef struct {
	RMuint16 	handleID;
	RMuint16 	programNumber;
	char	 	day[25];
	char	 	startTime[25];
	char	 	endTime[25];
	char 		path[25];
	RMuint16	eventID;
	RMuint8		scheduleTpye;
	RMuint8		scheduleMode;
	RMuint16	status;
	RMbool		isAlert;
	RMbool		isTimeToRecord;
	RMbool		isCheckUSB;

}SPvrScheduleInfo;

enum PvrCallbackEvent
{
	CHECK_USB_HDD = 0,
	RECORD_ALERT,
	RECORDING,
	RECORD_DONE,
	RECORD_FAILED,
	PLAY_ALERT,
};

enum PvrScheduleType
{
	SCHEDULE_TYPE_PLAY = 0,
	SCHEDULE_TYPE_RECORD,
};

enum PvrScheduleMODE
{
	SCHEDULE_MODE_ONCE = 0,
	SCHEDULE_MODE_DAILY,
};

enum EPGMode
{
	EPG_MODE_UNKNOWN = 0,
	EPG_MODE_GETTING_PRESENT_EVENTS,
	EPG_MODE_GETTING_ALL_EVENTS,
	EPG_MODE_GETTING_CURRENT_CHANNEL,
};

#endif

typedef struct {
	ChannelInfo rgMasterChan[BCAST_LIST_SIZE];
	RMuint16 firstChanIdx;
	RMuint16 lastChanIdx;
    RMuint16 numChan;

	RMuint16 curChanIdx;
	RMuint16 curProgIdx;
	RMuint16 curVidIdx ;
	RMuint16 curAudIdx;
	
	RMuint16	uiSIState;
	RMuint16	uiEPGMode;	
	void*		dtv_epg_thread_id;
	RMbool		isStopGetEpg;
	RMbool		isStartGetEpg;
	RMbool		isScanningEpg;
	RMuint8		auiEitScheStatus[MAX_SUBCHANNEL_NUMBER];//for tracking when scan eit schedule
	
	RMuint16	uiPmtStatus; 
	thread_proc_arg thread_arg;

	//  Scanning ...
	void*			dtv_scan_thread_id;
	SDtvScanParam	dtvScanParam;
	RMbool			isStopScan;
	RMbool			isScanning;
    RMuint32	    curFreqScan; 

	//  Playing ...
	void*			dtv_play_thread_id;
	SDtvPlayParam	dtvPlayParam;
	RMbool			isPlaying;
	RMbool			isStopPlay;

	RMuint32		tunerType;
	RMuint32		tunerMode;
	RMuint32		frequency;
	RMuint8			signalStrength;
	RMuint8			signalQuality;
	RMbool			isLocked;
	RMuint8			uiBandwidth;	

	//dvbc
	RMuint32		symbolRate;	
	RMuint16		QAM;		

	// Epg
	STOTInfo		sTot;
	SNITInfo    	sNit;
	//MGTInfo			sMgt;
	MGTInfo			sMgt[ATSC_BCAST_LIST_SIZE];
	SSTTInfo		sStt;
	RMuint16		numNit;
	RMbool   		isGetNit;
	
    // Subtitle
    RMbool          isStartGetSubtitle;
    void*           dtv_subtitle_thread_id;

    //display subtitle
    void*           dtv_display_stt_thread_id;

#ifdef DTV_SMK_CAS_SUPPORT
	RMbool			isStopGetCAKey;
	RMbool			m_isOpeningCACallback;
#endif //DTV_SMK_CAS_SUPPORT

	//
	int				ampIndex;
	int				dtvIndex;

	SSectionFilterStatus 	aFilterStatus;
	RMuint8 		numChannelNeedToAdd;

#ifdef WITH_PVR
	// Pvr
	SPvrScheduleInfo	sPvrScheduleInfo[10];
	SPvrScheduleInfo	sPvrRecordingInfo[10];
	RMuint16			uiNumScheduledPvrs;
	RMuint16			uiNumRecordingPvrs;
#endif
	RMuint8				signalStatus;

}SDFBDtvPlayerData;





/**************************************************************************************
 Time
 **************************************************************************************/
#define MAX_TIME_ZONE	74
#define ADD				0
#define MINUS			1
#define MAXBUFSIZE 1024

struct YearMontDayWeek_dtv{
	RMuint16 day;				// [1, 31] , the day of the month, in the range 1 to 31
	RMuint16 month;				//*[1, 12] , the number of months since january, in the range 1 to 12.
	RMuint16 year;				// [1900, ..] , the number of years, since 1900.
	RMuint8  week_day;			// [0, 6] = [SUN, SAT] ,  the number of week days since sunday, in the range 0 to 6.
};

struct HourMinuteSecond_dtv 
{
	//RMuint8 day;
	RMuint8 hour;				// [0, 24) the number of hours, in the range 0 to 13.
	RMuint8 minute;				// [0, 60) the number of minutes, in the range 0 to 59.
	RMuint8 second;				// [0, 60) the number of seconds, in the range 0 to 59.
};

struct DayTime_dtv{
	struct YearMontDayWeek_dtv ymdw;
	struct HourMinuteSecond_dtv hms;
};

struct TimeZone_dtv
{	
	RMuint8		hour;	
	RMuint8		minute;
	RMuint8		status;		// ADD or MINUS
	RMuint8     daylight;	// Automatically adjust clock for daylight saving changes. 
							// Base on Time Zone on PC (Not use now)
	RMascii		acSTimeZoneName[128];
};


/**************************************************************************************
 Others
 **************************************************************************************/
typedef struct {
	RMbool	isOpen;
	int		ampIndex;
	RMuint32 currentFrequency;
}SAmpInfo;

//for ATSC
/* _remove_
typedef struct
{
	RMuint16	patTsId;
	RMuint8		patVer;
	RMuint8		pmtProg;
	RMuint8		pmtVer;
	RMuint8		mgtNumTbl;
	RMuint8		mgtVer;
	RMuint16	vctTsId;
	RMuint8		vctVer;
	
	//EIT
	RMuint8		eitTableId;
	RMuint8		eitVer;
	RMuint16	eitSrcId;
	RMuint16	eitTsId;
	RMuint16	eitProgramNumber;
	
	//SDT
	RMuint16	sdtTsId;
	RMuint8		sdtVer;	
	
	//khanhngo add
	RMuint16	rrtVer;
	RMuint16	tvctVer;
	RMuint16	ettVer[6];
	RMuint16	sttVer;
}S_PREV; // previous values to take care of duplicate callbacks
*/
enum AudioDtv
{
    AUD_MUTE,				
    AUD_MIXER_VOLUME_MAIN ,
    AUD_LEFT,
    AUD_RIGHT,
};

enum ChannelChanging
{
	NO_CHANGE,
    CHANGE_LCN,
    CHANGE_NAME,
    CHANGE_LCN_AND_NAME,
    CHANGE_ALL,
};

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

#endif  // #ifndef _CDEFS_DTV_H_
