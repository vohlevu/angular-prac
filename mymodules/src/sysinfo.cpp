/******************************************************************************
 File name: 	SysInfo.cpp
 Copyright (c) 2010 KENVI, Corp. All Rights Reserved
 Proprietary and Confidential
 Purpose:		
 Note:			
 Version	1.0
 Date		Jan 15, 2010
 ******************************************************************************/

#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;
#include "sysinfo.h"

#define PATH_LENGTH 256

static char MountPoint[]	= "../userdata";
static char userfilename[]	= "sysinfo.dat";

/**
* 
* <b>FUNCTION: </b>
*
* CSysInfo()
*
* <b>DESCRIPTION: </b>
* 
* Constructor
* 
* @param	No
*		
* @return	No
*
*/
CSysInfo::CSysInfo()
{
	printf("CSysInfo::CSysInfo()\n");
}

/**
* <b>FUNCTION: </b>
*
* ~CSysInfo()
*
* <b>DESCRIPTION: </b>
* 
* Destructor
* 
* @param	No
*		
* @return	No
*
*/
CSysInfo::~CSysInfo()
{
	printf("CSysInfo::~CSysInfo()\n");
}

/**
* <b>FUNCTION: </b>
*
* Savedtv()
*
* <b>DESCRIPTION: </b>
* 
* Savedtv user config into the NAND
* 
* @param	SDtvParams
*		
* @return
*	RM_OK if succes, otherwise RM_ERROR
*
*/
#ifndef NO_DTV_SUPPORT
RMstatus CSysInfo::Savedtv(SDtvParams *p_SDtvParams)
{
	
	RMuint32	uiSize;
	void		*pData;
	char		userfilepath[PATH_LENGTH];

	memset(&userfilepath, 0, PATH_LENGTH);

	sprintf(userfilepath, "%s/%s", MountPoint, userfilename);
    ofstream outconf(userfilepath, ios::out | ios::binary);
	
    if(!outconf) 
	{
		printf("\n\n\n CSysInfo::Savedtv() -> Can not write file\n");
		return RM_ERROR;
	}
	
	pData		= (void*)p_SDtvParams;
	uiSize		= sizeof(SDtvParams);
	
    outconf.write((char *)pData, uiSize);
    
    if(!outconf.good())
	{
		printf("\n\n\n  CSysInfo::Savedtv() -> !outconf.good()\n");
		outconf.close();
		return RM_ERROR;
	}

	outconf.close();
	printf("\n\n\n  CSysInfo::Savedtv() -> OK");
    return RM_OK;
}
#endif
/**
* <b>FUNCTION: </b>
*
* Read()
*
* <b>DESCRIPTION: </b>
* 
* Readdtv user config from the NAND
* 
* @param	SDtvParams
*		
* @return
*	RM_OK if success, otherwise RM_ERROR
*
*/
#ifndef NO_DTV_SUPPORT
RMstatus CSysInfo::Readdtv(SDtvParams *p_SDtvParams)
{
	RMuint32	uiSize;
	void		*pData;
	char		userfilepath[PATH_LENGTH];

	memset(&userfilepath, 0, PATH_LENGTH);

	sprintf(userfilepath, "%s/%s", MountPoint, userfilename);
    ifstream inconf(userfilepath, ios::in | ios::binary);

    if(!inconf)
	{
		printf("CSysInfo::Read() -> Can not open file\n");
        return RM_ERROR;
    }
	pData		= (void*)p_SDtvParams;
	uiSize		= sizeof(SDtvParams);


    inconf.read((char *)pData, uiSize);
    
    if(!inconf.good())
	{
		printf("CSysInfo::Read() -> Read data fail\n");
		inconf.close(); 
		char cmd[sizeof(userfilepath) + 10];
		sprintf(cmd, "rm -rf %s", userfilepath);
		system(cmd);
		return RM_ERROR;
	}
	
	inconf.close();
   printf("\n\n\n  CSysInfo::Readdtv() -> OK");
    return RM_OK;
}
#endif
/**
* <b>FUNCTION: </b>
*
* ResetSysInfo()
*
* <b>DESCRIPTION: </b>
* 
* Reset system info to default
* 
* @param	No
*		
* @return	No
*
*/
void CSysInfo::ResetSysInfo()
{
}

/******************************************************************************
	End																				
******************************************************************************/
