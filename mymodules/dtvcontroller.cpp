/******************************************************************************
*
* File name		: dtvcontroller.cpp
* Copyright (c) :
* Created Date	: 21 - Sep - 2010
* Author		: QuyenLu
* Purpose		: 
* Note			: 
*
******************************************************************************/
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "dtvcontroller_define.h"
#include "dtvcontroller.h"
/******************************************************************************
* FUNCTION:
*		CDtvCtrl::CDtvCtrl()
*  DESCRIPTION:
*
* @PARAM:
* @RETURN:
* NOTE:
******************************************************************************/
CDtvCtrl::CDtvCtrl()
{
	printf("CDtvCtrl::CDtvCtrl \n");
	if (!m_pCTimer) {
		m_pCTimer = new CTimer();
	}
	char sqlQuery[300];
	if (!m_pEpgDatabase) {
		m_pEpgDatabase 	= new CDatabase();
	}
	if (m_pEpgDatabase) {
		m_pEpgDatabase->OpenDatabase((char*)"../userdata/epg.db", 0);
		strcpy(sqlQuery, (char*)"CREATE TABLE IF NOT EXISTS  EventEPGDetail (ChannelID, EventID, StartTime, EndTime, Title, Description, Language)");
		m_pEpgDatabase->CreateNewTable(sqlQuery);
	}
	GetTimeZone("GMT+7", &tz);
	m_uiTunerNum = MAX_TUNER_NUM;
}

/******************************************************************************
* FUNCTION:
*  CDtvCtrl::~CDtvCtrl()
*  DESCRIPTION:
*
* @PARAM:
* @RETURN:
* NOTE:
******************************************************************************/
CDtvCtrl::~CDtvCtrl()
{
	printf("\n\n\n\n\n \033[31m   ======================~CDtvCtrl()=====================================  \n\033[0m");
	DeInit();
}

/******************************************************************************
* FUNCTION:
*  		Init()
* DESCRIPTION:
*		Init Dtv Core
* @PARAM:
* 		None
* @RETURN:
*  		int	:  1 : if success
*  					   1 : if fail
* NOTE:
******************************************************************************/
int CDtvCtrl::Init()
{
	printf(">>>>>>>>>>>>>> CDtvCtrl::Init<<<<<<<<<<<<<<<<< \n");
	return 1;
}

/******************************************************************************
* FUNCTION: 
*  		DeInit()
* DESCRIPTION:
*		Terminate Dtv
* @PARAM: 
* 		None 
* @RETURN: 
*  		int	:  1 : if success
*  					   1 : if fail
* NOTE: 
******************************************************************************/
int CDtvCtrl::DeInit()
{
	printf("\n\n\n\n=========================~DeInit()================================");
	int err = 1;
	return err;
}

/******************************************************************************
* FUNCTION: 
*  		void SetDtvSysInfoParams(SDtvParams*	dtvParams)
* DESCRIPTION:
*		Transmit dtv data from system info to DtvCtrl class 	
* @PARAM: 
* 		SDtvParams*	dtvParams : a pointer to dtv data in system info
* @RETURN: 
* NOTE: 
******************************************************************************/
void CDtvCtrl::SetDtvSysInfoParams(SDtvParams*	dtvParams)
{
	s_pSysInfoDtvParams = dtvParams;
}

