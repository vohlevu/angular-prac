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