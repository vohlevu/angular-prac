var discovery =  {
    start: function(successCallback, errorCallback) {
		//console.log(">>>>>>>>>>>> discovery.start js <<<<<<<<<<<<");
        cordova.exec(
            successCallback, // success callback function
            errorCallback, // error callback function
            'Discovery', // mapped to our native Java class called "Discovery"
            'start', // with this action name
            []	 // and this array of custom arguments to create our entry
        );
    }
}
module.exports = discovery;