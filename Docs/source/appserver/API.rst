Especificación de REST API
======================================================================
A continuación se detalla la API para realizar la comunicación entre ClientApp y el AppServer

Notar, el campo AUTH, en caso de ser obligatorio debe contener un token de autenticación.

A excepción del caso de LOGIN, donde contendra username y password.

Versión 1.

.. contents::
   :local:

--------------------------------------------------------
Registro y login
--------------------------------------------------------

Registro de nuevo usuario
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
======================  ===========================================
URL                  	| \http://<appserver>/signup
AUTH                 	| No
VERB                 	| POST
PARAMETROS           	| {
			| “first_name” : “<nombre>”,
			| “last_name” : “<apellido>”,
			| “birth” : “<DD/MM/YYYY>”,
			| “email” : “<email>”,
			| “user_name” : “<nombre de usuario>”,
			| “pass” : “<contraseña>”,
			| “city” : “<ciudad>”,
			| “longitude” : <longitud>,
			| “latitude” : <latitud>
			| }
POSIBLES RESPUESTAS	| 201 - OK (se creo /users/<userID>)
			| 422 - Posible errores:
			|	1. Nombre de usuario no disponible
                        |       2. Email invalido (no implementado)
                        |       3. Fecha malformada
                        |       4. Ubicacion fuera de rango
======================  ===========================================

Login
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Se autentica al usuario contra el servidor, y se le devuelve su token y user ID si es correcto.

Usuario y contraserña se envia en el campo auth.

======================  ===========================================
URL                  	| \http://<appserver>/login
AUTH                 	| Si: username=<mi_username> password=<mi_password>
VERB                 	| POST
PARAMETROS           	|
POSIBLES RESPUESTAS	| 401 - Usuario o pass incorrectos
			| 200 - OK + Siguiente JSON:
			| {
			| “userID” : <userID>,
			| “token” : <token>
			| }
======================  ===========================================

Logout
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Pide al servidor que se cierre la sesión actual. Es decir, que invalide el token.

======================  ===========================================
URL                  	| \http://<appserver>/login
AUTH                 	| Si
VERB                 	| DELETE
PARAMETROS           	| a
POSIBLES RESPUESTAS	| 204 - OK
			| 401 - No autorizado
======================  ===========================================

--------------------------------------------------------
Datos generales de usuario
--------------------------------------------------------

Perfil : Consulta (Salvo foto y resumen)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/users/<UserID>
AUTH                 	| No hace falta
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 404 - UserID Inexistente
			| 200 - OK + Siguiente JSON:
			| {
			|  “name “ : “<nombre>”,
			|  "skills": ["<skillname1>", "<skillname2>"],
			|  "job_positions": [{
			|      "name": "<positionname1>",
			|       “start” : “<DD/MM/YYYY>”,
			|       “end” :  “current”
			|       }, {
			|       "name": "<positionname2>",
			|      “start” : “<DD/MM/YYYY>”,
			|      “end” :  “<DD/MM/YYYY>”
			|      }],
			|  “city” : “<ciudad>”,
 			|  “Contactos” : <numero>
			| }
======================  ==========================

Perfil : Actualizar (Salvo foto y resumen)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/users/<UserID>
AUTH                 	| Si
VERB                 	| PUT
PARAMETROS           	| 404 - UserID Inexistente
			| 200 - OK + Siguiente JSON:
			| {
			|  “name “ : “<nombre>”,
			|  "skills": ["<skillname1>", "<skillname2>"],
			|  "job_positions": [{
			|      "name": "<positionname1>",
			|       “start” : “<DD/MM/YYYY>”,
			|       “end” :  “current”
			|       }, {
			|       "name": "<positionname2>",
			|      “start” : “<DD/MM/YYYY>”,
			|      “end” :  “<DD/MM/YYYY>”
			|      }],
			|  “city” : “<ciudad>”,
 			|  “Contactos” : <numero>
			| }
POSIBLES RESPUESTAS	| 204 - OK
			| 401 - No autorizado
			| 404 - UserID Inexistente
			| 422 - Skills o positions invalidos
======================  ==========================

