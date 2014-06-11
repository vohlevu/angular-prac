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
	// The dynamically built HTML pages. In a real-life app, In a real-life app, use Handlerbar.js, Mustache.js or another template engine
	var homePage =
		'<div>' +
			'<div class="body">' +
				'<div class="header"><div class="title">STB Remote</div></div>' +
				'<ul class="list">' +
					'<li><a href="#page1">Remote Layout</a></li>' +
					'<li><a href="#page2">Medi Bot</strong></a></li>' +
					'<li><a href="#page3">Ripple Bot</a></li>' +
				'</ul>' +
			'</div>' +
		'</div>';

	var detailsPage =
		'<div>' +
			'<div class="body">' +
				'<div class="robot">' +
					'<img src="img/{{img}}"/>' +
					'<h2>{{name}}</h2>' +
					'<p>{{description}}</p>' +
				'</div>' +
			'</div>' +
		'</div>';

	var slider = new PageSlider($("#app"));
	var key = "";
	var host = "10.0.2.2";
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
    // Application Constructor
    oApp.initialize = function() {
        console.log('>>>>>>>>>>>>>>>> oApp.initialize <<<<<<<<<<<<<<<<');
        oApp.bindEvents();
		oApp.route();
		arrRemoteBtn = oApp.transform( arrRemoteBtn );
    };
    // Bind Event Listeners
    //
    // Bind any events that are required on startup. Common events are:
    // 'load', 'deviceready', 'offline', and 'online'.
    oApp.bindEvents = function() {
        //document.addEventListener('deviceready', oApp.onDeviceReady, false);
    };
    // deviceready Event Handler
    oApp.onDeviceReady = function() {
        oApp.receivedEvent('deviceready');
    };
	oApp.goHome = function(event) {
		window.location.hash="#";
		event.preventDefault();
		return false;
    };
	// Basic page routing
	oApp.route = function(event) {
		var page,
			hash = window.location.hash;

		if (hash === "#page1") {
			var source   = $("#remote-template").html();
			var template = Handlebars.compile(source);
			page = template(arrRemoteBtn);
			oApp.connect();
		} else if (hash === "#page2") {
			page = oApp.merge(detailsPage, {img: "medibot.jpg", name: "Medi Bot", description: "Lorem Ipsum"});
		} else if (hash === "#page3") {
			page = oApp.merge(detailsPage, {img: "ripplebot.jpg", name: "Ripple Bot", description: "Lorem Ipsum"});
			window.discovery.start(oApp.stub, oApp.stub);
			window.discovery.setShowToastCallback(oApp.showToast, oApp.stub);
			window.discovery.getNetworkInfo(oApp.getNetworkInfo, oApp.stub);
		} else {
			page = homePage;
		}
		slider.slidePage($(page));
		$('a.img-contain').click(function(event){
			oApp.send("Send from Phonegap App >> id:" + this.id);
			//alert(this.id);
			event.preventDefault();
		});
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
		window.plugins.toast.show(message, 'short', 'bottom', 
				function(a){console.log('toast success: ' + a)},
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
$(window).on('hashchange', function(){
	app.route();
});
window.addEventListener('load', function () {
    new FastClick(document.body);
	document.body.addEventListener('touchmove', function(e) {
									e.preventDefault();
								}, false);
}, false);