/******************************************************************************
* FUNCTION: 
*  		RMstatus ResetDtvSystemInfo(RMuint8 uiTunerID)
* DESCRIPTION:
*		Reset Dtv System Info
* @PARAM: 
*		RMuint8 uiTunerID: Identify tuner that need use
* @RETURN: 
*		RMstatus    :  RM_OK : if success
*                      RM_ERROR: fail	 
* NOTE: 
******************************************************************************/
RMstatus CDtvCtrl::ResetDtvSystemInfo(RMuint8 uiTunerID)
{
	SDChanInfo*	s_pSysInfoChan = &s_pSysInfoDtvParams->sChanInfo[uiTunerID];
	memset(s_pSysInfoChan->aVideoChansIndex,0,sizeof(s_pSysInfoChan->aVideoChansIndex));
	memset(s_pSysInfoChan->aRadioChansIndex,0,sizeof(s_pSysInfoChan->aRadioChansIndex));
	memset(s_pSysInfoChan->astDChannel,0,sizeof(s_pSysInfoChan->astDChannel));
	//memset (s_pSysInfoDtvParams,0,sizeof(SDtvParams));
	 
	s_pSysInfoChan->numDChannel 		= 0;
	s_pSysInfoChan->currentDChannel 	= 0;
	s_pSysInfoChan->videoChanNum		= 0;
	s_pSysInfoChan->scan_type			= 0;
	s_pSysInfoChan->lastChanIdx			= 0;
	s_pSysInfoChan->firstChanIdx		= 0;
	s_pSysInfoChan->statusSubLang		= -1;
	s_pSysInfoChan->parentalCtrlOnOff 	= PARENTAL_CONTROL_OFF; 
	s_pSysInfoChan->parentalCtrlAge		= RATING_18;
	
	s_pSysInfoChan->sAutoScanParams.uiBandwidth  	= DEFAULT_BANDWIDTH;
	s_pSysInfoChan->sAutoScanParams.uiStartFreq     = DEFAULT_START_FREQ;
	s_pSysInfoChan->sAutoScanParams.uiEndFreq    	= DEFAULT_END_FREQ;
	s_pSysInfoChan->sAutoScanParams.uiNitFreq    	= DEFAULT_NIT_FREQ;
	s_pSysInfoChan->sAutoScanParams.uiModulation    = DEFAULT_MODULATION;
	s_pSysInfoChan->sAutoScanParams.uiSymbolRate 	= DEFAULT_SYMBOLRATE;

	s_pSysInfoChan->scan_type			= DEFAULT_SCAN_TYPE;
	s_pSysInfoChan->dvbcFreq			= DEFAULT_NIT_FREQ;//DEFAULT_START_FREQ;
	s_pSysInfoChan->dvbcQAM				= DEFAULT_MODULATION;
	s_pSysInfoChan->dvbcSymbolRate		        = DEFAULT_SYMBOLRATE;
	s_pSysInfoChan->resetChannel		        = DEFAULT_RESET_CHANNEL;

	return RM_OK;
}

