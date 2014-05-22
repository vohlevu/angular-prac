
function Main (){
    var oMain = this;
	Authentication.Debug("####--------init Main ... ");
	
    oMain.keyController       = null;         // keyController
	oMain.parentPageElement   = $("body").children("div");                          // $("#Wrapper") - element
    oMain.parentPageName      = oMain.parentPageElement.attr("id");                 // Wrapper - stringID
    
    oMain.coldPageElement     = oMain.parentPageElement.children('div').eq(0);
    oMain.stackInfoPage       = [];           // stack
	
	oMain.isInitDTV = false;
	oMain.isInitDTVAmp = false;
	
	oMain.start = function (){ 
		Authentication.Debug("####--------oMain.start ... ");
		oMain.setKeyControl();
		oJSExtensions.init();
		oJSExtensions.start();
		var oVietnamTV = new VietnamTV();
		oVietnamTV.start();
		var oEpg = new Epg(oVietnamTV);
		var item = $("<div></div>").data("url",oEpg).data('type','object');
		oMain.transitionPage(item,oMain.coldPageElement);
	};

	/*
	 * Param item: element contain object page
	 * Param pageWrap: element will contain new page
	 */
	oMain.transitionPage = function(item,pageWrap){
		Authentication.Debug("####--------transitionPage-------typeItemObject: ");
		var typeItemObject  = $(item).data('type');
		var itemObject      = eval($(item).data('url'));
		//var name      		= $(item).data('name');
		if (!itemObject || itemObject == null) { return; }
		if(typeItemObject == 'object'){
			if(!pageWrap) pageWrap = oMain.coldPageElement;

			oMain.keyController.removeAllZones();
				   
			pageWrap.children().css({
				opacity : 1,
				display : 'none'
			});
			oMain.pushObjToStack(itemObject.pageID, itemObject);
			itemObject.pageElement = $("<div></div>").attr("id",itemObject.pageID)
													.css({  "position"  : "absolute",
															"z-index"   : oMain.stackInfoPage.length,
															"left"      : "0px",
															"top"       : "0px",
															"width"     : pageWrap.css("width"),
															"height"    : pageWrap.css("height")
														});
			pageWrap.append(itemObject.pageElement);
			itemObject.start(oMain);
		}
	};
	
	/*
	 * Initialize keyController
	 */
	oMain.setKeyControl = function (){
		if(!oMain.keyController){
			oMain.keyController = new gtv.jq.KeyController();
		}
	};
	oMain.pushObjToStack = function(pageName,item) {
		var objItem = {
				pageID      : pageName,          // page name
				itemObj     : item               // object
			};
		oMain.stackInfoPage.unshift(objItem); // push item to top of stack
	};
};
var main = new Main();
$(document).ready(function() {
	main.start();
	var socket = io.connect('http://localhost:8000');
	socket.emit('create', 'tcpremote');
    socket.on('greeting', function(data){
		alert(data);
    });
    socket.on('event', function(data){
		alert("Receive event from server : " + data);
    });
});