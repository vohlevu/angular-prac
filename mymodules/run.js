var controller = require('./bin/controller');

var socketCallbackFunc = function (data) {
	if ("" != data) {
		console.warn("run.js >>>> socketCallbackFunc : " + data);
	}
}

controller.registerSocketCallbackFunc(socketCallbackFunc);
//console.warn(controller.fibonacci(9));
var totalChannel = controller.getTotalChannel();
for (var i = 0; i < totalChannel; i++) {
	console.warn("run.js >>>> " + controller.getChannelNameById(i));
}