/******************************************************************************
* FUNCTION: 
*  		RMstatus SetDtvStatus(RMuint8 uiDtvStatus)
*
* DESCRIPTION:
*		 Set General Dtv Status
* @PARAM: 
*		RMuint8 uiDtvStatus
*
* @RETURN: 
*		
* NOTE: 
******************************************************************************/
RMstatus CDtvCtrl::SetDtvStatus(RMuint8 uiTunerID, RMuint8 uiDtvStatus)
{
    if (uiTunerID >= m_uiTunerNum)
		return RM_ERROR;

	m_aDtvStatus[uiTunerID] = uiDtvStatus;
	return RM_OK;
}
/******************************************************************************
* FUNCTION: 
*  		RMstatus SwitchToDtvPort(RMuint8 uiTunerID)
* DESCRIPTION:
*		Switch tunerID
* @PARAM: 
*		RMuint8 uiTunerID: tunerID needs switch
* @RETURN: 
*		
* NOTE: 
******************************************************************************/
RMstatus CDtvCtrl::SwitchToDtvPort(RMuint8 uiTunerID)
{
	if (uiTunerID >= m_uiTunerNum)
		return RM_ERROR;
	m_uiCurTunerID = uiTunerID;
	return RM_OK;
}
/******************************************************************************
* FUNCTION: 
*  		RMstatus RestoreChannelInfo(RMuint8 uiTunerID)
* DESCRIPTION:
*		Restore Channel Info
* @PARAM: 
*		RMuint8 uiTunerID: Identify tuner that need use
* @RETURN: 
*		RMstatus    :  RM_OK : if success
*                      RM_ERROR: fail	
* NOTE: 
******************************************************************************/
RMstatus CDtvCtrl::RestoreChannelInfo(RMuint8 uiTunerID, RMbool isInit)
{
	RMstatus err = RM_OK;
	SChanSetInfo	 chanInfo;
	RMuint16 		 oldChanelIdx = 1000; 
	SChanParam	 	 sChanParam;
	SProgParam   	 sProgParam;
	SProgSetInfo 	 progInfo;

	printf("\n>>>>>>>>>>>>>>>>>> RestoreChannelInfo!!! >>>>>>>>>>>>>>>>>>\n");

	SDChanInfo*	s_pSysInfoChan = &s_pSysInfoDtvParams->sChanInfo[m_uiCurTunerID];
	
	SetDtvStatus(uiTunerID, DTV_STATUS_RESTORE_CHAN_INFO_DOING);
	
	if (isInit) {
		for (RMuint16 i = 0; i < s_pSysInfoChan->numDChannel ;i++)
		{
			if (s_pSysInfoChan->astDChannel[i].chanIdx != oldChanelIdx)
			{
				sChanParam.chanIdx  = s_pSysInfoChan->astDChannel[i].chanIdx;
				chanInfo.numProgram = s_pSysInfoChan->astDChannel[i].numProgram;
				chanInfo.transport_stream_id	= s_pSysInfoChan->astDChannel[i].transport_stream_id;
				/*if (m_pDtvPlayer->SetChanInfo(m_aDtvIndex[uiTunerID], &sChanParam, &chanInfo) != RM_OK) {
						printf("CDtvApp::RestoreChannelInfo: Can not SetChanInfo \n");
						return RM_ERROR;
				}*/
				oldChanelIdx = s_pSysInfoChan->astDChannel[i].chanIdx;
			}
		}
	}
	//Nit
	memcpy(&progInfo.sNit, &s_pSysInfoChan->sNit, sizeof(SNITInfo));
	memcpy(&progInfo.sSdt, &s_pSysInfoChan->sSdt, sizeof(SSDTInfo));

	for (RMuint16 i = 0; i < s_pSysInfoChan->numDChannel ;i++)
	{
		progInfo.uiFreq			= s_pSysInfoChan->astDChannel[i].uiFreq;
		progInfo.standard		= s_pSysInfoChan->astDChannel[i].standard;
		progInfo.symbolrate		= s_pSysInfoChan->astDChannel[i].symbolrate;
		progInfo.QAM			= s_pSysInfoChan->astDChannel[i].DVBCQAM;
		progInfo.logChanNum		= s_pSysInfoChan->astDChannel[i].logChanNum;//lcn

		//Logical Channel Number
		progInfo.fav		= s_pSysInfoChan->astDChannel[i].uiFavor;
		progInfo.lock		= s_pSysInfoChan->astDChannel[i].uiLock;
		progInfo.skip		= s_pSysInfoChan->astDChannel[i].uiSkip;

		//teletext
		progInfo.numTeletextEs	= s_pSysInfoChan->astDChannel[i].numTeletextEs;
		memcpy(progInfo.rgTeletextEs, s_pSysInfoChan->astDChannel[i].rgTeletextEs, sizeof(progInfo.rgTeletextEs));
		
		//subtitle
		progInfo.numSubtitleEs	= s_pSysInfoChan->astDChannel[i].numSubtitleEs;
		memcpy(progInfo.rgSubtitleEs, s_pSysInfoChan->astDChannel[i].rgSubtitleEs, sizeof(progInfo.rgSubtitleEs));
		
		//subtitle language
		progInfo.numSubLang	= s_pSysInfoChan->astDChannel[i].numSubLang;
		memcpy(progInfo.sttLang, s_pSysInfoChan->astDChannel[i].sttLang, sizeof(progInfo.sttLang));

		//video
		progInfo.numVidEs		= s_pSysInfoChan->astDChannel[i].numVidEs;
		memcpy(progInfo.rgVidEs, s_pSysInfoChan->astDChannel[i].rgVidEs, sizeof(progInfo.rgVidEs));
		
		//audio
		progInfo.numAudEs		= s_pSysInfoChan->astDChannel[i].numAudEs;
		memcpy(progInfo.rgAudEs, s_pSysInfoChan->astDChannel[i].rgAudEs, sizeof(progInfo.rgAudEs));
		
		//pcr
		progInfo.pcr_pid		= s_pSysInfoChan->astDChannel[i].pcr_pid;
		
		//Name
		memset(progInfo.short_name, 0, sizeof(progInfo.short_name));
		for (RMuint16 j = 0; j < DIGITAL_CHANNEL_NAME_LENGTH; j ++) {
			progInfo.short_name[j] = s_pSysInfoChan->astDChannel[i].acName[j];
		}
		progInfo.short_name[DIGITAL_CHANNEL_NAME_LENGTH] = 0;	

		//parental 
		memcpy(&progInfo.rgParentalRating, &s_pSysInfoChan->astDChannel[i].rgParentalRating, sizeof(progInfo.rgParentalRating));

		progInfo.major_channel_number	= s_pSysInfoChan->astDChannel[i].uiMajor;
		progInfo.minor_channel_number	= s_pSysInfoChan->astDChannel[i].uiMinor;
		progInfo.program_number			= s_pSysInfoChan->astDChannel[i].program_number;
		progInfo.free_CA_mode			= s_pSysInfoChan->astDChannel[i].free_CA_mode; 
		progInfo.program_map_PID		= s_pSysInfoChan->astDChannel[i].program_map_PID; 
		progInfo.pmt_index				= s_pSysInfoChan->astDChannel[i].pmt_index;
		progInfo.firstChanIdx 			= s_pSysInfoChan->firstChanIdx; 
		progInfo.lastChanIdx  			= s_pSysInfoChan->lastChanIdx; 
		progInfo.source_id				= s_pSysInfoChan->astDChannel[i].source_id; 

		sProgParam.chanIdx				= s_pSysInfoChan->astDChannel[i].chanIdx;
		sProgParam.progIdx				= s_pSysInfoChan->astDChannel[i].progIdx;


		/*if (m_pDtvPlayer->SetProgInfo(m_aDtvIndex[uiTunerID], &sProgParam, &progInfo) != RM_OK)
		{
			DERROR("RestoreChannelInfo: Can not SetProgInfo \n");
			err = RM_ERROR;
		}*/
	}

	if(s_pSysInfoChan->numDChannel > 0) {
#ifdef AUTO_UPDATE_CHANNEL

#else
		printf("s_pSysInfoChan->currentDChannel = %d\n", s_pSysInfoChan->currentDChannel);
		//m_pDtvPlayer->Play(m_aDtvIndex[uiTunerID]);
		//SetDChannel(m_uiCurTunerID, s_pSysInfoChan->currentDChannel);
		SetDtvStatus(uiTunerID, DTV_STATUS_RESTORE_CHAN_INFO_DONE);
#endif
	}
	return err;
}

