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