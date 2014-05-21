/******************************************************************************
 
******************************************************************************/

#ifndef __DTV_CONTROLLER__H_
#define __DTV_CONTROLLER__H_

#include <iostream>
#include "include/zenotimer.h"
using namespace std;
class CDtvCtrl
{
/********** variables *************/
private:
	

protected:
	
	CTimer		*m_pCTimer;

public :
	CDtvCtrl();
	~CDtvCtrl();

	//init and deinit
	int 			Init();
	int 			DeInit();
	int 			TestJson();
	char*			GetTime();
	int 			TestCurl();

};

#endif //__DTV_CONTROLLER__H_

/******************************************************************************
	End
******************************************************************************/
