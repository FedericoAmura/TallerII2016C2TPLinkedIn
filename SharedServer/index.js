
var http = require('http');
var serveStatic = require('serve-static');

/* https://github.com/expressjs/serve-static#serve-files-with-vanilla-nodejs-http-server
 * Serve up public/ folder */
var servePublic = serveStatic('public', {'index': ['index.html', 'index.htm']});

http.createServer(function handler(req, res) {
	console.log(req.method, req.url, 'HTTP'+req.httpVersion, req.headers); // , req is too long

	res.setHeader('Access-Control-Allow-Origin', '127.0.0.1');
    
    servePublic(req, res, function nextHandler(req, res){
    });    
}).listen(5000, '127.0.0.1');
console.log('Server running at http://127.0.0.1:5000/');
