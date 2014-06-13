/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
function app() {
	var oApp = this;
	var detailsPage =
		'<div data-role="page" id="page2" class="page">' +
			'<div class="body">' +
				'<div class="robot">' +
					'<img src="img/{{img}}"/>' +
					'<h2>{{name}}</h2>' +
					'<p>{{description}}</p>' +
				'</div>' +
			'</div>' +
		'</div>';
	var arrRemoteBtn = [
		{ id: 'btnPower', img: 'img/btn/power.png' },
		{ id: 'btnReboot', img: 'img/btn/reboot.png' },
		{ id: 'btnMute', img: 'img/btn/mute.png' },
		{ id: 'btnVolDown', img: 'img/btn/voldown.png' },
		{ id: 'btnVolUp', img: 'img/btn/volup.png' },
		{ id: 'btnNum1', img: 'img/btn/num1.png' },
		{ id: 'btnNum2', img: 'img/btn/num2.png' },
		{ id: 'btnNum3', img: 'img/btn/num3.png' },
		{ id: 'btnNum4', img: 'img/btn/num4.png' },
		{ id: 'btnNum5', img: 'img/btn/num5.png' },
		{ id: 'btnNum6', img: 'img/btn/num6.png' },
		{ id: 'btnNum7', img: 'img/btn/num7.png' },
		{ id: 'btnNum8', img: 'img/btn/num8.png' },
		{ id: 'btnNum9', img: 'img/btn/num9.png' },
		{ id: 'btnNum0', img: 'img/btn/num0.png' },
		{ id: 'btnPlay', img: 'img/btn/play.png' },
		{ id: 'btnMenu', img: 'img/btn/menu.png' },
		{ id: 'btnUp', img: 'img/btn/up.png' },
		{ id: 'btnHome', img: 'img/btn/home.png' },
		{ id: 'btnDelete', img: 'img/btn/delete.png' },
		{ id: 'btnPause', img: 'img/btn/pause.png' },
		{ id: 'btnLeft', img: 'img/btn/left.png' },
		{ id: 'btnOk', img: 'img/btn/ok.png' },
		{ id: 'btnRight', img: 'img/btn/right.png' },
		{ id: 'btnCapsnum', img: 'img/btn/capsnum.png' },
		{ id: 'btnStop', img: 'img/btn/stop.png' },
		{ id: 'btnInfo', img: 'img/btn/info.png' },
		{ id: 'btnDown', img: 'img/btn/down.png' },
		{ id: 'btnEpg', img: 'img/btn/epg.png' },
		{ id: 'btnMouse', img: 'img/btn/mouse.png' },
		{ id: 'btnPrev', img: 'img/btn/prev.png' },
		{ id: 'btnRev', img: 'img/btn/rev.png' },
		{ id: 'btnNext', img: 'img/btn/next.png' },
		{ id: 'btnFwd', img: 'img/btn/fwd.png' },
		{ id: 'btnKeyboard', img: 'img/btn/keyboard.png' },
		{ id: 'btnRed', img: 'img/btn/red.png' },
		{ id: 'btnGreen', img: 'img/btn/green.png' },
		{ id: 'btnYellow', img: 'img/btn/yellow.png' },
		{ id: 'btnBlue', img: 'img/btn/blue.png' },
		{ id: 'btnBack', img: 'img/btn/back.png' }
	];
	var key = "";
	var host = "10.0.2.2";
	
    // Application Constructor
	oApp.initialize = function() {
        console.log('>>>>>>>>>>>>>>>> oApp.initialize <<<<<<<<<<<<<<<<');
		oApp.appendPage1();
		oApp.appendPage2();
        oApp.bindEvents();
    };
	
	oApp.appendPage1 = function() {
		var arrTmp = oApp.transform( arrRemoteBtn );
		var source = $("#remote-template").html();
		var template = Handlebars.compile(source);
		var page = $(template(arrTmp));
		page.appendTo("#app"/*$.mobile.pageContainer*/);
    };
	
	oApp.appendPage2 = function() {
		var page = $(oApp.merge(detailsPage, {img: "ripplebot.jpg", name: "Ripple Bot", description: "Lorem Ipsum"}));
		page.appendTo("#app"/*$.mobile.pageContainer*/);
    };
	
    oApp.bindEvents = function() {
        document.addEventListener('deviceready', oApp.onDeviceReady, false);
    };
	
    oApp.onDeviceReady = function() {
		$(document).bind('backbutton', onPressBack);
        //oApp.receivedEvent('deviceready');
    };
	
	oApp.showList = function() {
		var length = arrRemoteBtn.length
		for (var i = 0 ; i < length ; i++) {
			// <img src="'+arrRemoteBtn[i].img+'"/>
			$('#list-wrapper div .iscroll-content').append('<li><a href=""><h3>' + arrRemoteBtn[i].id + '</h3></a></li>');
		}
		$('#list-wrapper').trigger('create');
		$('#list-wrapper').iscrollview("refresh");
    };
	
	oApp.merge = function(tpl, data) {
		return tpl.replace("{{img}}", data.img)
				  .replace("{{name}}", data.name)
				  .replace("{{description}}", data.description);
	};
	
    // Update DOM on a Received Event
    oApp.receivedEvent = function(id) {
        console.log('Received Event: ' + id);
    };
	
    oApp.showToast = function(message) {
		console.log('index.js >> showToast : ' + message)
		window.plugins.toast.show(message, 'short', 'bottom', 
				function(a){/*console.log('toast success: ' + a)*/},
				function(b){console.log('toast error: ' + b)});
	};
	
    oApp.getNetworkInfo = function(data) {
		console.log(data.IP);
		console.log(data.SSID);
	};
	
    oApp.stub = function(message) {
		console.log(message);
		//adb logcat *:S CordovaLog:D
	};
	
    oApp.connect = function() {
		window.tlantic.plugins.socket.connect(oApp.onConnect, oApp.stub, host, 18002);
	};
	
    oApp.send = function(data) {
		window.tlantic.plugins.socket.send(oApp.stub, oApp.stub, key, data);
	};
	
    oApp.onConnect = function(k) {
		key = k;
	};
	
    oApp.disconnect = function() {
		window.tlantic.plugins.socket.disconnect(oApp.stub, oApp.stub, key);
	};
	
    oApp.isConnected = function() {
		window.tlantic.plugins.socket.isConnected(key, oApp.stub, oApp.stub);
	};
	
	oApp.transform = function ( arr ) {
		var result = [], temp = [];
		arr.forEach( function ( elem, i ) {
			if ( i > 0 && i % 5 === 0 ) {
				result.push( temp );
				temp = [];
			}
			temp.push( elem );
		});
		if ( temp.length > 0 ) {
			result.push( temp );
		}
		return result;
	};	
};
$(document).on('vclick', '#list-wrapper li a', function(){
	
	//event.preventDefault();
	return false;
});
$(document).on('vclick', 'a.img-contain', function(){
	app.send("Send from Phonegap App >> id:" + this.id);
	//event.preventDefault();
	return false;
});
$(document).on('vclick', '#home-menu-list li a', function(){
	var hash = $(this).attr('href');
	$.mobile.changePage( hash, { transition: "slide", changeHash: false });
});
$(document).bind( "pagebeforechange", function( e, data ) {
	if ( typeof data.toPage === "string" ) {
		var u = $.mobile.path.parseUrl( data.toPage );
		if ( u.hash.search(/^#page1/) !== -1 ) {
			app.connect();
		} else if ( u.hash.search(/^#page2/) !== -1 ) {
			window.discovery.setShowToastCallback(app.showToast, app.stub);
			window.discovery.getNetworkInfo(app.getNetworkInfo, app.stub);
			window.discovery.start(app.stub, app.stub);
		} else if ( u.hash.search(/^#page3/) !== -1 ) {
			//app.showList();
		}
	}
});
$(document).on('pagebeforeshow', '#page3', function(){
	app.showList();
});
window.addEventListener('load', function () {
    new FastClick(document.body);
	document.body.addEventListener('touchmove', function(e) {
									e.preventDefault();
								}, false);
}, false);

// Handle back button press
var pageHistoryCount = 0;
var goingBack = false;
//var stackPage = new Array();

$(document).bind("pageshow", function showPage (e, data) {
	if (goingBack) {
		goingBack = false;
	} else {
		pageHistoryCount++;
		console.log("Showing page #"+pageHistoryCount);
	}
});

function exitApp () {
	console.log("Exiting app");
	navigator.app.exitApp();
};

function onPressBack (e) {
	e.preventDefault();
	if(pageHistoryCount > 0) pageHistoryCount--;
	if (pageHistoryCount == 0) {

		navigator.notification.confirm("Are you sure you want to quit?", function(result){
			if(result == 2){
				exitApp();
			}else{
				pageHistoryCount++;
			}
		}, 'Quit My App', 'Cancel,Ok');
	} else {
		goingBack = true;
		console.log("Going back to page #"+pageHistoryCount);
		//navigator.app.backHistory();
		$.mobile.changePage("#homepage", {transition: 'slide',	reverse: true });	//$.mobile.back();
	}
};