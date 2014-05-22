var controller = require('./bin/controller');

//console.warn(controller.fibonacci(9));
var totalChannel = controller.getTotalChannel();
for (var i = 0; i < totalChannel; i++) {
	console.warn("run.js >>>> " + controller.getChannelNameById(i));
}
