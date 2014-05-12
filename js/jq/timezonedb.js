function TimeZoneDB(){
	this.getJSON = function(obj, success){
		try{
			var key = obj.key,  lat = obj.lat, lng = obj.lng;

			var uid = "_" + new Date().getTime() + Math.floor(Math.random() * 10000), script = document.createElement('script');
			
			window[uid] = function(data){
				success && success(data);
				
				try{
					delete window[uid];
				}
				catch(e){
					window[uid] = undefined;
				}
			}
			// Make JSONP request

			script.src = "http://api.timezonedb.com/?key=" + key + "&lat=" + lat + "&lng=" + lng + "&format=json&callback=" + uid;
			
			document.body.appendChild(script);

			setTimeout(function(){
				script.parentNode.removeChild(script);
			},5000);
		}
		catch(e){
			oGlobal.time_gmtOffset					= "+3";
			oGlobal.time_dst						= "0";
			//alert("JSON request failed.");
			return false;
		}
	}
}