char* CDtvCtrl::GetTime() {
	/*char time[DATETIME_SLEN];
	strcpy(time, "23/34/6/11/2007");*/
	char buffer [DATETIME_SLEN];
	struct tm timeinfo;
	m_pCTimer->GetSystemTime(timeinfo);
	//printf("%04d.%02d.%02d %02d:%02d:%02d\n",1900+timeinfo.tm_year,1+timeinfo.tm_mon,timeinfo.tm_mday,timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
	strftime (buffer,DATETIME_SLEN,"%H/%M/%d/%m/%Y",&timeinfo);
	printf("\n================CDtvCtrl::getTime=========================== %s\n\n", buffer);
	return buffer;
}

/******************************************************************************
* FUNCTION: 
*  		char* GetChannelNameById(RMuint8 iDChannel)
* DESCRIPTION:
*		Get name of channel by id
* @PARAM: 
*		
* @RETURN: 
*		Name of all channel
* NOTE: 
******************************************************************************/
char* CDtvCtrl::GetChannelNameById(RMuint8 iDChannel)
{
    SDChanInfo*	s_pSysInfoChan = &s_pSysInfoDtvParams->sChanInfo[m_uiCurTunerID];
    return (char*)s_pSysInfoChan->astDChannel[iDChannel].acName;
}