Resumen : Consulta
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/users/<user_id>/resumen
AUTH                 	| No hace falta
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 404 - UserID Inexistente
			| 200 - OK + Siguiente JSON:
			| {
			| “resumen” : “Bla Bla Bla.“
			| }
======================  ==========================

Resumen : Actualizar
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/users/<user_id>/resumen
AUTH                 	| Si
VERB                 	| PUT
PARAMETROS           	| {
			| “resumen” : “Bla Bla Bla.“
			| }
POSIBLES RESPUESTAS	| 204 - OK
			| 401 - No aurotizado
			| 404 - UserID inexistente
======================  ==========================

Foto : Consulta
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Los datos binarios vienen como texto plano en base64.

======================  ==========================
URL                  	| \http://<appserver>/users/<user_id>/foto
AUTH                 	| No hace falta
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 404 - UserID inexistente
			| 200 - OK + Siguiente JSON:
			| {
			| “foto” = “ab03d7f4c80239b8…”
			| }
======================  ==========================

Foto : Actualizar
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Los datos binarios vienen como texto plano en base64.

======================  ==========================
URL                  	| \http://<appserver>/users/<user_id>/foto
AUTH                 	| Si
VERB                 	| PUT
PARAMETROS           	| {
			| “foto” = “ab03d7f4c80239b8…”
			| }
POSIBLES RESPUESTAS	| 204 - OK
			| 401 - No aurotizado
			| 404 - UserID inexistente
			| 415 - Los datos no corresponden a un jpg
======================  ==========================

Foto : Consulta thumbnail
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Este recurso se autogenera a partir de la foto que envie el usuario.

Los datos binarios vienen como texto plano en base64.

======================  ==========================
URL                  	| \http://<appserver>/users/<user_id>/thumb
AUTH                 	| No hace falta
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 404 - UserID inexistente
			| 200 - OK + Siguiente JSON:
			| {
			| “thumb” = “f0a4b34692d4…”
			| }
======================  ==========================

Datos breves : Consulta
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Util para, por ejemplo, mostrar listas de usuarios.

======================  ==========================
URL                  	| \http://<appserver>/users/<user_id>/brief
AUTH                 	| No hace falta
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 404 - UserID Inexistente
			| 200 - Ok + Siguiente JSON
			| {
			|  “name” : “<nombre>”,
			|  “popularidad“ : <numero>,
			|  “city” : “<ciudad>”,
			|  “thumb” : “f0a4b34692d4...”
			| }
======================  ==========================

--------------------------------------------------------
Búsquedas sobre usuarios
--------------------------------------------------------

======================  ==========================
URL                  	| \http://<appserver>/users/?category=...skill=...&job=...&geoloc=...&distance=...&popsort=..."
AUTH                 	| No hace falta
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 400 - Posible errores:
			|	1. Categoria inexistente
			|	2. Skill inexistente
			|	3. Job inexistente
			|	4. Locacion malformada
			|	5. Distancia <0
			|	6. Popsort != true o false
			| 200 - OK + Siguiente JSON
			| {
			| “users” = [<uid1>, <uid2>, <uid3>, ....]
			| }
======================  ==========================

--------------------------------------------------------
Contactos
--------------------------------------------------------

Consultar peticiones pendientes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/users/<user_id>/notif
AUTH                 	| Si
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 404 - UserID inexistente
			| 401 - No autorizado
			| 200 - OK + Siguiente JSON:
			| {
			|   “pending” : [<senderuid1>, <senderuid2>, …]
			| }
======================  ==========================

Consultar número de peticiones pendientes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/users/<user_id>/notif/new
AUTH                 	| Si
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 404 - UserID inexistente
			| 401 - No autorizado
			| 200 - OK + Siguiente JSON:
			| {
			|   “count” : <numero>
			| }
======================  ==========================

Consultar una petición particular
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/users/<user_id>/notif/<user_id_contacto>
AUTH                 	| Si
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 404 - La peticion no existe
			| 401 - No autorizado
			| 200 - OK + Siguiente JSON:
			| {
			|  “senderID” : <senderUserID>,
			|  “targetID” : <targetUserID>,
			|  “message” : “Bla bla bla”
			| }
======================  ==========================

