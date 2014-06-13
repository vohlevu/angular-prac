var discovery =  {
    startDiscovery: function(successCallback, errorCallback) {
        cordova.exec(
            successCallback, // success callback function
            errorCallback, // error callback function
            'Discovery', // mapped to our native Java class called "Discovery"
            'startDiscovery', // with this action name
            []	 // and this array of custom arguments to create our entry
        );
    },
    stopDiscovery: function(successCallback, errorCallback) {
        cordova.exec(
            successCallback, // success callback function
            errorCallback, // error callback function
            'Discovery', // mapped to our native Java class called "Discovery"
            'stopDiscovery', // with this action name
            []	 // and this array of custom arguments to create our entry
        );
    },
    getNetworkInfo: function(successCallback, errorCallback) {
        cordova.exec(
            successCallback,
            errorCallback,
            'Discovery',
            'getNetworkInfo',
            []
        );
    },
    setShowToastCallback: function(showToastCallback, errorCallback) {
        cordova.exec(
            showToastCallback,
            errorCallback,
            'Discovery',
            'setShowToastCallback',
            []
        );
    }
}
module.exports = discovery;