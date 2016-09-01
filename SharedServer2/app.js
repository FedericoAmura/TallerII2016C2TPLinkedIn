var express = require("express");
//var app = module.exports = express.createServer();
var app = express.createServer();

app.get('/', function(req, res) { 
  res.send('Hola. Probando express.js');
});

app.get('/prueba', function(req, res) {
  res.send('Hola. Probando express.js en /prueba');
});

app.listen(3000);
console.log("Servidor creado. Puerto:", app.address().port);

/* No le den bola a esto.
 *
 * Module dependencies.
 */
/*
var express = require('express')
  , routes = require('./routes');

var app = module.exports = express.createServer();

// Configuration

app.configure(function(){
  app.set('views', __dirname + '/views');
  app.set('view engine', 'jade');
  app.use(express.bodyParser());
  app.use(express.methodOverride());
  app.use(app.router);
  app.use(express.static(__dirname + '/public'));
});

app.configure('development', function(){
  app.use(express.errorHandler({ dumpExceptions: true, showStack: true }));
});

app.configure('production', function(){
  app.use(express.errorHandler());
});

// Routes

app.get('/', routes.index);

app.listen(3000, function(){
  console.log("Express server listening on port %d in %s mode", app.address().port, app.settings.env);
});
*/
