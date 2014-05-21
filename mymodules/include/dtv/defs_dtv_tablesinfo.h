#ifndef _DEFS_TABLESINFO_H_
#define _DEFS_TABLESINFO_H_

/**************************************************************************************
 For ATSC
 **************************************************************************************/
typedef struct {
	RMuint8 language[3];
	RMuint8	line_21_field;
	RMuint8	caption_service_number;
	RMuint8 wide_aspect_ratio;	
}CaptionServiceInfo;

typedef struct {
	RMuint8 	version;
	RMbool		isChanged;
	RMbool		isFinishedGetEPG;
	RMuint16 	sectLen;
	RMuint16 	rgEitPid[ATSC_MAX_EIT_NUMBER];		// only retrieve EIT[0] to EIT[3]
	RMuint16 	rgPresentEitPid;					// Present Event Info
	RMuint16 	rgPresentEitIdx;					
	RMuint16 	numEit;								// actual number of EIT[]
	RMuint16 	rgEttPid[ATSC_MAX_ETT_NUMBER];
	RMuint16 	numEtt;
	RMuint16 	tvctPid;
}MGTInfo;

typedef struct{
	RMuint8		gpsUtcOffset;
	RMuint8		dsStatus;
	RMuint8		dsDayOfMonth;
	RMuint8		dsHour;
	RMuint32	rawTime;
	RMuint32	uiClockId;
}SSTTInfo;


/**************************************************************************************
 RRT table
 **************************************************************************************/
typedef struct{
	RMuint8		rating_region;
	RMuint16	dimensions_defined;
	struct
	{
		RMuint8 dimension_name[ATSC_RATING_NAME_LEN];
		RMuint16 values_defined;
		struct  
		{
			RMuint8 rating_value[ATSC_RATING_NAME_LEN];
			RMuint8 abbrev_rating_value[ATSC_RATING_NAME_LEN];
		}astValues[12];
		
	}astDimensions[12];
}RRTInfo;


/**************************************************************************************
 PMT table
 **************************************************************************************/
typedef struct {
	RMuint8		ISO_639_language_code[3];
	RMuint16 	pid;
	RMuint16	composition_page_id;
}STTLang;

typedef struct {
	RMuint8		stream_type;
	RMuint16	elementary_pid;
	RMuint8		ISO639_language[3];	
}SESInfo;

typedef struct {
	RMuint16 program_number;
	RMuint16 program_map_PID;
	RMuint8	 pmt_index;
	RMuint16 transport_stream_id;
	RMuint8	 version_number;

#ifdef DTV_SMK_CAS_SUPPORT
	RMuint16 emmPid;
	RMuint16 ecmPid;
#endif //DTV_SMK_CAS_SUPPORT

} SPMTInfo;



/**************************************************************************************
 EIT table
 **************************************************************************************/

typedef struct {
	//universal properties
	RMuint16	event_id;	
	RMuint8		szTitle[TITLE_LEN];	

	RMuint8		szExtText[EXTEND_LEN];
	RMuint8		uiExtTextLen;

	union {
		// dvbt 
		struct {
		RMuint8		start_time[5];
		RMuint8		duration[3];
		//RMuint8		free_CA_mode;
		//RMuint8		running_status;
		RMuint8		uiParentalRating;
		//RMuint8		szShoText[SHORT_DES_LEN];
		//RMuint8		uiShoTextLen;
		//component descriptor
		//struct 		ComponentDes astComponentDes[MAX_COMPONENT_DES];
		//RMuint8		uiNumComponentDes;
		
		//RMuint8		ISO_639_language_code_Title[3];
		
		//RMuint8		szTitle_ex[MAX_LANGUAGE_NUMBER][TITLE_LEN];
		//RMuint8		ISO_639_language_code_Title_ex[MAX_LANGUAGE_NUMBER+1][3];
		
		//RMuint8		ISO_639_language_code_ExtText[3];
		//RMuint8		szExtText_ex[MAX_LANGUAGE_NUMBER][EXTEND_DES_LEN];
		//RMuint8		ISO_639_language_code_ExtText_ex[MAX_LANGUAGE_NUMBER+1][3];
		}SDvb_t;
		
		//atsc
		struct {
			RMuint32	start_time_atsc;
			RMuint8		ETM_location;
			RMuint32	length_in_seconds;
			RMuint8		uiParentalRating;
			RMuint16	rating_region[16];
			CaptionServiceInfo caption_services[16];
			RMuint8	numCaptionService;
		}SAtsc;
		// ----------------------------------
	} UEpgInfo;

}SEventInfo;