/******************************************************************************
* FUNCTION: 
*  		RMuint16 GetTotalChannel()
* DESCRIPTION:
*		Get total number of channel
* @PARAM: 
*		
* @RETURN: 
*		Total number of channel
* NOTE: 
******************************************************************************/
RMuint16 CDtvCtrl::GetTotalChannel()
{
    SDChanInfo*	s_pSysInfoChan = &s_pSysInfoDtvParams->sChanInfo[m_uiCurTunerID];
    return s_pSysInfoChan->numDChannel;
}

char* CDtvCtrl::GetUnixTime() {
	char buffer [DATETIME_SLEN];
	struct tm timeinfo;
	m_pCTimer->GetSystemUnixTime(timeinfo);
	strftime (buffer,DATETIME_SLEN,"%H/%M/%d/%m/%Y",&timeinfo);
	printf("\n================CDtvCtrl::getTime=========================== %s\n\n", buffer);
	return buffer;
}

string CDtvCtrl::CreateEPGInfoString(char* channelId)
{
	string strEPGInfo = "";
	char s[100] = "";
	char utf8[300];
	char **table;
	char *err_msg;
	int i, rows = 0, columns = 0;
	RMstatus status;
	DayTime_dtv stStartTime, stEndTime, stTempTime;
	char acTime[TIME_SLEN], acDesGeneral[DES_GENERAL_SLEN], textUtf8[EXT_TEXT_SLEN];
	RMuint16 totalEvent = 0;
	RMbool	statusPvr = false;
	struct tm timeinfo;
	m_pCTimer->GetSystemTime(timeinfo);		// Get current time
	// Testing
	timeinfo.tm_year = 107;		// 2007
	timeinfo.tm_mon = 10;		// Nov
	timeinfo.tm_mday = 6;		// 6
	//PrintDayTime("PrintDayTime", stStartTime);
	
	ConvertTmToDaytimeDtv(timeinfo, &stStartTime);
	stStartTime.hms.second = 59;
	RMuint32 currentTime = GetNumSecondFromDefaultDate(stStartTime);
	if (tz.status == ADD)
	{
		currentTime -= UTCTOGPS + tz.hour*60*60 + tz.minute*60;
	}
	else //tz.status == MINUS)
	{
		currentTime -= UTCTOGPS - tz.hour*60*60 - tz.minute*60;
	}
	sprintf(utf8,
			"select EventID, StartTime, EndTime, Title, Description from EventEPGDetail where ChannelID = '%s' AND StartTime + EndTime > %ld order by StartTime",
			channelId,
			currentTime);
	status = m_pEpgDatabase->ExecuteQuery((char*)utf8, &table, &rows, &columns, &err_msg);
	// check status

	strcpy(s, "Tue Nov 6, 2007  11:39 PM|||2|||11/07/2007|||0|||0|||1|||0|||");
	strEPGInfo += s;
	
	for (i = 1; i <= rows; i++) {
		stEventEPG[i-1].eventID = atoi(table[i*columns]);
		stEventEPG[i-1].startTime = _atoi64(table[i*columns + 1]);
		stEventEPG[i-1].duration = _atoi64(table[i*columns + 2]);
		strcpy(stEventEPG[i-1].title, table[i*columns + 3]);
		strcpy(stEventEPG[i-1].description, table[i*columns + 4]);
        GPSToUTCDayTime(stEventEPG[i-1].startTime, &stTempTime);
		ChangeDayTimeZone(stTempTime, (TimeZone_dtv&)tz, 0, &stStartTime);

		memset(&stTempTime, 0, sizeof(DayTime_dtv));
		GPSToUTCDayTime(stEventEPG[i-1].startTime + stEventEPG[i-1].duration, &stTempTime);
		ChangeDayTimeZone(stTempTime, (TimeZone_dtv&)tz, 0, &stEndTime);
		GetEventTime(stStartTime, acTime);
		GetEventTime_StartEnd(stStartTime, stEndTime, acDesGeneral);

		//separate member on list
		if(totalEvent > 0)
			strEPGInfo += STRING_DOUBLE_HYPHEN;
		totalEvent++;
		statusPvr = 0;
		sprintf(s, "%d |%s |%s |%d |%s |%s ",
				//acTime,
				statusPvr,
				stEventEPG[i-1].title,
				acDesGeneral,
				stEventEPG[i-1].eventID,
				true? "HD": "SD",
				stEventEPG[i-1].description);

		ConvertISO8859ToUTF8((unsigned char*)s, textUtf8);
		strEPGInfo += textUtf8;
	}
	strEPGInfo += " ";
	strEPGInfo += STRING_TRIO_HYPHEN;
	ConvertUintToString(totalEvent, s);

	strEPGInfo += s;
	strEPGInfo += STRING_TRIO_HYPHEN;
	free(stEventEPG);
	sqlite3_free_table(table);
    //cout << "strEPGInfo = " << strEPGInfo << "\n" ;
	return strEPGInfo;
}

