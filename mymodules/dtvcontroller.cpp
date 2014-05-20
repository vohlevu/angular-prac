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