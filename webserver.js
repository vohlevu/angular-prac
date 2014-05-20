// A very basic web server in node.js
// Stolen from: Node.js for Front-End Developers by Garann Means (p. 9-10) 

var port = 8000;
var serverUrl = "127.0.0.1";

var http = require("http");
var path = require("path"); 
var fs = require("fs");
var querystring = require('querystring');
var controller = require('./mymodules/build/Release/controller');

console.log("Starting web server at " + serverUrl + ":" + port);

http.createServer( function(req, res) {
	
	//console.log("Request method : " + req.method);
	if(req.method == "POST") {
		processPost(req, res, function() {
			console.log("Call API : " + req.post.func);
			var funcResult = eval("controller." + req.post.func + "()");
            // Use request.post here

            res.writeHead(200, "OK", {'Content-Type': 'text/json'});
			var returnObj = {status: "OK", result: funcResult};
			var json = JSON.stringify(returnObj);
			console.log("Response data : " + json);
            res.end(json);
        });
	} else {
		var now = new Date();

		var filename = req.url || "index.html";
		var ext = path.extname(filename);
		var localPath = __dirname;
		var validExtensions = {
			".html" : "text/html",			
			".js": "application/javascript", 
			".css": "text/css",
			".txt": "text/plain",
			".jpg": "image/jpeg",
			".gif": "image/gif",
			".png": "image/png",
			".json": "json"
		};
		var isValidExt = validExtensions[ext];

		if (isValidExt) {
			
			localPath += filename;
			path.exists(localPath, function(exists) {
				if(exists) {
					console.log("Serving file: " + localPath);
					getFile(localPath, res, isValidExt);
				} else {
					console.log("File not found: " + localPath);
					res.writeHead(404);
					res.end();
				}
			});

		} else {
			console.log("Invalid file extension detected: " + ext)
		}
	}
}).listen(port, serverUrl);

function getFile(localPath, res, mimeType) {
	fs.readFile(localPath, function(err, contents) {
		if(!err) {
			res.setHeader("Content-Length", contents.length);
			res.setHeader("Content-Type", mimeType);
			res.statusCode = 200;
			res.end(contents);
		} else {
			res.writeHead(500);
			res.end();
		}
	});
}
function processPost(request, response, callback) {
    var queryData = "";
    if(typeof callback !== 'function') return null;

    if(request.method == 'POST') {
        request.on('data', function(data) {
            queryData += data;
            if(queryData.length > 1e6) {
                queryData = "";
                response.writeHead(413, {'Content-Type': 'text/plain'}).end();
                request.connection.destroy();
            }
        });

        request.on('end', function() {
            request.post = querystring.parse(queryData);
            callback();
        });

    } else {
        response.writeHead(405, {'Content-Type': 'text/plain'});
        response.end();
    }
}
