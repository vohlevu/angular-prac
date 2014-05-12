var oJSExtensions = {};

oJSExtensions.mediasetting 			= null;
oJSExtensions.dtvplayback_obj 			= null;
oJSExtensions.epg_obj					= null;


oJSExtensions.init = function () {
	oJSExtensions.mediasetting = new jsmediasetting();
	oJSExtensions.epg_obj = new jsdtvepg();
	oJSExtensions.dtvplayback_obj = new jsdtvplayback();
};

oJSExtensions.start = function () {
	
};

oJSExtensions.setEncoderAspectRatio = function (strValue) {
	
};

oJSExtensions.setLPBIndexMode = function (iValue) {
	
};

function jsdtvepg(){
	var oJsdtvepg = this;
	oJsdtvepg.showEPG = function (){
		Authentication.Debug(">>>> oMediasetting.showEPG <<<<<<");
	};
	oJsdtvepg.getTime = function (){
		Authentication.Debug(">>>> oMediasetting.getTime <<<<<<");
	};
	oJsdtvepg.SetWndRect = function (){
		Authentication.Debug(">>>> oMediasetting.SetWndRec <<<<<<");
	};
	oJsdtvepg.setEpgGetMode = function (){
		Authentication.Debug(">>>> oMediasetting.setEpgGetMode <<<<<<");
	};
};
function jsmediasetting(){
	var oJsmediasetting = this;
	oJsmediasetting.SetWndRect = function (){
		Authentication.Debug(">>>> oMediasetting.SetWndRec <<<<<<");
	};
	oJsmediasetting.setEpgGetMode = function (){
		Authentication.Debug(">>>> oMediasetting.setEpgGetMode <<<<<<");
	};
};
function jsdtvplayback(){
	var oJsdtvplayback = this;
	oJsdtvplayback.SetWndRect = function (){
		Authentication.Debug(">>>> oMediasetting.SetWndRec <<<<<<");
	};
	oJsdtvplayback.setEpgGetMode = function (){
		Authentication.Debug(">>>> oMediasetting.setEpgGetMode <<<<<<");
	};
};