Aceptar una petición
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/users/<user_id>/notif/<user_id_contacto>
AUTH                 	| Si
VERB                 	| POST
PARAMETROS           	|
POSIBLES RESPUESTAS	| 404 - La peticion no existe
			| 401 - No autorizado
			| 204 - OK
======================  ==========================

Declinar una petición
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/users/<user_id>/notif/<user_id_contacto>
AUTH                 	| Si
VERB                 	| DELETE
PARAMETROS           	|
POSIBLES RESPUESTAS	| 404 - La peticion no existe
			| 401 - No autorizado
			| 204 - OK
======================  ==========================

Consultar contactos
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/users/<user_id>/contacts
AUTH                 	| Si
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 404 - UserID inexistente
			| 401 - No autorizado
			| 200 - OK + Siguiente JSON:
			| {
			| “contacts” : [<userID1>, <userID2>, …]
			| }
======================  ==========================

Crear petición de contacto
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Si existe una petición vieja igual, se la sobreescribe.

======================  ==========================
URL                  	| \http://<appserver>/users/<userID>/contacts
AUTH                 	| Si
VERB                 	| POST
PARAMETROS           	| {
			|  “senderID” : <userID>,
			|  “targetID” : <targetUserID>,
			|  “message” : “Bla bla bla”
			| }
POSIBLES RESPUESTAS	| 404 - Alguno de los usuarios no existe
			| 401 - No autorizado
			| 204 - OK
======================  ==========================

Consulta particular entre dos usuarios
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/users/<userID>/contacts/<targetUserID>
AUTH                 	| Si
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 404 - No son contactos
			| 401 - No autorizado
			| 204 - Son contactos
======================  ==========================

Eliminar contacto
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/users/<userID>/contacts/<targetUserID>
AUTH                 	| Si
VERB                 	| DELETE
PARAMETROS           	|
POSIBLES RESPUESTAS	| 404 - No eran contactos
			| 401 - No autorizado
			| 204 - OK
======================  ==========================

--------------------------------------------------------
Recomendaciones y popularidad
--------------------------------------------------------

Ver si un usuario particular recomendó a otro
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/users/popular/<userID1>/<userID2>
AUTH                 	| No hace falta
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 404 - Alguno de los usuarios no existe
			| 200 - OK + Siguiente JSON:
			| {
			|  “recommender” : <userID1>
			|  “recommendee” : <userID2>
			|  “recommends” : true/false
			| }
======================  ==========================

Recomendar a un usuario
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/users/popular/<userID1>/<userID2>
AUTH                 	| Si
VERB                 	| PUT
PARAMETROS           	| {
			|  “recommender” : <userID1>
			|  “recommendee” : <userID2>
			|  “recommends” : true/false
			| }
POSIBLES RESPUESTAS	| 404 - Alguno de los usuarios no existe
			| 401 - No autorizado
			| 204 - OK
======================  ==========================

Consultar 10 más populares
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/users/popular
AUTH                 	| No hace falta
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 200 - OK + siguiente json:
			| {
			|  “users” : [<userID1>,<userID2>,...]
			| }
======================  ==========================

Consultar 10 más populares por un skill
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/users/popular/skill/<skillName>
AUTH                 	| No hace falta
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 200 - OK + siguiente json:
			| {
			|  “skill” : “<skillName>”
			|  “users” : [<userID1>,<userID2>,...]
			| }
======================  ==========================


Consultar 10 más populares por una posición
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/users/popular/position/<positionName>
AUTH                 	| No hace falta
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 200 - OK + siguiente json:
			| {
			|  “position” : “<positionName>
			|  “users” : [<userID1>,<userID2>,...]
			| }
======================  ==========================

--------------------------------------------------------
Conversaciones
--------------------------------------------------------

Consultar cantidad de mensajes nuevos
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/chat/<userID>/new
AUTH                 	| Si
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 404 - No existe el userID
			| 401 - No autorizado
			| 200 - OK + Siguiente JSON
			| {
			| “new” : [{
			|  “senderUID” : <senderUID>,
			|  “targetUID” : <userID>,
			|  “count” : 5}.{
			|  “senderUID” : <senderUID>,
			|  “targetUID” : <userID>,
			|  “count” : 2} ….  ]
			| }
