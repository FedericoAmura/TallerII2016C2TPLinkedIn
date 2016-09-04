var express = require('express');
var app = express();
app.use(express.static('public'));

//Configuracion express
app.set('port', (process.env.PORT || 5000));

//seteamos rutas y respuestas del server en estas
//Login
app.get('/', function (req, res) {
	res.sendFile(__dirname+'/public/index.html');
});

app.get('/login', function (req, res) {
	response = {
			first_name:req.query.first_name,
			last_name:req.query.last_name
	};
	console.log(response);
	res.end(JSON.stringify(response));
});

//Database Manager
app.get('/manager', function (req, res) {
	res.sendFile(__dirname+'/public/databaseManager/index.html');
});

//Categorias
//Get categorias
app.get('/categories', function (req, res) {
	res.send('{"categories":[{"name":"software","description":"software description"},{"name":"music",“description":"all kind of music"}],"metadata":{"version":"0.1","count":2}}');
});

//Alta categoria segun json
app.post('/categories', function (req, res) {
	res.status(201);
	res.send('{"category":{"name":"sport",“description":"sport activities"}}');
});

//Modificacion de categoria SIN USO
app.post('/categories/:category', function (req, res) {
	res.status(201);
	res.send('{"category":{"name":"outdoor activies","description":"all kind of outdoor activities"}}');
});

//Baja de categoria SIN USO
app.delete('/categories/:category', function (req, res) {
	res.status(204);
})

//Get JobPositions
app.get('/job_positions', function (req, res) {
	res.send('{"job_positions":[{"name":"developer","description":"a software developer","category":"software"},{"name":"project manager","description":"a project manager","category":"management"},{"name":"dj","category":"music"}],"metadata":{"version":"0.1","count":3}}');
});

//Get JobPositions de una categoria
app.get('/job_positions/categories/:category', function (req, res) {
	res.send('{"job_positions":[{"name":"developer","description":"a software developer","category":"software"}],"metadata":{"version":"0.1","count":1}}');
});

//Alta JobPosition segun JSON
app.post('/job_positions/categories/:category', function (req, res) {
	res.status(201);
	res.send('"job_position":{"name":"developer","description":"a software developer","category":"software"}');
});

//Modificacion de una JobPosition segun JSON
app.put('/job_positions/categories/:category/:position', function (req, res) {
	res.status(200);
	res.send('{"job_position":{"name":"developer","description":"a software developer","category":"software"}}');
});

//Baja de una JobPosition SIN USO
app.delete('/job_positions/categories/:category/:position', function (req, res) {
	res.status(204);
});

//Get Skills
app.get('/skills', function (req, res) {
	res.send('{"job_positions":[{"name":"developer","description":"a software developer","category":"software"},{"name":"project manager","description":"a project manager","category":"management"},{"name":"dj","category":"music"}],"metadata":{"version":"0.1","count":3}}');
});

//Get Skills de una categoria
app.get('/skills/categories/:category', function (req, res) {
	res.send('{"job_positions":[{"name":"developer","description":"a software developer","category":"software"}],"metadata":{"version":"0.1","count":1}}');
});

//Alta Skill segun JSON
app.post('/skills/categories/:category', function (req, res) {
	res.status(201);
	res.send('"job_position":{"name":"developer","description":"a software developer","category":"software"}');
});

//Modificacion de una Skill segun JSON
app.put('/skills/categories/:category/:skill', function (req, res) {
	res.status(200);
	res.send('{"job_position":{"name":"developer","description":"a software developer","category":"software"}}');
});

//Baja de una Skill SIN USO
app.delete('/skills/categories/:category/:skill', function (req, res) {
	res.status(204);
});


//activamos server
var server = app.listen(app.get('port'), function () {
	var host = server.address().address;
	var port = server.address().port;

	console.log("Server listening at http://%s:%s", host, port);
});