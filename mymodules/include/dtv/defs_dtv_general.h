#ifndef _DTV_DEFINE_H_
#define _DTV_DEFINE_H_


#define MAX_STREAMS_DTV 100
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define	MHZ_TO_HZ				1000000
#define TIMEOUT_1SEC			1000000
#define EMM_TIMEOUT_US		  	(20  * TIMEOUT_1SEC)
#define ECM_TIMEOUT_US		  	(16  * TIMEOUT_1SEC)
#define PAT_TIMEOUT_US		  	(6  * TIMEOUT_1SEC)
#define PMT_TIMEOUT_US		  	(6  * TIMEOUT_1SEC)
#define SDT_TIMEOUT_US			(6	* TIMEOUT_1SEC)
#define TOT_TIMEOUT_US		  	(6  * TIMEOUT_1SEC)
#define NIT_TIMEOUT_US		  	(15 * TIMEOUT_1SEC)
#define EIT_TIMEOUT_US		  	(16 * TIMEOUT_1SEC)
#define ETT_TIMEOUT_US		  	(16 * TIMEOUT_1SEC)
#define RRT_TIMEOUT_US		  	(16 * TIMEOUT_1SEC)
#define TVCT_TIMEOUT_US			(16 * TIMEOUT_1SEC)
#define STT_TIMEOUT_US			(16 * TIMEOUT_1SEC)
#define MGT_TIMEOUT_US		  	(16 * TIMEOUT_1SEC)
#define TTX_TIMEOUT_US		  	(6  * TIMEOUT_1SEC)
#define SDT_TIMEOUT_S			13
#define MAX_SUBCHANNEL_NUMBER		150

///////////////////////////////////////////////////////////////////////////
#define MAX_LANGUAGE_NUMBER		1
#define CHANNEL_SPACING_6		6
#define CHANNEL_SPACING_7		7
#define CHANNEL_SPACING_8		8
#define MAX_SERVICE_NUMBER		50
#define BCAST_LIST_SIZE 		1001
#define ATSC_BCAST_LIST_SIZE 	136
// auto scan params
#define DEFAULT_RESET_CHANNEL 	1
#define DEFAULT_SCAN_TYPE		1
#define DEFAULT_START_FREQ	474
#define DEFAULT_END_FREQ	858
#define DEFAULT_NIT_FREQ	730
#define DEFAULT_BANDWIDTH	8
#define DEFAULT_SYMBOLRATE	6956
#define DEFAULT_MODULATION	3 //64QAM

// OTA setting
#define DEFAULT_OTA_FREQ		730
#define DEFAULT_OTA_MODE		2
#define DEFAULT_OTA_SYMBOLRATE	6956
#define DEFAULT_OTA_MODULATION	3

/*
*	- Values of stream type 0x06 which has descriptor id:
*     + 0x56 (Teletext_descriptor)		-> Teletext stream
*     + 0x6A (AC-3_descriptor)			-> Ac3 audio stream
*     + 0x59 (subtitling_descriptor)	-> Subtile stream
*     + other							-> Private data
*/
enum StreamStatus{
	STREAM_STATUS_NONE		= 0,
	STREAM_STATUS_VIDEO		= 1,
	STREAM_STATUS_AUDIO		= 2,
	STREAM_STATUS_AC3		= 3,
	STREAM_STATUS_TELETEXT	= 4,
	STREAM_STATUS_SUBTITLE	= 5,
	STREAM_STATUS_DTS		= 6,
		
};

/*
// status of epg 
enum STStateEnum  {
SI_STATE_NONE			= 0,
SI_STATE_MPEG2			= 1,
SI_STATE_SDT			= 2,
SI_STATE_EIT_PF			= 3,
SI_STATE_EIT_S			= 4,
SI_STATE_EIT_PF_MON		= 5,
SI_STATE_FINISH			= 6,
};
*/
// status of epg 
enum STStateEnum  {
	SI_STATE_NONE			= 0,
	SI_STATE_MPEG2,			
//dvb
	SI_STATE_SDT,
	SI_STATE_EIT_PF,
	SI_STATE_EIT_S,
	SI_STATE_EIT_PF_MON,
//atsc
	SI_STATE_MGT,			
	SI_STATE_TVCT,			
	SI_STATE_STT,			
	SI_STATE_RRT,			
	SI_STATE_EIT,			
	SI_STATE_ETT,				
	SI_STATE_MGT_MON,		
	SI_STATE_FINISH,			
};

enum ChannelStatus
{
	CHANNEL_FIND_FOUND = 0,
	CHANNEL_FIND_NOT_FOUND,
	CHANNEL_FIND_SKIP,
	CHANNEL_INSERT_OK,
	CHANNEL_INSERT_EXIST,
	CHANNEL_INSERT_FULL,
};

#define SI_MON_DISABLE				0
#define SI_MON_ENABLE				1
#define SI_MON_CHANGE				2

#define EPG_STATUS_NONE				0
#define EPG_STATUS_EIT				1
#define EPG_STATUS_ETT				2
#define EPG_STATUS_FINISH			3

#define NUM_SEC_PER_SEG				8

#define MAX_SUBCHANNEL_NUMBER		150
#define MAX_TRANSPORT_STREAM_ID		160
#define MAX_EIT_NUMBER				500 //(2+(8*NUM_SEG))	//Eits(prensend+schedule) + Eits(schedule for 7 days)
#define MAX_EVENT_NUMBER			16
#define MAX_ES_NUMBER				5

#define TITLE_LEN					(1*256)
#define EXTEND_LEN					(5*256)
#define EXTEND_DES_LEN				255

#define SHORT_DES_LEN				254//64
#define SHORT_NAME_LENGTH			70

#define MAX_COMPONENT_DES			5

#define ATSC_MAX_EIT_NUMBER			4
#define ATSC_MAX_ETT_NUMBER			4
#define ATSC_MAX_RAW_TIME			100
#define ATSC_RATING_NAME_LEN		64



#define MAX_AUDIO_STREAMS 5
#define MAX_CHANNELS 10

#define MAX_PROG 20
#define MAX_CHAN 60

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#endif  // _DTV_DEFINE_H_
