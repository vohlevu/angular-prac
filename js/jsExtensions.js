var oJSExtensions = {};

oJSExtensions.mediasetting 			= null;
oJSExtensions.dtvplayback_obj 			= null;
oJSExtensions.epg_obj					= null;


oJSExtensions.init = function () {
	oJSExtensions.mediasetting = new jsmediasetting(oJSExtensions);
	oJSExtensions.epg_obj = new jsdtvepg(oJSExtensions);
	oJSExtensions.dtvplayback_obj = new jsdtvplayback(oJSExtensions);
};

oJSExtensions.start = function () {
	
};

oJSExtensions.send = function (obj, func) {
	var data = {
					'obj' 	: obj,
					'func' : func
				};
	var result;
	$.ajax({
		type: 'POST',
		url: location.href,
		data: data,
		dataType: 'json',
		async:false,
		success: function(resultData) {
			if (resultData.status == "OK")
				result = resultData.result;
		}
	});
	Authentication.Debug(">>>> oJSExtensions.send <<<<<< " + result);
	return result;
};

function jsdtvepg(ext){
	var oJsdtvepg = this;
	var extensions = ext;
	var TAG = "jsdtvepg";
	var countCheckUpdateEvent;
	oJsdtvepg.showEPG = function (){
		Authentication.Debug(">>>> oMediasetting.showEPG <<<<<<");
		countCheckUpdateEvent = 0;
		main.isInitDTVAmp = true;
	};
	oJsdtvepg.getTime = function (){
		Authentication.Debug(">>>> oMediasetting.getTime <<<<<<");
		return extensions.send(TAG,"getTime");
		//return "23/34/6/11/2007";
	};
	oJsdtvepg.getInfo = function (){
		Authentication.Debug(">>>> oMediasetting.getInfo <<<<<<");
		var str = "Tue Nov 6, 2007  11:39 PM|||2|||11/07/2007|||0|||0|||1|||0|||"+
				"0 |QUEST |12:00 AM - 12:30 AM |152 |SD |QUEST "+
				"||0 |QUEST |12:30 AM - 01:00 AM |153 |SD |Preemie Babies, Illuminating The Northern Lights How Bay Area researchers are improving care for premature babies; how scientists are shedding new light on the earth's magnetic field; Bay Area Falcon webcams " + 
				"||0 |Nova |01:00 AM - 02:00 AM |154 |SD |Nova "+
				"||0 |Nature |02:00 AM - 03:00 AM |155 |SD |Diamonds An encyclopedic look at the oldest, hardest natural substance in the world - diamonds - from their beginnings in the core of the earth, through mining and acquisition, to the history of the most famous of these extraordinary gems. " +
				"||0 |Frontline/World |03:00 AM - 04:00 AM |284 |SD |Extraordinary Rendition An examination of the CIA's controversial practice of kidnapping terror suspects for interrogation. "+
				"||0 |Austin City Limits |04:00 AM - 05:00 AM |285 |SD |Wilco The ever-evolving Wilco performs classics and cuts from its latest disk, Sky Blue Sky. " +
				"||0 |Joanne Weir's Cooking Class |05:00 AM - 05:30 AM |286 |SD |Joanne Weir's Cooking Class "+
				"||0 |America's Test Kitchen |05:30 AM - 06:00 AM |287 |SD |Strawberry Cream Cake Serve a spectacular dessert with ATK's strawberry cream cake. Equipment: standing mixers. Science: whipping cream. " +
				"||0 |Desert Speaks |06:00 AM - 06:30 AM |416 |SD |Desert Speaks "+
				"||0 |Farmers' Almanac TV |06:30 AM - 07:00 AM |417 |SD |Farmers' Almanac TV "+
				"||0 |Travels to the Edge with Art Wolfe |07:00 AM - 07:30 AM |418 |SD |Travels to the Edge with Art Wolfe "+
				"||0 |Check, Please! Bay Area |07:30 AM - 08:00 AM |419 |SD |Check, Please! Bay Area "+
				"||0 |History Detectives |08:00 AM - 09:00 AM |420 |SD |History Detectives  "+
				"|||13|||";
		return str;
	};
	oJsdtvepg.SetWndRect = function (x, y, width, height){
		Authentication.Debug(">>>> oMediasetting.SetWndRec <<<<<<");
		$('#epgWrap #epgVideo').css({'top':y,
								'left':x,
								'width':width,
								'height':height});
	};
	oJsdtvepg.executeKey = function (index){
		Authentication.Debug(">>>> oMediasetting.executeKey <<<<<< " + index);
		var strSrc;
		switch(index){
			case 0:
				strSrc = "http://mp3.zing.vn/embed/video/ZW6AC6OO?autostart=true";
				break;
			case 1:
				strSrc = "http://mp3.zing.vn/embed/video/ZWZF6UAC?autostart=true";
				break;
			case 2:
				strSrc = "http://mp3.zing.vn/embed/video/ZW6779W7?autostart=true";
				break;
			case 3:
				strSrc = "http://mp3.zing.vn/embed/video/ZW6990AZ?autostart=true";
				break;
			default:
				strSrc = "http://mp3.zing.vn/embed/video/ZW6ACUDO?autostart=true";
				break;
		}
		$('#epgWrap #epgVideo').attr('src', strSrc);
		return true;
	};
	oJsdtvepg.setEpgGetMode = function (){
		Authentication.Debug(">>>> oMediasetting.setEpgGetMode <<<<<<");
	};
	oJsdtvepg.checkUpdateEvent = function (){
		Authentication.Debug(">>>> oMediasetting.checkUpdateEvent <<<<<<");
		if (countCheckUpdateEvent > 5)
			return true;
		countCheckUpdateEvent++;
		return false
	};
	oJsdtvepg.executeEpgKey = function (){
		Authentication.Debug(">>>> oMediasetting.executeEpgKey <<<<<<");
		return true;
	};
};
function jsmediasetting(){
	var oJsmediasetting = this;
	oJsmediasetting.setScale = function (){
		Authentication.Debug(">>>> oMediasetting.setScale <<<<<<");
	};
	oJsmediasetting.setEpgGetMode = function (){
		Authentication.Debug(">>>> oMediasetting.setEpgGetMode <<<<<<");
	};
};
function jsdtvplayback(){
	var oJsdtvplayback = this;
	oJsdtvplayback.setDtvStatus = function (){
		Authentication.Debug(">>>> oMediasetting.setDtvStatus <<<<<<");
	};
	oJsdtvplayback.getAudioType = function (){
		Authentication.Debug(">>>> oMediasetting.getAudioType <<<<<<");
		return "DOLBY";
	};
	oJsdtvplayback.getVideoType = function (){
		Authentication.Debug(">>>> oMediasetting.getVideoType <<<<<<");
		return "MPEG-2";
	};
	oJsdtvplayback.enterDtvPort = function (){
		Authentication.Debug(">>>> oMediasetting.enterDtvPort <<<<<<");
	};
	oJsdtvplayback.restoreDTVEngSource = function (){
		Authentication.Debug(">>>> oMediasetting.restoreDTVEngSource <<<<<<");
	};
	oJsdtvplayback.switchToMainVideoScaler = function (){
		Authentication.Debug(">>>> oMediasetting.switchToMainVideoScaler <<<<<<");
	};
	oJsdtvplayback.releaseDTVEngSource = function (){
		Authentication.Debug(">>>> oMediasetting.releaseDTVEngSource <<<<<<");
	};
	oJsdtvplayback.removeMainVideoScaler = function (){
		Authentication.Debug(">>>> oMediasetting.removeMainVideoScaler <<<<<<");
	};
	oJsdtvplayback.stopPlay = function (){
		Authentication.Debug(">>>> oMediasetting.stopPlay <<<<<<");
	};
};