======================  ==========================

Marcar mensajes como leidos
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Marca todos los de una conversación como leidos.

======================  ==========================
URL                  	| \http://<appserver>/chat/<user_id1>/new
AUTH                 	| Si
VERB                 	| POST
PARAMETROS           	| {
			|  “userUID” : <userID>,
			|  “targetUID” : <targetUID>
			| }
POSIBLES RESPUESTAS	| 404 - No existe el chat
			| 401 - No autorizado
			| 204 - OK
======================  ==========================

Enviar mensaje
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/chat/<user_id1>/<user_id2>
AUTH                 	| Si
VERB                 	| POST
PARAMETROS           	| {
			|  “senderUID” : <userID>,
			|  “targetUID” : <targetUID>,
			|  “message” : “Bla bla bla”
			| }
POSIBLES RESPUESTAS	| 404 - Usuario invalido
			| 401 - No autorizado
			| 201 - OK (Creado recurso /chat/<user_id1>/<user_id2>/?ini=#&fin=#)
======================  ==========================

Consultar número de último mensaje
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/chat/<user_id1>/<user_id2>/last
AUTH                 	| Si
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 401 - No autorizado
			| 404 - No existe algun UserID
			| 200 - OK + Siguiente JSON:
			| {
			|   “lastmsg” : <msgID>
			| }
======================  ==========================

Consultar mensajes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/chat/<user_id1>/<user_id2>/?ini=a&fin=b
AUTH                 	| Si
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 401 - No autorizado
			| 404 - No existe algun UserID
			| 200 - OK + Siguiente JSON:
			| {
			| “messages” : [
			|  {
			|  “senderUID” : <userID>,
			|  “targetUID” : <targetUID>,
			|  “message” : “Bla bla bla”,
			|  “msgID” : 43
			|  },{
			|  “senderUID” : <userID>,
			|  “targetUID” : <targetUID>,
			|  “message” : “Bla bla bla”,
			|  “msgID” : 44
			|  },... ]
			| }
======================  ==========================

--------------------------------------------------------
Categorías / Job Positions / Skills
--------------------------------------------------------

Categorías
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/categories
AUTH                 	| No hace falta
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 200 - OK + Siguiente JSON:
			| {
			|  "categories": [{
			|  "name": "software",
			|  "description": "soft description"
			|  },{
			|  "name": "music",
			|  “description": "all kind of music"
			|  }, … ]
			| }
======================  ==========================

Consultar position por categoría
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/job_positions/category/<category>
AUTH                 	| No hace falta
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 404 - La categoría no existe
			| 200 - OK + Siguiente JSON
			| {
			| "job_positions": [{
			|   "name": "developer",
			|   "description": " a software developer"
			|   "category": "software"
			|   }, {
			|   "name": "project manager",
			|   "description": " a project manager"
			|   "category": "management"
			|   }, ...]
			| }
======================  ==========================

Consultar position particular
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/job_positions/<job_position>
AUTH                 	| No hace falta
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 404 - La position no existe
			| 200 - OK + Siguiente JSON:
			| {
			| "name": "developer",
			| "description": " a software developer"
			| "category": "software"
			| }
======================  ==========================

Consultar skill por categoría
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/skills/category/<category>
AUTH                 	| No hace falta
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 404 - La categoría no existe
			| 200 - OK + Siguiente JSON:
			| {
			| "skills": [{
			|   "name": "c",
			|   "description": "c programming language",
			|   "category": "software"
			|   },{
			|   "name": "PMI",
			|   "description": "Project Management Institute",
			|   "category": "management"
			|  }, ...]
			| }
======================  ==========================

Consultar skill particular
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

======================  ==========================
URL                  	| \http://<appserver>/skills/<skill>
AUTH                 	| No hace falta
VERB                 	| GET
PARAMETROS           	|
POSIBLES RESPUESTAS	| 404 - El skill no existe
			| 200 - OK + Siguiente JSON:
			| {
			|  "name": "c",
			|  "description": "c programming language",
			|  "category": "software"
			| }
======================  ==========================