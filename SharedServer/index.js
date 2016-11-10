var express = require('express');
var app = express();
app.use(express.static('public'));
var pg = require('pg');
var bodyParser = require("body-parser");

var pgurl = process.env.DATABASE_URL || "postgresql://postgres_user:password@127.0.0.1:5432/jobify_db";
var port = process.env.PORT || 5000;

var loginUsername = "admin";
var loginPassword = "admin";

var appversion = 0.2;

//Configuracion express
app.set('port', port);
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: true}));

function errPGConn(err, res) {
	console.log(err);
	res.send('{"error":'+err+'}');
	res.status(409);
}

function noContentTypeJSON(req, res) {
	if (req.get('Content-Type').search('application/json') < 0) {
		res.send('{"error":"Content Type must be application/json"}');
		res.status(415);
		return true;
	}
	return false;
}

//seteamos rutas y respuestas del server en estas
//Login
app.get('/', function (req, res) {
	res.sendFile(__dirname+'/public/index.html');
});

app.post('/', function (req, res) {
	if (loginUsername==req.body.username && loginPassword==req.body.password) {
		res.sendFile(__dirname+'/public/app.html');
	} else {
		res.sendFile(__dirname+'/public/index.html');
		console.log("Wrong login with: "+req.body.username+":"+req.body.password);
	}
});

//Categorias
//Get categorias
app.get('/categories', function (req, res) {
	pg.connect(pgurl, function (err, client, done) {
		if (err) {
			errPGConn(err, res);
			return;
		}
		client.query('SELECT * FROM categories', function (err, result) {
			done();
			if (err) {
				errPGConn(err, res);
				return;
			}
			res.send({categories: result.rows,metadata:{version:appversion,count:result.rows.length}});
			res.status(200);
		});
	});
});

//Alta categoria segun json
app.post('/categories', function (req, res) {
	if (noContentTypeJSON(req, res)) return;
	pg.connect(pgurl, function (err, client, done) {
		if (err) {
			errPGConn(err, res);
			return;
		}
		client.query('INSERT INTO categories (name, description) VALUES (\''+req.body.category.name+'\',\''+req.body.category.description+'\')', function (err, result) {
			done();
			if (err) {
				errPGConn(err, res);
				return;
			}
			res.send(req.body);
			res.status(201);
		});
	});
});

//Modificacion de categoria SIN USO
app.put('/categories/:categoria', function (req, res) {
	if (noContentTypeJSON(req, res)) return;
	pg.connect(pgurl, function (err, client, done) {
		if (err) {
			errPGConn(err, res);
			return;
		}
		client.query('UPDATE categories SET (name, description) = (\''+req.body.category.name+'\',\''+req.body.category.description+'\') WHERE name=\''+req.params.categoria+'\'', function (err, result) {
			done();
			if (err) {
				errPGConn(err, res);
				return;
			}
			res.send(req.body);
			res.status(201);
		});
	});
});

//Baja de categoria SIN USO
app.delete('/categories/:categoria', function (req, res) {
	pg.connect(pgurl, function (err, client, done) {
		if (err) {
			errPGConn(err, res);
			return;
		}
		client.query('DELETE FROM categories WHERE name=\''+req.params.categoria+'\'', function (err, result) {
			done();
			if (err) {
				errPGConn(err, res);
				return;
			}
			res.status(204);
			res.end();
		});
	});
})


//Get JobPositions
app.get('/job_positions', function (req, res) {
	pg.connect(pgurl, function (err, client, done) {
		if (err) {
			errPGConn(err, res);
			return;
		}
		client.query('SELECT * FROM jobs', function (err, result) {
			done();
			if (err) {
				errPGConn(err, res);
				return;
			}
			res.send({job_positions:result.rows,metadata:{version:appversion,count:result.rows.length}});
			res.status(200);
		});
	});
});

//Get JobPositions de una categoria
app.get('/job_positions/categories/:categoria', function (req, res) {
	pg.connect(pgurl, function (err, client, done) {
		if (err) {
			errPGConn(err, res);
			return;
		}
		client.query('SELECT * FROM jobs WHERE category=\''+req.params.categoria+'\'', function (err, result) {
			done();
			if (err) {
				errPGConn(err, res);
				return;
			}
			res.send({job_positions: result.rows,metadata:{version:appversion,count:result.rows.length}});
			res.status(200);
		});
	});
});

//Alta JobPosition segun JSON
app.post('/job_positions/categories/:categoria', function (req, res) {
	if (noContentTypeJSON(req, res)) return;
	pg.connect(pgurl, function (err, client, done) {
		if (err) {
			errPGConn(err, res);
			return;
		}
		client.query('INSERT INTO jobs (name, description, category) VALUES (\''+req.body.job_position.name+'\',\''+req.body.job_position.description+'\',\''+req.params.categoria+'\')', function (err, result) {
			done();
			if (err) {
				errPGConn(err, res);
				return;
			}
			res.send({job_position:{name:req.body.job_position.name,description:req.body.job_position.description,category:req.params.categoria}});
			res.status(201);
		});
	});
});

