/******************************************************************************
 File name: 	dtvdefs_chaninfo.h
 Copyright: 
 Purpose:		
 Note:			
 Version	
 Date		
******************************************************************************/

#ifndef __DTV_DEFS_CHANINFO__H_

#define MAX_DIGITAL_CHANNEL				1000
#define DIGITAL_CHANNEL_NAME_LENGTH		32
#define DIGITAL_MHZ_TO_HZ				1000000 
#define MAX_FAVOR_GROUP 				10

typedef struct SFavorGroup
{
	RMuint8		fvGroupIdx;
	RMuint8		fvGroupStatus;
	RMuint8		fvGroupName[100];
}SFavorGroup;

typedef struct structDigitalChannel	
{
	SESInfo 	rgVidEs[5];
	RMuint16 	numVidEs;
    SESInfo 	rgAudEs[MAX_ES_NUMBER];
	RMuint16 	numAudEs;	
	SESInfo 	rgTeletextEs[5];
	RMuint16 	numTeletextEs;
	SESInfo 	rgSubtitleEs[5];
	RMuint16 	numSubtitleEs;
	
	SParentalRating	rgParentalRating;
		
	STTLang		sttLang[MAX_ES_NUMBER];  
	RMuint16 	numSubLang;  
    RMint16 	curSubLang; 
	
	RMuint16 	pcr_pid;
	RMuint8		acName[DIGITAL_CHANNEL_NAME_LENGTH + 1];
	RMuint8		free_CA_mode; 
	RMuint16	program_map_PID;
	
	RMuint16	progIdx;//streamIndex
	RMuint16	chanIdx;//channelIndex
	RMuint16	audioIdx;
	RMuint16	videoIdx;
	
	RMuint16	numProgram;
	RMuint16 	transport_stream_id;
	RMuint8	 	version_number;
	RMuint16	program_number;
	RMuint16	logChanNum;
	RMuint32	uiFreq;//frequency

	RMuint16	DVBCsymbolRate;
	RMuint16	DVBCQAM;

	RMuint32  	tpfreq;
	RMuint32  	lnbfreq;
    RMuint32  	symbolrate;
	RMuint8  	standard;

	RMuint8		uiMajor;
	RMuint8		uiMinor				:5; // 1 -> ...32
	RMuint8		uiFavor				:1;				
	RMuint8		uiLock				:2;
	RMuint8		uiSkip				:4;
	RMuint8		uiDelete			:1;
    RMuint8		uiMove  			:1;
    RMuint8		uiRename  			:1;
    RMuint8		uiCurrentChannel	:1;
	RMuint8		uiParental;
	RMbool 		bTeletext;			//Teletext
	RMuint8		pmt_index;
	RMuint16 	source_id;
	RMbool		isHD;
	RMbool		isRadio;
	char		videoType[10];
	char		audioType[10];

    RMuint8	 	volume; //volume for single channel

        RMuint8		passwordLock[DIGITAL_CHANNEL_NAME_LENGTH];

#ifdef DTV_SMK_CAS_SUPPORT
	RMuint16 	ecm_pid;
	RMuint16 	emm_pid;
#endif //DTV_SMK_CAS_SUPPORT

    //favor
    SFavorGroup	sFavorGroup[MAX_FAVOR_GROUP];
} DigitalChannel;


#endif //__DTV_DEFS_CHANINFO__H_


/******************************************************************************
	End
******************************************************************************/