//union SEITInfo
typedef struct
{
	SEventInfo* pEvent;
	RMuint8		numEvent;
	RMuint8		table_pid;
	RMuint16	service_id;
	RMuint8		version_number;
	RMuint8		section_number;
	RMuint8		last_section_number;
	RMuint8		segment_last_section_number;
	RMuint8		last_table_id;	
}SEITInfo;

/**************************************************************************************
 SDT table
 **************************************************************************************/
typedef struct 
{
	RMuint8		short_name[SHORT_NAME_LENGTH];
	RMuint16	program_number;
	RMuint8		version_number;

	RMuint16	major_channel_number;
	RMuint16	minor_channel_number;
	RMuint8		free_CA_mode;
	RMuint16	source_id;

	union {
		//dvb-t
		struct {
		RMuint8		EIT_present_following_flag;
		RMuint8		EIT_schedule_flag;
		RMuint8		EIT_schedule_last_table_id;
		RMuint16	transport_stream_id;
		}SDvb_t;
		
		
		//atsc
		struct {
			RMuint8		hidden, hide_guide;
			RMuint16	channel_TSID;
			RMuint16	source_id;
			RMuint8		service_type;
		}SAtsc;
		//--------------------------------
	} UEpgInfo;
} SVCTInfo;

typedef struct 
{
	SVCTInfo aVct[MAX_SUBCHANNEL_NUMBER];
	RMuint16 numProgram;
	RMuint16 chanIdx;
} SUpdateVCTInfo;
/**************************************************************************************
 NIT table
 **************************************************************************************/
typedef struct {
	RMuint16	network_id;
	RMuint8		section_number;
	RMuint8		last_section_number;
	RMuint8		version_number;
	RMbool		isFinishGetNIT;
	RMbool		isSameNITVersion;
	RMbool		isNeedToUpdateCableDelivery[MAX_TRANSPORT_STREAM_ID];

	//RMuint8	uiNumSatelliteDeliverySystem;
	RMuint8	uiNumCableDeliverySystem;

	struct {
		RMuint16	transport_stream_id;
		RMuint16	original_network_id;
		struct LogicalChannelDes stLogicalChannelDes;

		//struct  SatelliteDeliverySystemDes stSatelliteDeliverySystemDes;
		struct  CableDeliverySystemDes stCableDeliverySystemDes;
		struct  ServiceListDes stServiceListDes;

	}aTransportStream[MAX_TRANSPORT_STREAM_ID];
	RMuint8 uiNumTransportStream;
	RMuint16 uiNumNITService;
}SNITInfo;

/**************************************************************************************
 SDT table
 **************************************************************************************/
typedef struct {
	RMbool		isFinishGetSDT;
	RMbool		isSameSDTVersion;


	struct {
		RMbool		isSameSDTVersion;
		RMuint8		version_number;
		RMuint16	transport_stream_id;
		RMuint8 	uiNumServiceDesInfo;
		struct {
			RMuint8	free_CA_mode;
			RMuint16 service_id;
			RMuint8 service_type;
			RMuint8 service_provider_name_length;
			RMuint8 service_name_length;
			RMuint8 service_provider_name_char[SERVICE_DESCRIPTOR_SERVICE_PROVIDER_NAME_LENGTH + 1];
			RMuint8 service_name_char[SHORT_NAME_LENGTH + 1];
		}aServiceDesInfo[MAX_LOGICAL_CHANNEL_NUMBER];

	}aTransportStream[MAX_TRANSPORT_STREAM_ID];
	RMuint8 uiNumTransportStream;
}SSDTInfo;

/**************************************************************************************
 Tables: TDT & TOT
 **************************************************************************************/

typedef struct {
	RMuint8		UTCTime[5];
}STDTInfo;

typedef struct {
	RMuint8		UTCTime[5];
	RMuint32	uiClockId;
	struct LocalTimeOffsetDes stLocalTimeOffsetDes;
}STOTInfo;

/**************************************************************************************
 General Structures
 **************************************************************************************/

