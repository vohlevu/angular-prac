/******************************************************************************
 
******************************************************************************/

#ifndef __DTV_CONTROLLER__H_
#define __DTV_CONTROLLER__H_

#include <iostream>
using namespace std;
class CDtvCtrl
{
/********** variables *************/
private:
	int 		 m_uiTunerNum;

protected:
	
	int		m_uiTotalPlayer;

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
