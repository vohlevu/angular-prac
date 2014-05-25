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
var app = {
	// The dynamically built HTML pages. In a real-life app, In a real-life app, use Handlerbar.js, Mustache.js or another template engine
	homePage :
		'<div>' +
			'<header><h1>Page Slider</h1></header>' +
			'<div class="body">' +
				'<ul class="list">' +
					'<li><a href="#page1"><strong>Remote Layout</strong></a></li>' +
					'<li><a href="#page2"><strong>Medi Bot</strong></a></li>' +
					'<li><a href="#page3"><strong>Ripple Bot</strong></a></li>' +
				'</ul>' +
			'</div>' +
		'</div>',

	detailsPage :
		'<div>' +
			'<header><a href="#" class="btn">Back</a><h1>Robot</h1></header>' +
			'<div class="body">' +
				'<div class="robot">' +
					'<img src="img/{{img}}"/>' +
					'<h2>{{name}}</h2>' +
					'<p>{{description}}</p>' +
				'</div>' +
			'</div>' +
		'</div>',

	slider : new PageSlider($("#app")),
    // Application Constructor
    initialize: function() {
        this.bindEvents();
		this.route();
    },
    // Bind Event Listeners
    //
    // Bind any events that are required on startup. Common events are:
    // 'load', 'deviceready', 'offline', and 'online'.
    bindEvents: function() {
        //document.addEventListener('deviceready', this.onDeviceReady, false);
    },
    // deviceready Event Handler
    //
    // The scope of 'this' is the event. In order to call the 'receivedEvent'
    // function, we must explicity call 'app.receivedEvent(...);'
    onDeviceReady: function() {
        app.receivedEvent('deviceready');
    },
	goHome: function(event) {
		window.location.hash="#";
		event.preventDefault();
		return false;
    },
	// Basic page routing
	route : function(event) {
		var page,
			hash = window.location.hash;

		if (hash === "#page1") {
			//page = this.merge(this.detailsPage, {img: "buildbot.jpg", name: "Build Bot", description: "Lorem Ipsum"});
	//        slider.slide($(page), "right");
			var source   = $("#remote-template").html();
			var template = Handlebars.compile(source);
			page = template();
		} else if (hash === "#page2") {
			page = this.merge(this.detailsPage, {img: "medibot.jpg", name: "Medi Bot", description: "Lorem Ipsum"});
	//        slider.slide($(page), "right");
		} else if (hash === "#page3") {
			page = this.merge(this.detailsPage, {img: "ripplebot.jpg", name: "Ripple Bot", description: "Lorem Ipsum"});
	//        slider.slide($(page), "right");
		} else if (hash === "#btnPower") {
			alert("btnPower");
			return;
		} else {
			page = this.homePage;
	//        slider.slide($(homePage), "left");
		}
		this.slider.slidePage($(page));
	},
	merge : function(tpl, data) {
		return tpl.replace("{{img}}", data.img)
				  .replace("{{name}}", data.name)
				  .replace("{{description}}", data.description);
	},
    // Update DOM on a Received Event
    receivedEvent: function(id) {
        var parentElement = document.getElementById(id);
        var listeningElement = parentElement.querySelector('.listening');
        var receivedElement = parentElement.querySelector('.received');

        listeningElement.setAttribute('style', 'display:none;');
        receivedElement.setAttribute('style', 'display:block;');

        console.log('Received Event: ' + id);
    }
};
$(window).on('hashchange', function(){app.route();});
window.addEventListener('load', function () {
    new FastClick(document.body);
}, false);