typedef struct {
	// univesal properties of program.
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

	STTLang		sttLang[MAX_ES_NUMBER];  
	RMuint16 	numSubLang;  

	RMuint16 	pcr_pid;	
	RMuint8		pmt_index;
	RMuint16	program_map_PID;	

	RMuint8		short_name[SHORT_NAME_LENGTH];
	RMuint16	program_number;

	SEITInfo	rgEit[MAX_EIT_NUMBER];
	RMuint16 	numEit;

	SEITInfo	rgEtt[MAX_EIT_NUMBER];
	RMuint16 	numEtt;

	RMuint16	major_channel_number;
	RMuint16	minor_channel_number;
	RMuint8		free_CA_mode;
	SParentalRating	rgParentalRating;
	
	RMuint8		version_number;

#ifdef DTV_SMK_CAS_SUPPORT
        RMuint16 	ecmPid;
        RMuint16 	emmPid;
#endif //DTV_SMK_CAS_SUPPORT

	union 
	{
		// dvb-t properties
		struct {
			RMuint16	logChanNum;
			RMuint8		EIT_present_following_flag;
			RMuint8		EIT_schedule_flag;
			RMuint8		EIT_schedule_last_table_id;
		} SDvb_t;
		//Atsc properties
	struct {
		RMuint8 	mgtVer;
		RMuint16 	mgtSectLen;
		RMuint8		hidden;
		RMuint8		hide_guide;
		RMuint16	channel_TSID;
		RMuint16	source_id;
		RMuint8		service_type;
		RRTInfo		sRrt;
		RMuint16	program_map_PID;	
	} SAtsc_t;
	//-----------------------------------------------
	} UEpgInfo;

} SProgramInfo;



typedef struct {
	//PAT
	SPMTInfo* pPmt;
	RMuint16* pNumPmt;
	RMbool isFinishPAT;
	RMbool isParsePAT;
	RMbool isCallPAT;
	//CAT
	RMbool isFinishCAT;

#ifdef DTV_SMK_CAS_SUPPORT
	RMuint16 *pEmmPid;
#endif //DTV_SMK_CAS_SUPPORT

	//PMT
	RMbool isFinishPMT;
	RMbool isParsePMT;
	RMbool isCallPMT;
	SProgramInfo* pProg;

    //Subtitle
    RMbool isFinishSubTitle;

	union {
		
        struct {
		//SDT
		RMbool isFinishSDT;
		RMbool isParseSDT;
		RMbool isCallSDT;
		RMbool isFinishTVCT;
		SVCTInfo* paVct;
		SSDTInfo* pSdt;
		RMuint16* puiNumVChan;
		//NIT
		RMbool isFinishNIT;
		RMbool isParseNIT;
		RMbool isCallNIT;
		SNITInfo* pNit;
		//TOT
		RMbool isFinishTOT;
		STOTInfo* pTot;
		//EMM
		RMbool isFinishEMM;

#ifdef DTV_SMK_CAS_SUPPORT
		RMbool isFinishECM;
#endif //DTV_SMK_CAS_SUPPORT

        //TOT_TDT
		RMbool isFinishTOT_TDT;
		STOTInfo* pTot_Tdt;
            //Exist TOT
            RMbool isTOT;
		//EIT
			RMbool isStartEIT;
		RMbool isFinishEIT;
			RMbool isParseEIT;
			RMbool isCallEIT;
		SEITInfo*	pEit;
		//EIT monitor
		RMbool isFinishEIT_Mon;
		SEITInfo*	pEit_Mon;
        }SDvb_t;

			struct {
			RMbool isTemp;
			RMbool isFinishRrt;
			RRTInfo*	pRrt;
	
			RMbool isFinishMgt;
			MGTInfo* pMgt;
	
			RMbool isFinishStt;
			SSTTInfo* pStt;
	
			RMbool isFinishEIT;
			SEITInfo*	pEit;
	
			RMbool isFinishTVCT;
			SVCTInfo* paVct;
	
			//ETT
			RMbool isFinishETT;
			SEventInfo* pEventInfo;
			RMuint16 uiSourceId;
	
			RMuint16* puiNumVChan;
        }SAtsc_t;

	}EpgInfo;
}SSectionFilterStatus;


#endif  // _DEFS_TABLESINFO_H_
