function Epg(oTV){
	var oEpg				= this;
    var oMain      			= null;	
	oEpg.pageID           	= "Epg_page";
	oEpg.pageElement      	= $("<div></div>").attr("id", oEpg.pageID);
	oEpg.homeBehaviorZone	= null;
	
	// variable of ParserData function
	//var _currDatetime;
	var _currMinor;
	var _curChannel              = 1;
	
	var focusEPGDay              = 1;
	var _currWeekDay;
	var _oldCurrWeekDay;
	var _currDayMonth;
	var posWeekDay;
	var WeekDay = new Array("Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday");
	
	var bInitEpg				 = true;
	
	//var countEpgGetTime			 = 0;
	var _executeChannelEpg;	
	var timeIntervalNum          = 1500; // .5 second	
	////////////////////////////////////////
	var mcInterval; //interval update data of epg
	
	var itemCurLeftFocus;				// in screen, there is just one item that is focus
	/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ BASIC FUNCTION ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
	var aChannelListLeft = [];
	var aProgram = [];
	var oVietnamTV = oTV;
	var getTimeInterval; //interval update time
	var dateCurrent;

oEpg.start = function (MainPage){
	var iCurrChannel = oVietnamTV.getCurrChannel();
	if (undefined == iCurrChannel) {
		Authentication.Debug("EPG >> >>> NOT READY");
		setTimeout(function(){
			oEpg.start(main);
		},300);
		return;
	}
	Authentication.Debug("EPG >> >>> Start");
	oMain = MainPage;
	oEpg.makePage();
	
	_currMinor = -1;
	_oldCurrWeekDay = -1;
	_currWeekDay = -1;
	//_currDatetime = -1;
	totalProgram = -1;
	aProgram = [];
	aChannelListLeft = [];
	itemCurRightFocus = undefined;
	//countEpgGetTime = 0;
	bInitEpg            = true;
	oJSExtensions.epg_obj.showEPG();
	
	oEpg.definedVarBoxLeft();
	oEpg.definedVarBoxRight();
	oEpg.definedVarBoxTop();
	dateCurrent = oEpg.parseDateTime(oJSExtensions.epg_obj.getTime());
	oEpg.showHeaderDayTime(dateCurrent);	//set time
	
	oEpg.autoUpdateEPG();
	_executeChannelEpg = setInterval(function(){
		if (!bInitEpg) {
			clearInterval(_executeChannelEpg);
			if (aChannelListLeft.Data[iCurrChannel].isDTVChannel) {
				oJSExtensions.epg_obj.SetWndRect(71, 68, 306, 174);
				oJSExtensions.epg_obj.setEpgGetMode(3);
			}
			// Only set 1 time for OTT channel
			oJSExtensions.mediasetting.setScale(3, 71, 68, 306, 174);
			oEpg.setKeyControlEpg();
		} else {
			Authentication.Debug("EPG >> loop setKeyControlEpg <<<<<<");
		}
	},500);
	
	getTimeInterval = setInterval(function(){
								Authentication.Debug("EPG >> getTimeInterval >> " + dateCurrent.toString());
								dateCurrent.setMinutes(dateCurrent.getMinutes() + 1);
								oEpg.showHeaderDayTime(dateCurrent);	//set time
							},60000);
};

oEpg.makePage = function (){
	//Authentication.Debug("EPG >>  makePage");
	var html =  '<div id="epgWrap">'+
					'<div id="epgBoxLeft">'+
						'<ul id="epgListLeft"></ul>'+
					'</div><!-- end epgBoxLeft -->'+
					'<div id ="epgDayTime">'+
						'<p id ="epgDay"/>'+
						'<p id ="epgTime"/>'+
					'</div>'+
					'<div id="epgInfoTop">'+
						'<p id="epgNameChannel"></p>'+
						'<div id ="epgInfoLine" />'+
						'<p id="epgNameProgram"></p>'+
						'<p id="epgTimeShowProgram"></p>'+
						'<div id ="epgInfoChannel"/>'+
					'</div><!-- end infoTop -->'+
					'<div id="epgBoxDay"></div><!-- end dayOfWeek -->'+
					'<div id="epgBoxRight">'+
						'<ul id="epgListRight"></ul>'+
					'</div><!-- end epgBoxRight -->'+
					'<div id ="epgChannelNarrowUp" />'+
					'<div id ="epgChannelNarrowDown" />'+
					'<div id ="epgProgramNarrowUp" />'+
					'<div id ="epgProgramNarrowDown" />'+
					/*'<div id="epgPvr">'+
						'<img id="epgButtonRed" src="images/ChannelManager/red.png">'+
						'<div id="epgTextButtonRed" >'+ oLanguageSetting.Epg_Pvr_Add +'</div>'+
					'</div>'+*/
					'<div id ="epgGuide">'+
						'<p id ="epgTextNext">next day</p>'+
						'<div id ="epgButtonNext" />'+
						'<div id ="epgButtonPrevious" />'+
						'<p id ="epgTextPrevious">previous day</p>'+
					'</div>'+
					'<div id ="epgProgramDescription">'+
						'<div id="epgProgramDescriptionInfo">'+
							'<p id="epgProgramDescriptionNameChannel"></p>'+
							'<p id="epgProgramDescriptionTimeProgram"></p>'+
							'<div id ="epgProgramDescriptionLine" />'+
							'<p id="epgProgramDescriptionNameProgram"></p>'+
							'<div id ="epgProgramDescriptionInfoChannel"/>'+
							'<p id="epgProgramDescriptionProgram"></p>'+
						'</div>'+
					'</div>'+
				'</div><!-- end EPG -->	'; 
	oEpg.pageElement.append(html);
	oMain.coldPageElement.append(oEpg.pageElement);
};
	
oEpg.removeZone = function(zone){
	oMain.keyController.removeBehaviorZone(zone);
};

oEpg.autoUpdateEPG = function ()
{
	Authentication.Debug("EPG >> autoUpdateEPG");
    oEpg.getEpgData();
	oEpg.showTextEPGMain();  
    clearInterval(mcInterval);
    mcInterval = setInterval(function(){		
					oEpg.loadEPG();
				},5000);
	Authentication.Debug("EPG >> autoUpdateEPG END");
};

oEpg.getEpgData = function()
{
	Authentication.Debug("EPG >> getEpgData");	
	if(bInitEpg == true){
		aChannelListLeft = oVietnamTV.getFavoriteChannels();
		_currMinor = oVietnamTV.getCurrChannel();
		oEpg.setVarBoxLeft();
		if (-1 != _currMinor) {
			_curChannel = _currMinor + 1;
			var iMod 			= parseInt(_curChannel / Def_NumItemsLeftList);
			if (_curChannel % Def_NumItemsLeftList == 0) {
				iMod--;
				iCountInChannelList 	= Def_NumItemsLeftList;
			} else {
				iCountInChannelList 	= _curChannel % Def_NumItemsLeftList;
			}
			iHeadInChannelList 		= iMod* Def_NumItemsLeftList + 1;
			if(iHeadInChannelList + numItemInChannelList - 1 >= totalChannels)
				iRearInChannelList 		= totalChannels;
			else
				iRearInChannelList 		= iHeadInChannelList + numItemInChannelList - 1;
		} else {
			_curChannel = 1;
		}
		oEpg.getNewChannelList(iHeadInChannelList - 1, iRearInChannelList - 1);
		itemCurLeftFocus = Def_EleItemLeftZone.eq(iCountInChannelList - 1).addClass(Def_NameClassFocusLeftItem);
		oEpg.addMarqueeChannelList();
		oEpg.showTopChannelInfo(aChannelListLeft.Data[iHeadInChannelList + iCountInChannelList - 2]);
		
		if (!aChannelListLeft.Data[_currMinor].isDTVChannel) {
			str = oJSExtensions.epg_obj.getInfo();
			oEpg.parseData(str, true);
		}
		bInitEpg = false;
	}
	
    var str; 
	if (!aChannelListLeft.Data[iHeadInChannelList + iCountInChannelList - 2].isDTVChannel) {
		str = " ||| ||| ||| ||| ||| ||| ||| ||| ||| ||| ||| |||0|||";
		// internet channel, no need to interval update dtv epg.
		clearInterval(mcInterval);
		oEpg.parseData(str, false);
	} else {
		str = oJSExtensions.epg_obj.getInfo();
		oEpg.parseData(str, true);
	}
	
	/*if (countEpgGetTime < 10) {
		countEpgGetTime++;
		Authentication.Debug("EPG >> getEpgData countEpgGetTime:"+countEpgGetTime);*/
		if (_oldCurrWeekDay != _currWeekDay) {
			Authentication.Debug("EPG >> getEpgData update day of week >> _currWeekDay:" + _currWeekDay);
			if (_currWeekDay < 1 || _currWeekDay > 7) {	
				_oldCurrWeekDay = _currWeekDay;
				_currWeekDay = 1;
			} else {
				_oldCurrWeekDay = _currWeekDay;
			}
			focusEPGDay = 1;
			posWeekDay = _currWeekDay;
			eleFocusDay.html(WeekDay[posWeekDay - 1] + '<br/><p style="font-size: 18px;">' + _currDayMonth + '</p>');
			//Authentication.Debug("EPG >> getEpgData focusDays = " + focusEPGDay);
			totalProgram = -1; //0 for if (totalProgram != aProgram.length)
		}
	//}
 	Authentication.Debug("EPG >> getEpgData END");
};

oEpg.parseData = function(strInfoEPG, dtv)
{
	//Authentication.Debug("EPG >> parseData: " + strInfoEPG);
    var array_value = strInfoEPG.split("|||");
	if (dtv) {
		//_currDatetime = array_value[0];
		_currWeekDay = Number(array_value[1]);
		_currDayMonth = array_value[2];
		//_currMinor = Number(array_value[3]);
		_currEvent = Number(array_value[4]);
	}

	aProgram = [];
	if (array_value[8] > 0) {	// Total Event
		var _strTotalEvent = new Array();
		var _strOneEvent = new Array();
		_strTotalEvent = String(array_value[7]).split("||");
		for(var j = 0;j<_strTotalEvent.length;j++)
		{
			_strOneEvent = String(_strTotalEvent[j]).split("|");
			aProgram[j] = {
					'id' 	: j,
					'pvrStatus' : _strOneEvent[0],
					'name' 	: _strOneEvent[1],
					'timeShow'	: _strOneEvent[2],
					'eventID'	: _strOneEvent[3],
					'format'	: _strOneEvent[4],
					'description'	: _strOneEvent[5]
			};
		}
	}
	Authentication.Debug("EPG >> parseData END Total Event = " + array_value[8]);
};

oEpg.loadEPG = function(){   
    var i;
    var bCheckUpdate;
    bCheckUpdate = oJSExtensions.epg_obj.checkUpdateEvent();
	Authentication.Debug("EPG >> loadEPG: bCheckUpdate = " + bCheckUpdate);
	oEpg.getEpgData();
	oEpg.showTextEPGMain();
    if (bCheckUpdate == false){
		clearInterval(mcInterval);
    }
};

oEpg.checkInfo  = function(str) {
    if (!str || 0 == str.length) {
        return "No Info";
    } else {
        return String(str);
    }
};

oEpg.showTopChannelInfo = function(eleChannel) {
	if (undefined != eleChannel) {
		Def_EleTopChannelName.html(eleChannel.Name);
		var str = "";
		if (eleChannel.isDTVChannel) {
			var channelIdx = eleChannel.ID;
			var strTemp = oJSExtensions.dtvplayback_obj.getAudioType(channelIdx);
			if (strTemp == "DOLBY")
				str +=	'<div id ="epgDolby" />';
			else
				str +=	'<div id ="epgStereo" />';
			strTemp = oJSExtensions.dtvplayback_obj.getVideoType(channelIdx);
			if (strTemp == "MPEG-1")
				str +=	'<div id ="epgMPEG1" />';
			else if (strTemp == "MPEG-2")
				str +=	'<div id ="epgMPEG2" />';
			else if (strTemp == "MPEG-4")
				str +=	'<div id ="epgMPEG4" />';
			else if (strTemp == "H264")
				str +=	'<div id ="epgH264" />';
			//strTemp = oJSExtensions.dtvplayback_obj.getChannelType(channelIdx);
			//if (strTemp == "HD")
			strTemp = eleChannel.Name;
			var suffix = "HD";
			if (strTemp.indexOf(suffix, strTemp.length - suffix.length) !== -1)
				str +=	'<div id ="epgHD" />';
			else
				str +=	'<div id ="epgSD" />';
		}
		Def_EleTopChannelInfo.html(str);
	}
};

oEpg.showTextEPGMain = function() {
	if (aProgram.length == 0) {
		Def_EleTopProgramName.html("No Info");
		Def_EleTopProgramTime.html("No Info");
	} else {
		var currentId;
		if(undefined != itemCurRightFocus) {
			currentId = itemCurRightFocus.data('id');
		} else {
			currentId = 0;
		}
		Def_EleTopProgramName.html(oEpg.checkInfo(aProgram[0].name));
		Def_EleTopProgramTime.html(oEpg.checkInfo(aProgram[0].timeShow));
	}
	
	Authentication.Debug("EPG >> showTextEPGMain >> totalProgram :" + totalProgram + " >>aProgram.length:" + aProgram.length); 
	if (totalProgram != aProgram.length) {
		oEpg.setVarBoxRight();
		oEpg.getNewProgramList(iHeadInProgramList - 1, iRearInProgramList - 1);
	}
};

///////
oEpg.end = function (){
	//remove interval	
	clearInterval(mcInterval); //interval auto update info
	clearInterval(_executeChannelEpg);
	clearInterval(getTimeInterval);
	clearTimeout(timeoutSetScreenRectangle);
	
	//remove handle
	oEpg.removeZone(oEpg.homeBehaviorZone);
	
	//var iCurrChannel = oVietnamTV.getCurrChannel();
	//if (aChannelListLeft.Data[iCurrChannel].isDTVChannel)
		oJSExtensions.epg_obj.hideEpg();
	oJSExtensions.mediasetting.setScale(3, 0, 0, 1280, 720);
	
	var topObjOfStack = oMain.stackInfoPage.shift();      // get and remove top item of stack
    $("#" + topObjOfStack.pageID).remove();                 // remove element from page html

    $("#" + oMain.stackInfoPage[0].pageID).css({
        opacity : 1,
        display : 'block'
    });
	oMain.stackInfoPage[0].itemObj.comeback();
	Authentication.Debug("EPG >> ======== EXIT ========= EPG ===========END ==========");
};
oEpg.clearAllInterval = function ()
{
	Authentication.Debug("oEpg.clearAllInterval ....");
	//var iCurrChannel = oVietnamTV.getCurrChannel();
	//if (aChannelListLeft.Data[iCurrChannel].isDTVChannel)
		oJSExtensions.epg_obj.hideEpg();
	oJSExtensions.mediasetting.setScale(3, 0, 0, 1280, 720);
	clearInterval(_executeChannelEpg);
	clearInterval(mcInterval);
	clearInterval(getTimeInterval);
	clearTimeout(timeoutSetScreenRectangle);
	oMain.stackInfoPage[oMain.stackInfoPage.length-2].itemObj.clearAllInterval();

};
////////////////// HANDLE /////////////////////////////
//handle main
oEpg.setKeyControlEpg = function(){
	Authentication.Debug("EPG >> >>> setKeyControlEpg");
	var aKeyMapping = new Array();
	{
		aKeyMapping[Key.ENTER] 	= 	function(oldItem, newItem) {
			Authentication.Debug("EPG >> handle main __ key.ENTER");
			oEpg.startBoxRight();
			return { status: 'none' };
		};
		aKeyMapping[Key.RIGHT] 	= 	function(oldItem, newItem) {
			Authentication.Debug("EPG >> handle main __ key.RIGHT");
			oEpg.startBoxRight();
			return { status: 'none' };
		};
		aKeyMapping[Key.TIME_SEEK] 	= 	function(oldItem, newItem) {
			Authentication.Debug("EPG >> handle main __ key.TIME_SEEK");
			oEpg.end();
			return { status: 'none' };

		};
		aKeyMapping[Key.RETURN] = 	function(oldItem, newItem) {
			Authentication.Debug("EPG >> handle main __ key.RETURN");
			oEpg.end();
			return { status: 'none' };
		};
									
		aKeyMapping[Key.UP]  	= 	function(oldItem, newItem) {
			//Authentication.Debug("EPG >> handle main __ key.UP");
			/*var curChannelIdx = parseInt(itemCurLeftFocus.data('id'), 10) - 1;
			if (curChannelIdx == 0 )
				curChannelIdx = aChannelListLeft.length;
			var checkRecording = parseInt(oJSExtensions.pvr_obj.checkRecording());
			var checkSameFreq = parseInt(oJSExtensions.pvr_obj.checkSameFreq(curChannelIdx - 1));
			Authentication.Debug("#----checkRecording-------#" + checkRecording);
			Authentication.Debug("#----checkSameFreq-------#" + checkSameFreq);
			if(checkRecording >= 0 && checkSameFreq != true){
				isUp = true;
				isDown = false;
				oEpg.showNoticeRecord(oLanguageSetting.Dtv_Pvr_Notice_All_Program);
			}
			else {*/
				oEpg.executeUpChannelList();
			//}
			return { status: 'none' };
		};

		aKeyMapping[Key.DOWN] 	= 	function(oldItem, newItem) {
			//Authentication.Debug("EPG >> handle main __ key.DOWN");
			/*var curChannelIdx = parseInt(itemCurLeftFocus.data('id'), 10) + 1;
			if (curChannelIdx == aChannelListLeft.length)
				curChannelIdx = 1;
			var checkRecording = parseInt(oJSExtensions.pvr_obj.checkRecording());
			var checkSameFreq = parseInt(oJSExtensions.pvr_obj.checkSameFreq(curChannelIdx - 1));
			Authentication.Debug("#----checkRecording-------#" + curChannelIdx);
			Authentication.Debug("#----checkRecording-------#" + checkRecording);
			Authentication.Debug("#----checkSameFreq-------#" + checkSameFreq);
			if(checkRecording >= 0 && checkSameFreq != true){
				isUp = false;
				isDown = true;
				oEpg.showNoticeRecord(oLanguageSetting.Dtv_Pvr_Notice_All_Program);
			}
			else {*/
				oEpg.executeDownChannelList();
			//}
			return { status: 'none' };
		};	
	}
	
	var oNavSelectors = {item: '.epgItemListLeft'};
  	var oSelectionClasses = {};
  	var oActions = {};
	
	oEpg.homeBehaviorZone = new gtv.jq.KeyBehaviorZone({
										        containerSelector	: '#epgListLeft',
										        keyMapping			: aKeyMapping,
										        actions 			: oActions,
										        navSelectors 		: oNavSelectors,
										        selectionClasses 	: oSelectionClasses,
										        navigableData 		: 'url',
										        useGeometry 		: false
										      });
  	oMain.keyController.addBehaviorZone(oEpg.homeBehaviorZone, true);
	oMain.keyController.setZone(oEpg.homeBehaviorZone,true);
  	oMain.keyController.start();
};

/*OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
/* ================================================== BOX TOP ========================================= */
var Def_EleTopChannelName;
var Def_EleTopChannelInfo;
var Def_EleTopProgramName;
var Def_EleTopProgramTime;
var Def_EleTopDayTime;
var eleFocusDay;
oEpg.definedVarBoxTop = function(){
	Def_EleTopChannelName			= $('#epgNameChannel');
	Def_EleTopChannelInfo 			= $('#epgInfoChannel');
	Def_EleTopProgramName			= $('#epgNameProgram');
	Def_EleTopProgramTime 			= $('#epgTimeShowProgram');
	Def_EleTopDayTime				= $('#epgDayTime');
	eleFocusDay						= $('#epgBoxDay');
};
/*OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
/* ================================================== BOX LEFT ========================================= */
var Def_NumItemsLeftList	= 5;		// number items that the list can containt
var Def_EleLeftZone;				// element of Zone
var Def_EleItemLeftZone;
var Def_NameClassFocusLeftItem 	= 'epgFocusItemListLeft';		// class name of item when it is focus
var Def_NameClassFocusList 		= 'epgFocusList';
var Def_NameClassDimList 		= 'epgDimList';

var totalChannels;
var numItemInChannelList;
var iHeadInChannelList;
var iRearInChannelList;
var iCountInChannelList;

var setTimeOutMarquee;
oEpg.setVarBoxLeft = function(){
	totalChannels 			= aChannelListLeft.Data.length;
	numItemInChannelList 	= (totalChannels >= Def_NumItemsLeftList) ? Def_NumItemsLeftList : totalChannels;
	iHeadInChannelList 		= 1;
	iRearInChannelList 		= numItemInChannelList;
	iCountInChannelList 	= 1;
};

oEpg.definedVarBoxLeft = function(){
	Def_EleLeftZone				= $('#epgListLeft');
	Def_EleLeftZone.html("");
	for(var i = 0; i < Def_NumItemsLeftList; i++){
		var eleLi = $('<li>').addClass('epgItemListLeft');
		Def_EleLeftZone.append(eleLi);
	}
	Def_EleItemLeftZone			= $('.epgItemListLeft');
};

oEpg.getNewChannelList = function(top, end){
	var i = top;
	$.each(Def_EleItemLeftZone, function() {
		if (i <= end)
			$(this).html('<p>'+ aChannelListLeft.Data[i].major_minor + " " + aChannelListLeft.Data[i].Name +'</p>');
		else
			$(this).html('');
		i++;
	});
	
	iHeadInChannelList 	= top + 1;
	iRearInChannelList 	= end + 1;
};

oEpg.executeUpChannelList = function(selectedItem, newSelected){
	oVietnamTV.clearTimeoutPlayOTT();
	clearInterval(_executeChannelEpg);
    clearInterval(mcInterval);
	oJSExtensions.dtvplayback_obj.setDtvStatus(2);
	if(iCountInChannelList == 1){
		if(iHeadInChannelList > 1){
			var top 		= iHeadInChannelList - numItemInChannelList - 1;
			var end 		= iHeadInChannelList - 2;
			itemCurLeftFocus.removeClass(Def_NameClassFocusLeftItem);
			oEpg.getNewChannelList(top, end);
			itemCurLeftFocus 	= Def_EleItemLeftZone.last().addClass(Def_NameClassFocusLeftItem);
			oEpg.addMarqueeChannelList();
			iCountInChannelList = numItemInChannelList;
		}else if(iHeadInChannelList == 1){ // top list - first channel
		var length = aChannelListLeft.Data.length
			var iMod 			= length % Def_NumItemsLeftList;	// in list have max 9 items
			if(iMod == 0)
				iMod 			= Def_NumItemsLeftList;
			iHeadInChannelList 		= length - iMod + 1;
			iRearInChannelList 		= length;
			iCountInChannelList 	= iMod;
			itemCurLeftFocus.removeClass(Def_NameClassFocusLeftItem);
			oEpg.getNewChannelList(iHeadInChannelList - 1, iRearInChannelList - 1);
			itemCurLeftFocus = Def_EleItemLeftZone.eq(iCountInChannelList - 1).addClass(Def_NameClassFocusLeftItem);
			oEpg.addMarqueeChannelList();
		}
	}else{
		oEpg.setNextItemFocusChannelList('prev');
		iCountInChannelList--;
	}
	
	_executeChannelEpg = setInterval(function(){		
											oJSExtensions.dtvplayback_obj.setDtvStatus(0);
											oEpg.checkPlayChannel();
									}, timeIntervalNum);
	return { status: 'none' };
};

oEpg.executeDownChannelList = function(selectedItem, newSelected){
	oVietnamTV.clearTimeoutPlayOTT();
	clearInterval(_executeChannelEpg);
    clearInterval(mcInterval);
	oJSExtensions.dtvplayback_obj.setDtvStatus(2);
	if(iCountInChannelList == (iRearInChannelList - iHeadInChannelList + 1)){
		var length = aChannelListLeft.Data.length;
		if(iRearInChannelList < length){
			var top 		= iRearInChannelList;
			var end 		= iRearInChannelList + numItemInChannelList - 1;
			if(iRearInChannelList + numItemInChannelList >= length)
				end 		= length - 1;
			itemCurLeftFocus.removeClass(Def_NameClassFocusLeftItem);
			oEpg.getNewChannelList(top, end);
			itemCurLeftFocus 	= Def_EleItemLeftZone.first().addClass(Def_NameClassFocusLeftItem);
			oEpg.addMarqueeChannelList();
			iCountInChannelList 	= 1;
		}else{	// end list - last channel
			iHeadInChannelList 		= 1;
			iRearInChannelList 		= numItemInChannelList;
			iCountInChannelList 	= 1;
			itemCurLeftFocus.removeClass(Def_NameClassFocusLeftItem);
			oEpg.getNewChannelList(iHeadInChannelList - 1, iRearInChannelList - 1);
			itemCurLeftFocus = Def_EleItemLeftZone.first().addClass(Def_NameClassFocusLeftItem);
			oEpg.addMarqueeChannelList();
		}
	}else{
		oEpg.setNextItemFocusChannelList('next');
		iCountInChannelList++;
	}
	
	_executeChannelEpg = setInterval(function(){
											oJSExtensions.dtvplayback_obj.setDtvStatus(0);
											oEpg.checkPlayChannel();
									}, timeIntervalNum);
	return { status: 'none' };
};

oEpg.addMarqueeChannelList = function(){
	/*clearTimeout(setTimeOutMarquee);
	$('#epgListLeft li p[class*=marquee]').removeClass('marquee');
	if(itemCurLeftFocus.children('p').text().length > 30)
		setTimeOutMarquee = setTimeout(function(){
			itemCurLeftFocus.children('p').addClass('marquee');
		},1000);
			*/
};

oEpg.setNextItemFocusChannelList = function(dir){ 
	itemCurLeftFocus.removeClass(Def_NameClassFocusLeftItem);
	if(dir == 'next'){
		itemCurLeftFocus = itemCurLeftFocus.next().addClass(Def_NameClassFocusLeftItem);
	}else if(dir == 'prev'){
		itemCurLeftFocus = itemCurLeftFocus.prev().addClass(Def_NameClassFocusLeftItem);
	}
	oEpg.addMarqueeChannelList();
};

oEpg.checkPlayChannel = function(){
	var iCurrChannel = oVietnamTV.getCurrChannel();
	var iIndex = iHeadInChannelList + iCountInChannelList - 2;
	Authentication.Debug("\n oEpg.checkPlayChannel --- index = " + iIndex);
	clearInterval(_executeChannelEpg);
	itemCurRightFocus = undefined;
	if(aChannelListLeft.Data.length > 0){
		if (aChannelListLeft.Data[iIndex].isDTVChannel) {
			if(iCurrChannel != iIndex){
				if (!aChannelListLeft.Data[iCurrChannel].isDTVChannel) {
					oVietnamTV.clearAllInterval();
					if (oMain.isInitDTV == true && oMain.isInitDTVAmp == false){
						oJSExtensions.dtvplayback_obj.enterDtvPort();
						oMain.isInitDTVAmp = true;
					}else{
						if (oMain.isInitDTVAmp == true){
							oJSExtensions.dtvplayback_obj.restoreDTVEngSource(0);
						}
					}
				}
				if (oMain.isInitDTVAmp == true){
					var status = oJSExtensions.epg_obj.executeKey(aChannelListLeft.Data[iIndex].ID);
					Authentication.Debug("EPG >> >>> checkPlayChannel >>>> focusChannelIdx - 1 :" + aChannelListLeft.Data[iIndex].ID + " >>> status :" + status);
					if (status == true) {
						if (!aChannelListLeft.Data[iCurrChannel].isDTVChannel) {
							clearTimeout(timeoutSetScreenRectangle);
							timeoutSetScreenRectangle = setTimeout(function(){oEpg.setScreenRectangle();},1000);
						}
						totalProgram = -1; //0 for if (totalProgram != aProgram.length)
						oEpg.autoUpdateEPG();
					}
					if (aChannelListLeft.Data[iCurrChannel].isDTVChannel == false) {
						oJSExtensions.dtvplayback_obj.switchToMainVideoScaler(0);
					}
					oUserSetting.saveConf("current_major_minor", aChannelListLeft.Data[iIndex].major_minor);
					oVietnamTV.setCurrChannel(iIndex);
					oJSExtensions.epg_obj.setEpgGetMode(3);
				}
			}
		} else {
			if (aChannelListLeft.Data[iCurrChannel].isDTVChannel) {
				if(oMain.isInitDTV && oMain.isInitDTVAmp){
					oJSExtensions.dtvplayback_obj.stopPlay();
					oJSExtensions.dtvplayback_obj.releaseDTVEngSource(0);
					oJSExtensions.dtvplayback_obj.removeMainVideoScaler(0);
				}				
			}
			oVietnamTV.getLinkPlayChannel(iIndex);
			oEpg.autoUpdateEPG();
		}
		oEpg.showTopChannelInfo(aChannelListLeft.Data[iHeadInChannelList + iCountInChannelList - 2]);
	}else{
		return;
	}
};

var timeoutSetScreenRectangle = null;
oEpg.setScreenRectangle = function(){
	oJSExtensions.epg_obj.SetWndRect(71, 68, 306, 174);
};
/*
oEpg.changeChannel = function(){
	//Authentication.Debug("EPG >> >>> changeChannel");
	clearInterval(_executeChannelEpg);
	var focusChannelIdx = itemCurLeftFocus.data('id');
	var status = oJSExtensions.epg_obj.executeKey(focusChannelIdx - 1);
	//if (undefined != itemCurRightFocus)
		itemCurRightFocus = undefined;
	
	Authentication.Debug("EPG >> >>> changeChannel >>>> focusChannelIdx - 1 :" + (focusChannelIdx - 1) + " >>> status :" + status);
	if (status == true) {
		Def_EleTopChannelName.html(itemCurLeftFocus.text());
		totalProgram = -1; //0 for if (totalProgram != aProgram.length)
		oEpg.autoUpdateEPG();
	}
};*/
/*OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
/* ================================================== BOX RIGHT ========================================= */
var Def_NumItemsRightList	= 5;	// number items that the list can containt
var Def_EleRightZone;				// element of Zone
var Def_NameClassFocusRightItem	= 'epgFocusItemListRight';
var totalProgram;
var numItemInProgramList;
var iHeadInProgramList;
var iRearInProgramList;
var iCountInProgramList;

var itemCurRightFocus;
oEpg.startBoxRight = function(){
	itemCurLeftFocus.css({'background':'url(images/epg/left_cursor_selected.png) no-repeat center',
						'background-size':'200px 52px'});
	Def_EleLeftZone.addClass(Def_NameClassDimList);
	Def_EleRightZone.addClass(Def_NameClassFocusList);
	oEpg.showHideNarrow(false);
	oEpg.removeZone(oEpg.homeBehaviorZone);
	oEpg.setKeyControlBoxRight();
	if (totalProgram > 0) {
		//oEpg.showPvrMenu();
		if (undefined == itemCurRightFocus) {
			itemCurRightFocus = Def_EleRightZone.find('li').first().addClass(Def_NameClassFocusRightItem);
			iCountInProgramList 	= 1;
		} else {
			itemCurRightFocus.addClass(Def_NameClassFocusRightItem);
		}
	} else {
		Def_EleRightZone.find('li').first().addClass(Def_NameClassFocusRightItem);
	}
};

oEpg.definedVarBoxRight = function(){
	Def_EleRightZone			= $('#epgListRight');
	
	Def_EleRightZone.html();
	for(var i = 0; i < Def_NumItemsRightList; i++){
		var eleLi = $('<li>').addClass('epgItemListRight');
		Def_EleRightZone.append(eleLi);
	}
};

oEpg.setVarBoxRight = function(){
	var isResetView = false;
	if (totalProgram == -1) { // change channel, left/right day of week, update day of week
		isResetView = true
	}
	totalProgram 			= aProgram.length;
	numItemInProgramList 	= (totalProgram >= Def_NumItemsRightList) ? Def_NumItemsRightList : totalProgram;
	if(isResetView) {
		iHeadInProgramList 		= 1;
		iRearInProgramList 		= numItemInProgramList;
	}
	if (iRearInProgramList < numItemInProgramList) {
		iRearInProgramList 		= numItemInProgramList;
	}
};

oEpg.showHideNarrow = function(bool){
	if(bool) {
		$('#epgChannelNarrowUp').show();
		$('#epgChannelNarrowDown').show();
		$('#epgProgramNarrowUp').hide();
		$('#epgProgramNarrowDown').hide();
		$('#epgGuide').hide();
	} else {
		$('#epgChannelNarrowUp').hide();
		$('#epgChannelNarrowDown').hide();
		$('#epgProgramNarrowUp').show();
		$('#epgProgramNarrowDown').show();
		$('#epgGuide').show();
	}
}

oEpg.getNewProgramList = function(top, end){
	var currentId = -1;
	//var htmlTmp = '';
	if(undefined != itemCurRightFocus) {
		currentId = itemCurRightFocus.data('id');
	}

	var eleLi = Def_EleRightZone.find('li').eq(0);
	if (totalProgram == 0) {
		eleLi.data('id', 0).html('<p class="epgTimeProgram">' + oLanguageSetting.TVGuide_No_Information + '</p>');
		if (Def_EleRightZone.hasClass(Def_NameClassFocusList))
			eleLi.addClass(Def_NameClassFocusRightItem);
		eleLi = eleLi.next();
		itemCurRightFocus = undefined;
	} else {
		for(var i = top; i <= end; i++){
			eleLi.data('id', aProgram[i].id)
					 .html('<p class="epgTimeProgram">'+aProgram[i].timeShow+'</p><p class="epgNameProgram">'+aProgram[i].name+'</p>');
			//htmlTmp = '<div id="epgCheckPvrIcon'+ i +'" class="checkPvrIcon"></div>'; 
			//eleLi.append(htmlTmp);
			
			if(-1 != currentId && aProgram[i].id == currentId){
				eleLi.addClass(Def_NameClassFocusRightItem);
				itemCurRightFocus = eleLi;
			}
			eleLi = eleLi.next();
			
			/*if(oJSExtensions.epg_obj.GetStatusRecordEvent( itemCurLeftFocus.data('id') - 1, aProgram[i].eventID) == true ){
				oEpg.showAddPvrEvent(i);
			}*/
		}
		if (undefined == itemCurRightFocus && Def_EleRightZone.hasClass(Def_NameClassFocusList)) {//updating program and focus on program list.
			itemCurRightFocus = Def_EleRightZone.find('li').first().addClass(Def_NameClassFocusRightItem);
			iCountInProgramList 	= 1;
		}
	}
	while(eleLi.length) {
		eleLi.data('id', undefined).html('');
		eleLi = eleLi.next();
	}
	iHeadInProgramList 	= top + 1;
	iRearInProgramList 	= end + 1;
};

oEpg.setKeyControlBoxRight = function (){
	var aKeyMapping = new Array();
	{									
		aKeyMapping[Key.RETURN] = 	function(oldItem, newItem) {
			Authentication.Debug("EPG >> handle program __ key.RETURN");
			if (undefined != itemCurRightFocus)
				itemCurRightFocus.removeClass(Def_NameClassFocusRightItem);
			else
				Def_EleRightZone.find('li').first().removeClass(Def_NameClassFocusRightItem);
			//itemCurRightFocus = undefined;
			itemCurLeftFocus.removeAttr( 'style' );	// set background normal
			oEpg.showHideNarrow(true);
			Def_EleLeftZone.removeClass(Def_NameClassDimList);
			Def_EleRightZone.removeClass(Def_NameClassFocusList);
			oEpg.removeZone(oEpg.homeBehaviorZone);
			oEpg.setKeyControlEpg();
			//oEpg.hidePvrMenu();
			return { status: 'none' };
		};
									
		aKeyMapping[Key.ENTER]  	= 	function(oldItem, newItem) {
			Authentication.Debug("EPG >> handle program __ key.ENTER");
			if (undefined != itemCurRightFocus)
				oEpg.showProgramDescription(aChannelListLeft.Data[iHeadInChannelList + iCountInChannelList - 2], aProgram[itemCurRightFocus.data('id')]);
			return { status: 'none' };
		};
									
		aKeyMapping[Key.UP]  	= 	function(oldItem, newItem) {
			Authentication.Debug("EPG >> handle program __ key.UP");
			oEpg.executeUpProgramList();
			return { status: 'none' };
		};

		aKeyMapping[Key.DOWN] 	= 	function(oldItem, newItem) {
			Authentication.Debug("EPG >> handle program __ key.DOWN");
			oEpg.executeDownProgramList();
			return { status: 'none' };
		};	
		aKeyMapping[Key.LEFT] 	= 	function(oldItem, newItem) {
			Authentication.Debug("EPG >> handle program __ key.LEFT");
			oEpg.executeLeftFocusDays();
			return { status: 'none' };
		};	
		aKeyMapping[Key.RIGHT] 	= 	function(oldItem, newItem) {
			Authentication.Debug("EPG >> handle program __ key.RIGHT");
			oEpg.executeRightFocusDays();
			return { status: 'none' };
		};
		aKeyMapping[Key.RED] 	= 	function(oldItem, newItem) {
			/*var focusChannelIdx = itemCurLeftFocus.data('id');
			var focusProgramIdx = itemCurRightFocus.data('id');
			Authentication.Debug("EPG >> handle program __ key.RED");
			Authentication.Debug("EPG >> handle program __ focusChannelIdx="+focusChannelIdx);
			if (totalProgram > 0) {
				var currentPath = "/" + oDetectDevice.aUsbPath[0];
				 Authentication.Debug("EPG >> Name USB "+ currentPath);
				 if ((currentPath == "/F" )|| (currentPath == "/undefined"))
				 {
				  currentPath = "/mnt";
				 }
				 oEpg.parseEventInfo(focusProgramIdx);
				 Authentication.Debug("EPG >> checkOverLapTime " +oJSExtensions.pvr_obj.checkOverLapTime(Flag_Time_Start, Flag_Time_End));
				 Authentication.Debug("EPG >> status " +aProgram[focusProgramIdx].pvrStatus);
				 Authentication.Debug("EPG >> eventID " +aProgram[focusProgramIdx].eventID);
				 Authentication.Debug("EPG >> get status " +oJSExtensions.epg_obj.GetStatusRecordEvent( (focusChannelIdx - 1), aProgram[focusProgramIdx].eventID));
				if (oDetectDevice.iTotalDevice <= 0) {
					oEpg.showNoticeMessage(oLanguageSetting.Dtv_Pvr_Msg);
				}  
				else {	
					if(oJSExtensions.epg_obj.GetStatusRecordEvent( (focusChannelIdx - 1), aProgram[focusProgramIdx].eventID) == false ){
						if (oJSExtensions.pvr_obj.checkOverLapTime(Flag_Time_Start, Flag_Time_End) == false){						
							var statusAdd = oJSExtensions.pvr_obj.addScheduled((focusChannelIdx - 1), Flag_Time_Start, Flag_Time_End, currentPath ,aProgram[focusProgramIdx].eventID, 1, 0);
							Authentication.Debug("statusAdd "+ statusAdd);
							if(statusAdd == true){
								oEpg.showAddPvrEvent(focusProgramIdx);
								oJSExtensions.epg_obj.SetStatusRecordEvent( (focusChannelIdx - 1), aProgram[focusProgramIdx].eventID, true);
							}
							else{
								oEpg.showNoticeMessage(oLanguageSetting.Dtv_Pvr_Add_Error);
							}
						}
						else{
							oEpg.showNoticeMessage(oLanguageSetting.Dtv_Pvr_Conflict_Time);
						}
					}
					else {
						oJSExtensions.pvr_obj.deleteScheduledByStartTime(Flag_Time_Start);
						oJSExtensions.epg_obj.SetStatusRecordEvent( (focusChannelIdx - 1), aProgram[focusProgramIdx].eventID, false);
						oEpg.hideAddPvrEvent(focusProgramIdx);
					}
				 }
				 Authentication.Debug("EPG >> Name USB "+ currentPath);
			}	 
			*/
			return { status: 'none' };
		};
	}
	
	var oNavSelectors = {item: '.epgItemListRight'};
  	var oSelectionClasses = {};
  	var oActions = {};
	
	oEpg.homeBehaviorZone = new gtv.jq.KeyBehaviorZone({
										        containerSelector	: '#epgListRight',
										        keyMapping			: aKeyMapping,
										        actions 			: oActions,
										        navSelectors 		: oNavSelectors,
										        selectionClasses 	: oSelectionClasses,
										        navigableData 		: 'url',
										        useGeometry 		: false
										      });
  	oMain.keyController.addBehaviorZone(oEpg.homeBehaviorZone, true);
  	oMain.keyController.start();
};

oEpg.executeUpProgramList = function(selectedItem, newSelected){
	if (undefined == itemCurRightFocus)
		return;
	if(iCountInProgramList == 1){
		if(iHeadInProgramList > 1){
			oEpg.scrollUpBoxRight();
			iCountInProgramList = numItemInProgramList;			
		}else if(iHeadInProgramList == 1){ // top list - first channel
	
			var iMod 			= aProgram.length % Def_NumItemsRightList;	// in list have max 9 items
			if(iMod == 0)
				iMod 			= Def_NumItemsRightList;
			iHeadInProgramList 		= aProgram.length - iMod + 1;
			iRearInProgramList 		= aProgram.length;
			iCountInProgramList 	= iMod;
			itemCurRightFocus.removeClass(Def_NameClassFocusRightItem);
			oEpg.getNewProgramList(iHeadInProgramList - 1, iRearInProgramList - 1);
			itemCurRightFocus 		= Def_EleRightZone.find('li').eq(iCountInProgramList - 1).addClass(Def_NameClassFocusRightItem);
		}
	}else{
		oEpg.setNextItemFocusProgramList('prev');
		iCountInProgramList--;
	}
	return { status: 'none' };
};

oEpg.executeDownProgramList = function(selectedItem, newSelected){
	if (undefined == itemCurRightFocus)
		return;
	if(iCountInProgramList == (iRearInProgramList - iHeadInProgramList + 1)){
		if(iRearInProgramList < aProgram.length){
			oEpg.scrollDownBoxRight();
			iCountInProgramList 	= 1;			
		}else{	// end list - last channel
			iHeadInProgramList 		= 1;
			iRearInProgramList 		= numItemInProgramList;
			iCountInProgramList 	= 1;
			itemCurRightFocus.removeClass(Def_NameClassFocusRightItem);
			oEpg.getNewProgramList(iHeadInProgramList - 1, iRearInProgramList - 1);
			itemCurRightFocus 		= Def_EleRightZone.find('li').first().addClass(Def_NameClassFocusRightItem);
		}
	}else{
		oEpg.setNextItemFocusProgramList('next');
		iCountInProgramList++;
	}
	return { status: 'none' };
};

oEpg.setNextItemFocusProgramList = function(dir){ 
	itemCurRightFocus.removeClass(Def_NameClassFocusRightItem);
	if(dir == 'next'){
		itemCurRightFocus = itemCurRightFocus.next().addClass(Def_NameClassFocusRightItem);
	}else if(dir == 'prev'){
		itemCurRightFocus = itemCurRightFocus.prev().addClass(Def_NameClassFocusRightItem);
	}
	//oEpg.addMarqueeChannelList();
};

oEpg.scrollUpBoxRight = function(){
	var top 		= iHeadInProgramList - numItemInProgramList - 1;
	var end 		= iHeadInProgramList - 1 - 1;
	itemCurRightFocus.removeClass(Def_NameClassFocusRightItem);
	oEpg.getNewProgramList(top, end);
	itemCurRightFocus 	= Def_EleRightZone.find('li').last().addClass(Def_NameClassFocusRightItem);
	//oEpg.addMarqueeUSB();
};

oEpg.scrollDownBoxRight = function(){
	var top 		= iRearInProgramList;
	var end 		= iRearInProgramList + numItemInProgramList - 1;
	if(iRearInProgramList + numItemInProgramList >= aProgram.length)
		end 		= aProgram.length - 1;
	itemCurRightFocus.removeClass(Def_NameClassFocusRightItem);
	oEpg.getNewProgramList(top, end);
	itemCurRightFocus 	= Def_EleRightZone.find('li').first().addClass(Def_NameClassFocusRightItem);
	//oEpg.addMarqueeUSB();

};
/*OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
/* ================================================== BOX DAY ========================================= */
oEpg.parseDateTime = function(str) {
	var arrTime = str.split("/");
	return new Date(arrTime[4], arrTime[3] - 1, arrTime[2], arrTime[0], arrTime[1], 0, 0);
};
oEpg.showHeaderDayTime = function(date) {
	var day = date.getDay() - 1;
	if (day < 0)
		day = 6;
	Def_EleTopDayTime.find('p:eq(0)').html(WeekDay[day] + " " + oEpg.formatDate(date) + "/" + date.getFullYear());
	Def_EleTopDayTime.find('p:eq(1)').html(oEpg.formatAMPM(date));
};

oEpg.formatDate = function(date) {
	return oEpg.formatNumber(date.getMonth() + 1) + "/" + oEpg.formatNumber(date.getDate());
};

oEpg.formatAMPM = function(date) {
	var hours = date.getHours();
	var minutes = date.getMinutes();
	var ampm = hours >= 12 ? 'PM' : 'AM';
	hours = hours % 12;
	hours = hours ? hours : 12; // the hour '0' should be '12'
	hours = oEpg.formatNumber(hours);
	minutes = oEpg.formatNumber(minutes);
	return hours + ':' + minutes + ' ' + ampm;
};

oEpg.formatNumber = function(n) {
	return n < 10 ? '0'+n : n;
};

oEpg.executeLeftFocusDays = function() {
	if (focusEPGDay > 1) {
		focusEPGDay --;
		if (posWeekDay > 1) {
			posWeekDay --;
		} else {
			posWeekDay = 7;
		}
		clearInterval(mcInterval);
		clearInterval(_executeChannelEpg);
		var status = oJSExtensions.epg_obj.executeEpgKey(posWeekDay);
		Authentication.Debug("\n status = "+status);
		if (status == true ) {
			if (undefined != itemCurRightFocus)
				itemCurRightFocus.removeClass(Def_NameClassFocusRightItem);
			itemCurRightFocus = undefined;
			totalProgram = -1; //0 for if (totalProgram != aProgram.length)
			oEpg.getEpgData();
			oEpg.showTextEPGMain();
			eleFocusDay.html(WeekDay[posWeekDay - 1] + '<br/><p style="font-size: 18px;">' + _currDayMonth + '</p>');
			
			if (totalProgram > 0) {
				//oEpg.showPvrMenu();
				itemCurRightFocus = Def_EleRightZone.find('li').first().addClass(Def_NameClassFocusRightItem);
				iCountInProgramList 	= 1;
			} else {
				//oEpg.hidePvrMenu();
			}
			mcInterval = setInterval(function(){
				oEpg.loadEPG();
			},5000);
		}
	}
	if (focusEPGDay == 1) {
		$('#epgGuide #epgButtonPrevious').hide();
		$('#epgGuide #epgTextPrevious').hide();
	} else if (focusEPGDay == 6) {
		$('#epgGuide #epgButtonNext').show();
		$('#epgGuide #epgTextNext').show();
	}
	Authentication.Debug("******* move left day*******focusEPGDay:"+focusEPGDay + " >> posWeekDay:"+posWeekDay);
}
	
oEpg.executeRightFocusDays = function() {
	if (focusEPGDay < 7) {
		focusEPGDay ++;
		if (posWeekDay <7) {
			posWeekDay ++;
		} else {
			posWeekDay = 1;
		}
		clearInterval(mcInterval);
		clearInterval(_executeChannelEpg);
		var status = oJSExtensions.epg_obj.executeEpgKey(posWeekDay);
		Authentication.Debug("\n status = " + status);
		if (status == true) {
			if (undefined != itemCurRightFocus)
				itemCurRightFocus.removeClass(Def_NameClassFocusRightItem);
			itemCurRightFocus = undefined;
			totalProgram = -1; //0 for if (totalProgram != aProgram.length)
			oEpg.getEpgData();
			oEpg.showTextEPGMain();
			eleFocusDay.html(WeekDay[posWeekDay - 1] + '<br/><p style="font-size: 18px;">' + _currDayMonth + '</p>');
			
			if (totalProgram > 0) {
				//oEpg.showPvrMenu();
				itemCurRightFocus = Def_EleRightZone.find('li').first().addClass(Def_NameClassFocusRightItem);
				iCountInProgramList 	= 1;
			} else {
				//oEpg.hidePvrMenu();
			}
			mcInterval = setInterval(function(){
				oEpg.loadEPG();
			},5000);
		}
	}
	if (focusEPGDay == 7) {
		$('#epgGuide #epgButtonNext').hide();
		$('#epgGuide #epgTextNext').hide();
	} else if (focusEPGDay == 2) {
		$('#epgGuide #epgButtonPrevious').show();
		$('#epgGuide #epgTextPrevious').show();
	}
	Authentication.Debug("******* move right day*******focusEPGDay:"+focusEPGDay + " >> posWeekDay:"+posWeekDay);
}

oEpg.showProgramDescription = function(eleChannel, eleProgram) {
	Authentication.Debug("EPG >> >>> showProgramDescription");
	$('#epgProgramDescription').show();
	
	oEpg.removeZone(oEpg.homeBehaviorZone);
	oEpg.setKeyControlProgramDescription();
	
	$('#epgProgramDescription #epgProgramDescriptionNameChannel').html(eleChannel.Name);
	$('#epgProgramDescription #epgProgramDescriptionInfoChannel').html(Def_EleTopChannelInfo.html());
	$('#epgProgramDescription #epgProgramDescriptionTimeProgram').html(eleProgram.timeShow);
	$('#epgProgramDescription #epgProgramDescriptionNameProgram').html(eleProgram.name);
	$('#epgProgramDescription #epgProgramDescriptionProgram').html(eleProgram.description);
};

oEpg.hideProgramDescription = function() {
	Authentication.Debug("EPG >> >>> hideProgramDescription");
	$('#epgProgramDescription').hide();
	oEpg.removeZone(oEpg.homeBehaviorZone);
	oEpg.setKeyControlBoxRight();
};

oEpg.setKeyControlProgramDescription = function(){
	Authentication.Debug("EPG >> >>> setKeyControlProgramDescription");
	var aKeyMapping = new Array();
	{		
		aKeyMapping[Key.RETURN] = 	function(oldItem, newItem) {
			oEpg.hideProgramDescription();
			return { status: 'none' };
		};
	}
	
	var oNavSelectors = {item: '#epgProgramDescriptionInfo'};
  	var oSelectionClasses = {};
  	var oActions = {};
	
	oEpg.homeBehaviorZone = new gtv.jq.KeyBehaviorZone({
										        containerSelector	: '#epgProgramDescription',
										        keyMapping			: aKeyMapping,
										        actions 			: oActions,
										        navSelectors 		: oNavSelectors,
										        selectionClasses 	: oSelectionClasses,
										        navigableData 		: 'url',
										        useGeometry 		: false
										      });
  	oMain.keyController.addBehaviorZone(oEpg.homeBehaviorZone, true);
  	oMain.keyController.start();
};

oEpg.hideAllIconSpecial = function(){
	$('#epgSpecialChannel #epgIconScrambled').hide();
	$('#epgSpecialChannel #epgIconRadio').hide();
	$('#epgSpecialChannel #epgIconLocked').hide();
	$('#epgSpecialChannel #epgIconSkip').hide();
}
oEpg.checkStatusChannel = function (curChannelIdx){
	oEpg.hideAllIconSpecial();
	Authentication.Debug(">>> checkStatusChannel <<< " + curChannelIdx);
	if (totalChannels > 0) {
		if(oJSExtensions.dtvplayback_obj.getStatusSkipChannel(curChannelIdx) == 1){ 
			Authentication.Debug(">>> getStatusSkipChannel <<< " );
			$('#epgSpecialChannel #epgIconSkip').show();
			oJSExtensions.dtvplayback_obj.stopPlay();
		}
		else if(oJSExtensions.dtvplayback_obj.getStatusLockChannel(curChannelIdx) == 1){ 
			Authentication.Debug(">>> getStatusLockChannel <<< " );
			if (oBroadcastTV.isUnlocked == true)
				return;
				
			$('#epgSpecialChannel #epgIconLocked').show();
			oJSExtensions.dtvplayback_obj.stopPlay();
		}
		else if(oJSExtensions.dtvplayback_obj.displayRadioChannel(curChannelIdx) == 1){ 
			Authentication.Debug(">>> displayRadioChannel <<< " );
			$('#epgSpecialChannel #epgIconRadio').show();
		}
		else if(oJSExtensions.dtvplayback_obj.displayScrambleChannel(curChannelIdx) == 1){ 
			Authentication.Debug(">>> displayScrambleChannel <<< " );
			$('#epgSpecialChannel #epgIconScrambled').show();
		}
		else{			
			Authentication.Debug(">>> checkStatusChannel else <<< " );
		}
	}
};

oEpg.showPvrMenu = function (){
	$('#epgPvr').show();
};
oEpg.hidePvrMenu = function (){
	$('#epgPvr').hide();
};
var Flag_Time_Start;
var Flag_Time_End;

oEpg.parseEventInfo = function (fcProgram){
	Authentication.Debug("parseEventInfo >> _currDatetime=" + _currDatetime + "_currWeekDay=" + _currWeekDay +"_currDayMonth="+_currDayMonth);
	var timeStart = new Array();
	var timeEnd = new Array();
	var strDateMonth = new Array();
	var strCurrDate = new Array();
	var strTimeShow = new Array();
	var i;
	
	strCurrDate		= _currDayMonth.split("/");
	/*for(i = 0; i < strCurrDate.length; i++){
		Authentication.Debug("==== strCurrDate["+i+"]"+strCurrDate[i]);
	}*/
	strDateMonth = _currDatetime.split(" ");
	/*for(i = 0; i < strDateMonth.length; i++){
		Authentication.Debug("==== strDateMonth["+i+"]"+strDateMonth[i]);
	}*/
	strTimeShow = aProgram[fcProgram].timeShow.split(" ");
	/*for(i = 0; i < strTimeShow.length; i++){
		Authentication.Debug("==== strTimeShow["+i+"]"+strTimeShow[i]);
	}*/
	
	timeStart = strTimeShow[0].split(":");
	/*for(i = 0; i < timeStart.length; i++){
		Authentication.Debug("==== timeStart["+i+"]"+timeStart[i]);
	}*/
	timeEnd = strTimeShow[3].split(":");
	/*for(i = 0; i < timeEnd.length; i++){
		Authentication.Debug("==== timeEnd["+i+"]"+timeEnd[i]);
	}*/
	
	Flag_Time_Start = oEpg.checkFormatDay(strCurrDate[0]) +"/"+oEpg.checkFormatDay(strCurrDate[1])+"/"+
					strDateMonth[3]	+"  "+oEpg.checkFormatTime(timeStart[0],strTimeShow[1])+":"+timeStart[1];
	Authentication.Debug("==== Flag_Time_Start=="+Flag_Time_Start);
	
	
	Flag_Time_End = oEpg.checkFormatDay(strCurrDate[0]) +"/"+oEpg.checkFormatDay(strCurrDate[1])+"/"+
					strDateMonth[3]	+"  "+oEpg.checkFormatTime(timeEnd[0],strTimeShow[4])+":"+timeEnd[1];
	Authentication.Debug("==== Flag_Time_End=="+Flag_Time_End);
	
};
oEpg.checkFormatDay = function (str){
	var tmpNumber = parseInt(str,10);
	if (tmpNumber < 10){
		return "0" + tmpNumber;
	}
	return tmpNumber;
};
oEpg.checkFormatTime = function (strTime, strFormat){
	var tmpNumber = parseInt(strTime,10);
	if (tmpNumber == 12 && strFormat == "AM"){
		return 0;
	}
	else if(tmpNumber < 12 && strFormat == "PM"){
		return tmpNumber + 12;
	}
	else if(tmpNumber < 10){
		return "0" + tmpNumber;
	}
	return tmpNumber;
};

oEpg.showAddPvrEvent = function (idx){
	$('#epgCheckPvrIcon'+idx).show();
};
oEpg.hideAddPvrEvent = function (idx){
	$('#epgCheckPvrIcon'+idx).hide();
};
oEpg.showNoticeMessage = function(msg){
	Authentication.Debug("------------- >>>>>>>>>>>>> showNoticeMessage  <<<<<<<<<<<<<<<<<< ----------------");
	$('#epgWrap').append("<div id='Confirm'></div>");
	oEpg.showConfirmMsg(msg);
	oEpg.removeZone(oEpg.homeBehaviorZone);
	setTimeout(function(){
		oEpg.hideConfirmMsg();
	}, 3000);
};
	
oEpg.showConfirmMsg = function (strMessage)
{
	//Authentication.Debug("showConfirmMenu");
	var html = "<div id='BackGroundMess'>";
	html+= "<div class='item'></div>";		// *** lando - 3G - 28/03/2013
	html+= '<div id="StrMess" style="margin-top: 27px;"></div>';
	html+= "<div id='StrMess1'></div>";
	html+= "</div>";
	$('#Confirm').append(html);
	Authentication.Debug("showConfirmMenu : "+strMessage);
	$("#StrMess").append(strMessage);
	$("#StrOk").append(oLanguageSetting.Setting_YES).css("color","orange");
};

oEpg.hideConfirmMsg = function () {
	Authentication.Debug("#--------------hideConfirmMsg-------------#");
	$("img[id=StrOk]").remove();
	$("#StrOk").remove();
	$("#StrMess").remove();
	$('#Confirm').remove();
	if(oEpg.homeBehaviorZone != null){
		oEpg.removeZone(oEpg.homeBehaviorZone);
	}
	oEpg.setKeyControlBoxRight();
};
oEpg.convertDate = function(strStr){
	Authentication.Debug("convertDate =="+strStr);
	var strDate ="";
	var i;
	var strDay = new Array();
	var arrStr = new Array();
	arrStr = strStr.split(" ");
	for( i = 0; i < arrStr.length; i++){
		Authentication.Debug("==== arrStr["+i+"]"+arrStr[i]);
	}
	switch(arrStr[0]){
		case 'Mon':
			strDate = "Th? 2";
			break;
		case 'Tue':
			strDate = "Th? 3";
			break;
		case 'Wed':
			strDate = "Th? 4";
			break;
		case 'Thu':
			strDate = "Th? 5";
			break;
		case 'Fri':
			strDate = "Th? 6";
			break;
		case 'Sat':
			strDate = "Th? 7";
			break;
		case 'Sun':
			strDate = "CN";
			break;
	}
	Authentication.Debug("strDate 1=="+strDate);
	strDay = arrStr[2].split(",");
	for( i = 0; i < strDay.length; i++){
		Authentication.Debug("==== strDay["+i+"]"+strDay[i]);
	}
	strDate = strDate + ", " + oEpg.checkFormatDay(strDay[0]) + "/";
	Authentication.Debug("strDate 2=="+strDate);
	switch(arrStr[1]){
		case 'Jan':
			strDate +="01";
			break;
		case 'Feb':
			strDate +="02"; 
			break;
		case 'Mar':
			strDate +="03";
			break;
		case 'Apr':
			strDate +="04";
			break;
		case 'May':
			strDate +="05";
			break;
		case 'Jun':
			strDate +="06";
			break;
		case 'Jul':
			strDate +="07";
			break;
		case 'Aug':
			strDate +="08";
			break;
		case 'Sep':
			strDate +="09";
			break;
		case 'Oct':
			strDate +="10";
			break;
		case 'Nov':
			strDate +="11";
			break;
		case  'Dec':
			strDate +="12";
			break;
	}
	strDate = strDate + "/" + arrStr[3] + " - " + arrStr[5] +" "+ arrStr[6];
	Authentication.Debug("strDate 3=="+strDate);
	return strDate;
};

var isFocusConfirmMenu;

oEpg.showNoticeRecord = function(msg){
	$('#epgWrap').append("<div id='Confirm'></div>");
	oEpg.showConfirmRecord(msg);
	oEpg.removeZone(oEpg.homeBehaviorZone);
	oEpg.focusNotice();
}

oEpg.showConfirmRecord = function (strMessage)
{	
	isFocusConfirmMenu 	= false;
	var html = "<div id='BackGroundMess'>";
	html+= "<div class='item'></div>";
	html+= "<div id='StrMess'></div>";
	html+= "<div id='StrMess1'></div>";
	html+= "<div id='StrYes'></div>";
	html+= "<div id='StrNo'></div>";
	html+= "<img style='' id='FocusYes' src='images/Setting/caution/button%20selected.png'>";
	html+= "<img style='' id='FocusNo' src='images/Setting/caution/button%20selected.png'>";
	html+= "</div>";
	$('#Confirm').append(html);
	$("img[id=FocusYes]").hide();
	Authentication.Debug("showConfirmMenu : "+strMessage);

	$("#StrMess").append(strMessage);
	$("#StrYes").append(oLanguageSetting.Setting_YES).css("color","white");
	$("#StrNo").append(oLanguageSetting.Setting_NO).css("color","orange");
};

oEpg.hideNoticeRecord = function () {
	Authentication.Debug("------------- >>>>>>>>>>>>> hideNoticeMessage  <<<<<<<<<<<<<<<<<< ----------------");
	$("img[id=FocusNo]").remove();
	$("img[id=FocusYes]").remove();
	$("#StrYes").remove();
	$("#StrNo").remove();
	$('#Confirm').remove();
	isFocusConfirmMenu 	= false;
}

oEpg.focusNotice = function ()
{
	Authentication.Debug(" ------------ focusMessage ------------");
	var aKeyMsg = new Array();
	aKeyMsg[Key.LEFT] = function()
	{
		Authentication.Debug("#------------ Key.LEFT ------------#");
		isFocusConfirmMenu = true;
		$("img[id=FocusNo]").hide();
		$("img[id=FocusYes]").show();
		$("#StrYes").css("color","orange");
		$("#StrNo").css("color","white");
		return { status: 'none' };
	};
	
	aKeyMsg[Key.RIGHT] = function()
	{
		Authentication.Debug("#------------ Key.RIGHT ------------#");
		isFocusConfirmMenu = false;
		$("img[id=FocusNo]").show();
		$("img[id=FocusYes]").hide();
		$("#StrYes").css("color","white");
		$("#StrNo").css("color","orange");
		return { status: 'none' };
	}
	
	aKeyMsg[Key.ENTER] = function()
	{
		Authentication.Debug("#------------ Key.ENTER ------------#");
		var checkRecording = oJSExtensions.pvr_obj.checkRecording();
		if(checkRecording >= 0){
			if(isFocusConfirmMenu){
				oJSExtensions.pvr_obj.stopRecord();
				if(isUp == true) {
					oEpg.executeUpChannelList();
				}
				else if(isDown == true) {
					oEpg.executeDownChannelList();
				}
			}else {
				//curChannelIdx = oJSExtensions.dtvplayback_obj.getCurrentChannel();
			}
		}
		oEpg.hideNoticeRecord();
		oEpg.removeZone(oEpg.homeBehaviorZone);
		oEpg.setKeyControlEpg();
		return { status: 'none' };
	}
	
	aKeyMsg[Key.RETURN] = function()
	{
		oEpg.hideNoticeRecord();
		oEpg.removeZone(oEpg.homeBehaviorZone);
		oEpg.setKeyControlEpg();
		return { status: 'none' };
	}
		
	var oNavSelectors = {item: '.item'};
  	var oSelectionClasses = {};
  	var oActions = {};
	
	oEpg.homeBehaviorZone = new gtv.jq.KeyBehaviorZone({
										        containerSelector	: '#Confirm',
										        keyMapping			: aKeyMsg,
										        actions 			: oActions,
										        navSelectors 		: oNavSelectors,
										        selectionClasses 	: oSelectionClasses,
										        navigableData 		: 'url',
										        useGeometry 		: false
										      });
											  
  	oMain.keyController.addBehaviorZone(oEpg.homeBehaviorZone, true);
  	oMain.keyController.start();
}
} // END EPG