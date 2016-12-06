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

|   [==========] Running 53 tests from 15 test cases.
|   [----------] Global test environment set-up.
|   [----------] 12 tests from DBJsonTest
|   [ RUN      ] DBJsonTest.testRegistroYLogin
|   [       OK ] DBJsonTest.testRegistroYLogin (23 ms)
|   [ RUN      ] DBJsonTest.testExpiracionDeToken
|   [       OK ] DBJsonTest.testExpiracionDeToken (2016 ms)
|   [ RUN      ] DBJsonTest.testGetSetResumen
|   [       OK ] DBJsonTest.testGetSetResumen (64 ms)
|   [ RUN      ] DBJsonTest.testGetSetFoto
|   [       OK ] DBJsonTest.testGetSetFoto (16 ms)
|   [ RUN      ] DBJsonTest.testGetSetPerfil
|   [       OK ] DBJsonTest.testGetSetPerfil (19 ms)
|   [ RUN      ] DBJsonTest.testBrief
|   [       OK ] DBJsonTest.testBrief (18 ms)
|   [ RUN      ] DBJsonTest.testPeticionesYContactos
|   [       OK ] DBJsonTest.testPeticionesYContactos (19 ms)
|   [ RUN      ] DBJsonTest.testRecomendaciones
|   [       OK ] DBJsonTest.testRecomendaciones (19 ms)
|   [ RUN      ] DBJsonTest.testChat
|   [       OK ] DBJsonTest.testChat (16 ms)
|   [ RUN      ] DBJsonTest.testChatBrief
|   [       OK ] DBJsonTest.testChatBrief (18 ms)
|   [ RUN      ] DBJsonTest.testBusquedaPop
|   [       OK ] DBJsonTest.testBusquedaPop (27 ms)
|   [ RUN      ] DBJsonTest.testBusquedaProf
|   [       OK ] DBJsonTest.testBusquedaProf (21 ms)
|   [----------] 12 tests from DBJsonTest (2276 ms total)
|
|   [----------] 1 test from JsonTest
|   [ RUN      ] JsonTest.TestJsonChecker
|   [       OK ] JsonTest.TestJsonChecker (0 ms)
|   [----------] 1 test from JsonTest (0 ms total)
|
|   [----------] 11 tests from DBRawTest
|   [ RUN      ] DBRawTest.testRegistroYLogin
|   [       OK ] DBRawTest.testRegistroYLogin (16 ms)
|   [ RUN      ] DBRawTest.testGetSetDatos
|   [       OK ] DBRawTest.testGetSetDatos (16 ms)
|   [ RUN      ] DBRawTest.testGetSetResumen
|   [       OK ] DBRawTest.testGetSetResumen (16 ms)
|   [ RUN      ] DBRawTest.testGetSetPuestos
|   [       OK ] DBRawTest.testGetSetPuestos (16 ms)
|   [ RUN      ] DBRawTest.testGetSetSkills
|   [       OK ] DBRawTest.testGetSetSkills (17 ms)
|   [ RUN      ] DBRawTest.testGetSetFoto
|   [       OK ] DBRawTest.testGetSetFoto (52 ms)
|   [ RUN      ] DBRawTest.testContactos
|   [       OK ] DBRawTest.testContactos (16 ms)
|   [ RUN      ] DBRawTest.testRecomendaciones
|   [       OK ] DBRawTest.testRecomendaciones (16 ms)
|   [ RUN      ] DBRawTest.testChat
|   [       OK ] DBRawTest.testChat (16 ms)
|   [ RUN      ] DBRawTest.testBusquedaPopularidad
|   [       OK ] DBRawTest.testBusquedaPopularidad (28 ms)
|   [ RUN      ] DBRawTest.testBusquedaProfesional
|   [       OK ] DBRawTest.testBusquedaProfesional (27 ms)
|   [----------] 11 tests from DBRawTest (236 ms total)
|
|   [----------] 3 tests from FechaTest
|   [ RUN      ] FechaTest.FechaFromString
|   [       OK ] FechaTest.FechaFromString (0 ms)
|   [ RUN      ] FechaTest.FechaFromByteArray
|   [       OK ] FechaTest.FechaFromByteArray (1 ms)
|   [ RUN      ] FechaTest.FechaFromFecha
|   [       OK ] FechaTest.FechaFromFecha (0 ms)
|   [----------] 3 tests from FechaTest (1 ms total)
|
|   [----------] 4 tests from GeoTest
|   [ RUN      ] GeoTest.GeoFromDouble
|   [       OK ] GeoTest.GeoFromDouble (0 ms)
|   [ RUN      ] GeoTest.GeoFromByteArray
|   [       OK ] GeoTest.GeoFromByteArray (0 ms)
|   [ RUN      ] GeoTest.GeoToByteArray
|   [       OK ] GeoTest.GeoToByteArray (0 ms)
|   [ RUN      ] GeoTest.GeoFromGeo
|   [       OK ] GeoTest.GeoFromGeo (0 ms)
|   [----------] 4 tests from GeoTest (0 ms total)
|
|   [----------] 3 tests from DatosTest
|   [ RUN      ] DatosTest.DatosFromParameter
|   [       OK ] DatosTest.DatosFromParameter (0 ms)
|   [ RUN      ] DatosTest.DatosFromByteArray
|   [       OK ] DatosTest.DatosFromByteArray (0 ms)
|   [ RUN      ] DatosTest.DatosToByteArray
|   [       OK ] DatosTest.DatosToByteArray (0 ms)
|   [----------] 3 tests from DatosTest (0 ms total)
|
|   [----------] 2 tests from FotoTest
|   [ RUN      ] FotoTest.DatosFromString
|   [       OK ] FotoTest.DatosFromString (0 ms)
|   [ RUN      ] FotoTest.DatosFromByteArray
|   [       OK ] FotoTest.DatosFromByteArray (1 ms)
|   [----------] 2 tests from FotoTest (1 ms total)
|
|   [----------] 2 tests from PuestoTest
|   [ RUN      ] PuestoTest.DatosFromString
|   [       OK ] PuestoTest.DatosFromString (0 ms)
|   [ RUN      ] PuestoTest.DatosFromByteArray
|   [       OK ] PuestoTest.DatosFromByteArray (0 ms)
|   [----------] 2 tests from PuestoTest (0 ms total)
|
|   [----------] 1 test from JpegTest
|   [ RUN      ] JpegTest.TestResize
|   [       OK ] JpegTest.TestResize (0 ms)
|   [----------] 1 test from JpegTest (0 ms total)
|
|   [----------] 1 test from GCM_ConnectorTest
|   [ RUN      ] GCM_ConnectorTest.hacerPushNotificationConDatosInvalidos
|   JSON_PARSING_ERROR: Unexpected character (d) at position 0.
|   [       OK ] GCM_ConnectorTest.hacerPushNotificationConDatosInvalidos (6919 ms)
|   [----------] 1 test from GCM_ConnectorTest (6919 ms total)
|
|   [----------] 3 tests from LibTest
|   [ RUN      ] LibTest.TestCrearUnaBaseDeDatos
|   [       OK ] LibTest.TestCrearUnaBaseDeDatos (12 ms)
|   [ RUN      ] LibTest.TestJSonBuild
|   [       OK ] LibTest.TestJSonBuild (0 ms)
|   [ RUN      ] LibTest.TestJSonParse
|   [       OK ] LibTest.TestJSonParse (0 ms)
|   [----------] 3 tests from LibTest (12 ms total)
|
|   [----------] 1 test from SampleTest
|   [ RUN      ] SampleTest.ASampleTest
|   [       OK ] SampleTest.ASampleTest (0 ms)
|   [----------] 1 test from SampleTest (1 ms total)
|
|   [----------] 1 test from ServerTest
|   [ RUN      ] ServerTest.IntegrationTests
|   ...starting server on port 8888...
|   ...to stop the server press Ctrl+C ...
|   ^C...shutting down server...
|   [       OK ] ServerTest.IntegrationTests (39467 ms)
|   [----------] 1 test from ServerTest (39468 ms total)
|
|   [----------] 4 tests from SharedServerConnectorTest
|   [ RUN      ] SharedServerConnectorTest.get_categories
|   [       OK ] SharedServerConnectorTest.get_categories (407 ms)
|   [ RUN      ] SharedServerConnectorTest.get_job_positions
|   [       OK ] SharedServerConnectorTest.get_job_positions (5847 ms)
|   [ RUN      ] SharedServerConnectorTest.get_skills
|   [       OK ] SharedServerConnectorTest.get_skills (467 ms)
|   [ RUN      ] SharedServerConnectorTest.invalid_uri
|   [       OK ] SharedServerConnectorTest.invalid_uri (353 ms)
|   [----------] 4 tests from SharedServerConnectorTest (7074 ms total)
|
|   [----------] 4 tests from UtilsTest
|   [ RUN      ] UtilsTest.ValidacionDeUriValida
|   [       OK ] UtilsTest.ValidacionDeUriValida (2 ms)
|   [ RUN      ] UtilsTest.ValidacionDeUriInvalida
|   [       OK ] UtilsTest.ValidacionDeUriInvalida (18 ms)
|   [ RUN      ] UtilsTest.ConvertirMETHODaString
|   [       OK ] UtilsTest.ConvertirMETHODaString (0 ms)
|   [ RUN      ] UtilsTest.SplitSobreUnaCadena
|   [       OK ] UtilsTest.SplitSobreUnaCadena (0 ms)
|   [----------] 4 tests from UtilsTest (20 ms total)
|
|   [----------] Global test environment tear-down
|   [==========] 53 tests from 15 test cases ran. (56008 ms total)
|   [  PASSED  ] 53 tests.


Coverage
===============

Tras tener el ejecutable de tests unitarios, debe ejecutarse coverage.sh desde su directorio de ubicación.

Resultados
++++++++++++

Por ser un documento extenso y html, se espera se lo consulte direcamente en su directorio de creación.
Notar que varios de los archivos que se muestran son librerias, y en realidad el coverage de los archivos propios es superior al 90% si se observa detalladamente.