//Modificacion de una JobPosition segun JSON
app.put('/job_positions/categories/:categoria/:posicion', function (req, res) {
	if (noContentTypeJSON(req, res)) return;
	pg.connect(pgurl, function (err, client, done) {
		if (err) {
			errPGConn(err, res);
			return;
		}
		client.query('UPDATE jobs SET (name, description, category) = (\''+req.body.job_position.name+'\',\''+req.body.job_position.description+'\',\''+req.body.job_position.category+'\') WHERE name=\''+req.params.posicion+'\' AND category=\''+req.params.categoria+'\'', function (err, result) {
			done();
			if (err) {
				errPGConn(err, res);
				return;
			}
			res.send({job_position:{name:req.body.job_position.name,description:req.body.job_position.description,category:req.params.categoria}});
			res.status(200);
		});
	});
});

//Baja de una JobPosition SIN USO
app.delete('/job_positions/categories/:categoria/:posicion', function (req, res) {
	pg.connect(pgurl, function (err, client, done) {
		if (err) {
			errPGConn(err, res);
			return;
		}
		client.query('DELETE FROM jobs WHERE name=\''+req.params.posicion+'\' AND category=\''+req.params.categoria+'\'', function (err, result) {
			done();
			if (err) {
				errPGConn(err, res);
				return;
			}
			res.status(204);
			res.end();
		});
	});
});

//Get Skills
app.get('/skills', function (req, res) {
	pg.connect(pgurl, function (err, client, done) {
		if (err) {
			errPGConn(err, res);
			return;
		}
		client.query('SELECT * FROM skills', function (err, result) {
			done();
			if (err) {
				errPGConn(err, res);
				return;
			}
			res.send({skills: result.rows,metadata:{version:appversion,count:result.rows.length}});
		});
	});
});

//Get Skills de una categoria
app.get('/skills/categories/:categoria', function (req, res) {
	pg.connect(pgurl, function (err, client, done) {
		if (err) {
			errPGConn(err, res);
			return;
		}
		client.query('SELECT * FROM skills WHERE category=\''+req.params.categoria+'\'', function (err, result) {
			done();
			if (err) {
				errPGConn(err, res);
				return;
			}
			res.send({skills: result.rows,metadata:{version:appversion,count:result.rows.length}});
			res.status(200);
		});
	});
});

//Alta Skill segun JSON
app.post('/skills/categories/:categoria', function (req, res) {
	if (noContentTypeJSON(req, res)) return;
	pg.connect(pgurl, function (err, client, done) {
		if (err) {
			errPGConn(err, res);
			return;
		}
		client.query('INSERT INTO skills (name, description, category) VALUES (\''+req.body.skill.name+'\',\''+req.body.skill.description+'\',\''+req.params.categoria+'\')', function (err, result) {
			done();
			if (err) {
				errPGConn(err, res);
				return;
			}
			res.send({skill:{name:req.body.skill.name,description:req.body.skill.description,category:req.params.categoria}});
			res.status(201);
		});
	});
});

//Modificacion de una Skill segun JSON
app.put('/skills/categories/:categoria/:habilidad', function (req, res) {
	if (noContentTypeJSON(req, res)) return;
	pg.connect(pgurl, function (err, client, done) {
		if (err) {
			errPGConn(err, res);
			return;
		}
		client.query('UPDATE skills SET (name, description, category) = (\''+req.body.skill.name+'\',\''+req.body.skill.description+'\',\''+req.body.skill.category+'\') WHERE name=\''+req.params.habilidad+'\' AND category=\''+req.params.categoria+'\'', function (err, result) {
			done();
			if (err) {
				errPGConn(err, res);
				return;
			}
			res.send({skill:{name:req.body.skill.name,description:req.body.skill.description,category:req.params.categoria}});
			res.status(200);
		});
	});
});

//Baja de una Skill SIN USO
app.delete('/skills/categories/:categoria/:habilidad', function (req, res) {
	pg.connect(pgurl, function (err, client, done) {
		if (err) {
			errPGConn(err, res);
			return;
		}
		client.query('DELETE FROM skills WHERE name=\''+req.params.habilidad+'\' AND category=\''+req.params.categoria+'\'', function (err, result) {
			done();
			if (err) {
				errPGConn(err, res);
				return;
			}
			res.status(204);
			res.end();
		});
	});
});

app.use(function (req, res) {
	res.status(404)
	res.send('{"error":{"type":"resource not found","detail":"the requested reasource could not be found"}}')
});

//activamos server
var server = app.listen(app.get('port'), function () {
	var host = server.address().address;
	var port = server.address().port;

	console.log("Server listening at http://%s:%s", host, port);
});