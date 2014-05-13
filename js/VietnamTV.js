function VietnamTV (){
	var oVietnamTV = this;
	var oDataFavoriteChannels	= new Object();
	var iCurrChannel;
	
	oVietnamTV.start = function(){
		oVietnamTV.loadFavoriteChannels();
		iCurrChannel = 0;
	};

	oVietnamTV.loadFavoriteChannels = function(){
		Authentication.Debug(">>>> oVietnamTV.loadFavoriteChannels <<<<<<");
		$.ajax({
			url:"/userdata/FavoriteChannels.json",
			async:false,
			dataType:"json",
			success:function(data) {
				if(data.Data != null && data.Data != "" && data.Data != undefined){
					oDataFavoriteChannels = data;
				}else{
					Authentication.Debug("EPG >> loadFavoriteChannels >> ERROR");
				}
			},
			error: function () {
				Authentication.Debug("EPG >> loadFavoriteChannels >> ERROR");
			}
		});
	};
	
	oVietnamTV.getFavoriteChannels = function(){
		Authentication.Debug(">>>> oVietnamTV.getFavoriteChannels <<<<<<");
		return oDataFavoriteChannels;
	};
	
	oVietnamTV.clearTimeoutPlayOTT = function(){
		Authentication.Debug(">>>> oVietnamTV.clearTimeoutPlayOTT <<<<<<");
	};
	
	oVietnamTV.clearAllInterval = function(){
		Authentication.Debug(">>>> oVietnamTV.clearAllInterval <<<<<<");
	};
	
	oVietnamTV.getCurrChannel = function(){
		Authentication.Debug(">>>> oVietnamTV.getCurrChannel <<<<<< iCurrChannel:" + iCurrChannel);
		return iCurrChannel;
	};
	
	oVietnamTV.setCurrChannel = function(i){
		Authentication.Debug(">>>> oVietnamTV.setCurrChannel <<<<<< iCurrChannel:" + iCurrChannel);
		iCurrChannel = i;
	};
}