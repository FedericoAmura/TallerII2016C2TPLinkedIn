Tests y coverage
-----------------------------------------

Se proveen tests unitarios y de integración, los cuales deben ser ejecutados en conjunto para obtener el coverage.

Para el desarrollo de pruebas unitarias se utilizó `Google Test <https://github.com/google/googletest>`_ , una plataforma que permite trabajar en C++.

Se desarrollaron pruebas para la mayoría de las funcionalidades de la aplicación.

Tests de Integración
====================

Se provee el script de python tester/ClientTest.py.

El mismo debe ejecutarse teniendo el server corriendo, y mostrara por consola los resultados de sus tests.

Tests Unitarios
====================

Compilacion y ejecución
++++++++++++

Al ejecutar buildServer.sh se generará en la carpeta bin el ejecutable UnitTests.

Se ejecuta como cualquier ejecutable.

Cerca del final de su ejecución, se levantara el server y se pondra a escuchar, se pretende que en este momento se ejecuten las pruebas de integración para obtener un mayor coverage sin repetir código de testeo innecesario. Su ejecución o no, no alterara los resultados unitarios.

Resultados
++++++++++++

Corrida al momento de la entrega:

|	[==========] Running 55 tests from 19 test cases.
|	[----------] Global test environment set-up.
|	[----------] 1 test from SampleTest
|	[ RUN      ] SampleTest.ASampleTest
|	[       OK ] SampleTest.ASampleTest (0 ms)
|	[----------] 1 test from SampleTest (0 ms total)
|
|	[----------] 1 test from DELETE_HandlerTest
|	[ RUN      ] DELETE_HandlerTest.ManejarPeticionConVerboInvalidoDeberiaDevolverCodigo400
|	[       OK ] DELETE_HandlerTest.ManejarPeticionConVerboInvalidoDeberiaDevolverCodigo400 (0 ms)
|	[----------] 1 test from DELETE_HandlerTest (0 ms total)
|
|	[----------] 1 test from GET_HandlerTest
|	[ RUN      ] GET_HandlerTest.ManejarPeticionConVerboInvalidoDeberiaDevolverCodigo400
|	[       OK ] GET_HandlerTest.ManejarPeticionConVerboInvalidoDeberiaDevolverCodigo400 (0 ms)
|	[----------] 1 test from GET_HandlerTest (0 ms total)
|
|	[----------] 3 tests from SharedServerConnectorTest
|	[ RUN      ] SharedServerConnectorTest.get_categories
|	[       OK ] SharedServerConnectorTest.get_categories (978 ms)
|	[ RUN      ] SharedServerConnectorTest.get_job_positions
|	[       OK ] SharedServerConnectorTest.get_job_positions (506 ms)
|	[ RUN      ] SharedServerConnectorTest.get_skills
|	[       OK ] SharedServerConnectorTest.get_skills (749 ms)
|	[----------] 3 tests from SharedServerConnectorTest (2233 ms total)
|
|	[----------] 11 tests from DBJsonTest
|	[ RUN      ] DBJsonTest.testRegistroYLogin
|	[       OK ] DBJsonTest.testRegistroYLogin (109 ms)
|	[ RUN      ] DBJsonTest.testGetSetResumen
|	[       OK ] DBJsonTest.testGetSetResumen (92 ms)
|	[ RUN      ] DBJsonTest.testGetSetFoto
|	[       OK ] DBJsonTest.testGetSetFoto (118 ms)
|	[ RUN      ] DBJsonTest.testGetSetPerfil
|	[       OK ] DBJsonTest.testGetSetPerfil (94 ms)
|	[ RUN      ] DBJsonTest.testBrief
|	[       OK ] DBJsonTest.testBrief (99 ms)
|	[ RUN      ] DBJsonTest.testPeticionesYContactos
|	[       OK ] DBJsonTest.testPeticionesYContactos (94 ms)
|	[ RUN      ] DBJsonTest.testRecomendaciones
|	[       OK ] DBJsonTest.testRecomendaciones (100 ms)
|	[ RUN      ] DBJsonTest.testChat
|	[       OK ] DBJsonTest.testChat (92 ms)
|	[ RUN      ] DBJsonTest.testChatBrief
|	[       OK ] DBJsonTest.testChatBrief (93 ms)
|	[ RUN      ] DBJsonTest.testBusquedaPop
|	[       OK ] DBJsonTest.testBusquedaPop (100 ms)
|	[ RUN      ] DBJsonTest.testBusquedaProf
|	[       OK ] DBJsonTest.testBusquedaProf (98 ms)
|	[----------] 11 tests from DBJsonTest (1089 ms total)
|
|	[----------] 1 test from JsonTest
|	[ RUN      ] JsonTest.TestJsonChecker
|	[       OK ] JsonTest.TestJsonChecker (0 ms)
|	[----------] 1 test from JsonTest (0 ms total)
|
|	[----------] 11 tests from DBRawTest
|	[ RUN      ] DBRawTest.testRegistroYLogin
|	[       OK ] DBRawTest.testRegistroYLogin (94 ms)
|	[ RUN      ] DBRawTest.testGetSetDatos
|	[       OK ] DBRawTest.testGetSetDatos (92 ms)
|	[ RUN      ] DBRawTest.testGetSetResumen
|	[       OK ] DBRawTest.testGetSetResumen (93 ms)
|	[ RUN      ] DBRawTest.testGetSetPuestos
|	[       OK ] DBRawTest.testGetSetPuestos (93 ms)
|	[ RUN      ] DBRawTest.testGetSetSkills
|	[       OK ] DBRawTest.testGetSetSkills (92 ms)
|	[ RUN      ] DBRawTest.testGetSetFoto
|	[       OK ] DBRawTest.testGetSetFoto (130 ms)
|	[ RUN      ] DBRawTest.testContactos
|	[       OK ] DBRawTest.testContactos (97 ms)
|	[ RUN      ] DBRawTest.testRecomendaciones
|	[       OK ] DBRawTest.testRecomendaciones (83 ms)
|	[ RUN      ] DBRawTest.testChat
|	[       OK ] DBRawTest.testChat (93 ms)
|	[ RUN      ] DBRawTest.testBusquedaPopularidad
|	[       OK ] DBRawTest.testBusquedaPopularidad (104 ms)
|	[ RUN      ] DBRawTest.testBusquedaProfesional
|	[       OK ] DBRawTest.testBusquedaProfesional (107 ms)
|	[----------] 11 tests from DBRawTest (1079 ms total)
|
|	[----------] 3 tests from FechaTest
|	[ RUN      ] FechaTest.FechaFromString
|	[       OK ] FechaTest.FechaFromString (0 ms)
|	[ RUN      ] FechaTest.FechaFromByteArray
|	[       OK ] FechaTest.FechaFromByteArray (0 ms)
|	[ RUN      ] FechaTest.FechaFromFecha
|	[       OK ] FechaTest.FechaFromFecha (0 ms)
|	[----------] 3 tests from FechaTest (0 ms total)
|
|	[----------] 4 tests from GeoTest
|	[ RUN      ] GeoTest.GeoFromDouble
|	[       OK ] GeoTest.GeoFromDouble (0 ms)
|	[ RUN      ] GeoTest.GeoFromByteArray
|	[       OK ] GeoTest.GeoFromByteArray (0 ms)
|	[ RUN      ] GeoTest.GeoToByteArray
|	[       OK ] GeoTest.GeoToByteArray (0 ms)
|	[ RUN      ] GeoTest.GeoFromGeo
|	[       OK ] GeoTest.GeoFromGeo (0 ms)
|	[----------] 4 tests from GeoTest (0 ms total)
|
|	[----------] 3 tests from DatosTest
|	[ RUN      ] DatosTest.DatosFromParameter
|	[       OK ] DatosTest.DatosFromParameter (0 ms)
|	[ RUN      ] DatosTest.DatosFromByteArray
|	[       OK ] DatosTest.DatosFromByteArray (0 ms)
|	[ RUN      ] DatosTest.DatosToByteArray
|	[       OK ] DatosTest.DatosToByteArray (0 ms)
|	[----------] 3 tests from DatosTest (0 ms total)
|
|	[----------] 2 tests from FotoTest
|	[ RUN      ] FotoTest.DatosFromString
|	[       OK ] FotoTest.DatosFromString (1 ms)
|	[ RUN      ] FotoTest.DatosFromByteArray
|	[       OK ] FotoTest.DatosFromByteArray (0 ms)
|	[----------] 2 tests from FotoTest (1 ms total)
|
|	[----------] 2 tests from PuestoTest
|	[ RUN      ] PuestoTest.DatosFromString
|	[       OK ] PuestoTest.DatosFromString (0 ms)
|	[ RUN      ] PuestoTest.DatosFromByteArray
|	[       OK ] PuestoTest.DatosFromByteArray (0 ms)
|	[----------] 2 tests from PuestoTest (0 ms total)
|
|	[----------] 1 test from JpegTest
|	[ RUN      ] JpegTest.TestResize
|	[       OK ] JpegTest.TestResize (0 ms)
|	[----------] 1 test from JpegTest (0 ms total)
|
|	[----------] 1 test from PUT_HandlerTest
|	[ RUN      ] PUT_HandlerTest.ManejarPeticionConVerboInvalidoDeberiaDevolverCodigo400
|	[       OK ] PUT_HandlerTest.ManejarPeticionConVerboInvalidoDeberiaDevolverCodigo400 (0 ms)
|	[----------] 1 test from PUT_HandlerTest (0 ms total)
|
|	[----------] 4 tests from UtilsTest
|	[ RUN      ] UtilsTest.ValidacionDeUriValida
|	[       OK ] UtilsTest.ValidacionDeUriValida (0 ms)
|	[ RUN      ] UtilsTest.ValidacionDeUriInvalida
|	[       OK ] UtilsTest.ValidacionDeUriInvalida (10 ms)
|	[ RUN      ] UtilsTest.ConvertirMETHODaString
|	[       OK ] UtilsTest.ConvertirMETHODaString (0 ms)
|	[ RUN      ] UtilsTest.SplitSobreUnaCadena
|	[       OK ] UtilsTest.SplitSobreUnaCadena (0 ms)
|	[----------] 4 tests from UtilsTest (10 ms total)
|
|	[----------] 3 tests from LibTest
|	[ RUN      ] LibTest.TestCrearUnaBaseDeDatos
|	[       OK ] LibTest.TestCrearUnaBaseDeDatos (67 ms)
|	[ RUN      ] LibTest.TestJSonBuild
|	[       OK ] LibTest.TestJSonBuild (0 ms)
|	[ RUN      ] LibTest.TestJSonParse
|	[       OK ] LibTest.TestJSonParse (0 ms)
|	[----------] 3 tests from LibTest (67 ms total)
|
|	[----------] 1 test from ServerTest
|	[ RUN      ] ServerTest.IntegrationTests
|	...starting server on port 8888...
|	...to stop the server press Ctrl+C ...
|	^C...shutting down server...
|	[       OK ] ServerTest.IntegrationTests (3064 ms)
|	[----------] 1 test from ServerTest (3064 ms total)
|
|	[----------] 1 test from RequestHandlerTest
|	[ RUN      ] RequestHandlerTest.ManejarPeticionConVerboInvalidoDeberiaDevolverCodigo400
|	[       OK ] RequestHandlerTest.ManejarPeticionConVerboInvalidoDeberiaDevolverCodigo400 (0 ms)
|	[----------] 1 test from RequestHandlerTest (0 ms total)
|
|	[----------] 1 test from POST_HandlerTest
|	[ RUN      ] POST_HandlerTest.ManejarPeticionConVerboInvalidoDeberiaDevolverCodigo400
|	[       OK ] POST_HandlerTest.ManejarPeticionConVerboInvalidoDeberiaDevolverCodigo400 (0 ms)
|	[----------] 1 test from POST_HandlerTest (0 ms total)
|
|	[----------] Global test environment tear-down
|	[==========] 55 tests from 19 test cases ran. (7543 ms total)
|	[  PASSED  ] 55 tests.


Coverage
===============

Tras tener el ejecutable de tests unitarios, debe ejecutarse coverage.sh desde su directorio de ubicación.

Resultados
++++++++++++

Por ser un documento extenso y html, se espera se lo consulte direcamente en su directorio de creación.
Notar que varios de los archivos que se muestran son librerias, y en realidad el coverage de los archivos propios es superior al 90% si se observa detalladamente.