int CDtvCtrl::TestJson()
{
	string json_example = "{\"array\": \
								[\"item1\", \
								\"item2\"], \
								\"not an array\": \
								\"asdf\" \
							}";

	// Let's parse it  
	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(json_example, 
									   root, 
									   false);

	if(!parsedSuccess)
	{
		// Report failures and their locations 
		// in the document.
		cout<<"Failed to parse JSON"<<endl 
			<<reader.getFormatedErrorMessages()
			<<endl;
		return 1;
	}

	// Let's extract the array contained 
	// in the root object
	const Json::Value array = root["array"];

	// Iterate over sequence elements and 
	// print its values
	for(unsigned int index=0; index<array.size(); 
		++index)  
	{  
		cout<<"Element " 
			<<index 
			<<" in array: "
			<<array[index].asString()
			<<endl;
	}

	// Lets extract the not array element 
	// contained in the root object and 
	// print its value
	const Json::Value notAnArray = 
								root["not an array"];

	if(!notAnArray.isNull())
	{
		cout<<"Not an array: "
			<<notAnArray.asString()
			<<endl;
	}

	// If we want to print JSON is as easy as doing:
	cout<<"Json Example pretty print: "
		<<endl<<root.toStyledString()
		<<endl;

	return 0;
}
size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream) {
	struct FtpFile *out = (struct FtpFile *) stream;
	if (out && !out->stream) {
		/* open file for writing */
		out->stream = fopen(out->filename, "wb");
		if (!out->stream)
			return -1; /* failure, can't open file to write */
	}
	return fwrite(buffer, size, nmemb, out->stream);
}
int CDtvCtrl::TestCurl()
{
	CURL *curl;
	CURLcode res;
	struct FtpFile ftpfile = { "node-v0.10.26-x86.msi", /* name to store the file as if succesful */
	NULL };

	curl_global_init (CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if (curl) {
		/*
		 * You better replace the URL with one that works!
		 */
		curl_easy_setopt(curl, CURLOPT_URL,
				"http://nodejs.org/dist/v0.10.26/node-v0.10.26-x86.msi");
		/* Define our callback to get called when there's data to be written */
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
		/* Set a pointer to our struct to pass to the callback */
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);

		/* Switch on full protocol/debug output */
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		res = curl_easy_perform(curl);

		/* always cleanup */
		curl_easy_cleanup(curl);

		if (CURLE_OK != res) {
			/* we failed */
			fprintf(stderr, "curl told us %d\n", res);
		}
	}

	if (ftpfile.stream)
		fclose(ftpfile.stream); /* close the local file */

	curl_global_cleanup();

	return 0;
}