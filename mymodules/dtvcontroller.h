/******************************************************************************
 
******************************************************************************/

#ifndef __DTV_CONTROLLER__H_
#define __DTV_CONTROLLER__H_

// This is the JSON header
#include "json/json.h"
#include "curl/curl.h"
#include "zenotimer.h"
#include "dtvutility.h"
#include "database.h"

using namespace std;
#define DATETIME_SLEN		20
#define TIME_SLEN			20
#define DES_GENERAL_SLEN	40
#define SHORTTITLE_SLEN		255
#define LANGUAGE_SLEN		10
#define EXT_TEXT_SLEN		3*512

struct FtpFile {
	const char *filename;
	FILE *stream;
};

typedef struct
{
	RMbool		pvrStatus;
	RMuint16	eventID;
	RMuint64	startTime;
	RMuint32	duration;
	char title[SHORTTITLE_SLEN];
	char description[DES_GENERAL_SLEN];
	char language[LANGUAGE_SLEN];
}EventEPGDetail;

size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream);
class CDtvCtrl
{
/********** variables *************/
private:
	TimeZone_dtv tz;
	EventEPGDetail stEventEPG[500];

protected:
	
	CTimer		*m_pCTimer;
	CDatabase	*m_pEpgDatabase;
public :
	CDtvCtrl();
	~CDtvCtrl();

	//init and deinit
	int 			Init();
	int 			DeInit();
	int 			TestJson();
	char*			GetTime();
	char*			GetUnixTime();
	int 			TestCurl();
	string			CreateEPGInfoString(char* channelId);

};

#endif //__DTV_CONTROLLER__H_

/******************************************************************************
	End
******************************************************************************/
