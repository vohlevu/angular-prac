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
#include "dtvcontroller.h"
// This is the JSON header
#include "json/json.h"
#include "curl/curl.h"

struct FtpFile {
	const char *filename;
	FILE *stream;
};
size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream);
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
	/*char time[20];
	strcpy(time, "23/34/6/11/2007");*/
	char buffer [80];
	struct tm timeinfo;
	m_pCTimer->GetSystemTime(&timeinfo);
	strftime (buffer,80,"Now it's %y/%m/%d.",&timeinfo);
	printf("\n================CDtvCtrl::getTime=========================== %s\n\n", buffer);
	return buffer;
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