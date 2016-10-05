ConnectionsHandler
==========================================

.. cpp:class:: ConnectionsHandler

   Clase encargada de gestionar las conexiones con el servidor.

----------------
Miembros
----------------

.. cpp:member::  ns_mgr* Server::mgr

    Descripción del miembro.

.. cpp:member::  ns_connection Server::connection

    Descripción del miembro.

.. cpp:member::  DB_Handler* Server::db_handler

    Descripción del miembro.

.. cpp:member::  ConnectionsHandler* Server::connectionsHandler

    Descripción del miembro.

----------------
Métodos
----------------

.. cpp:function:: ConnectionsHandler(DB_Handler* db_handler)

    Constructor

    :param db_handler: Handler de las bases de datos.
    :returns: Devuelve un objeto de tipo ConnectionsHandler.

.. cpp:function:: bool isRunning()

    Método utilizado para chequear si el servidor está corriendo (escuchando y gestionando peticiones).

    :returns: Devuelve true si el servidor está corriendo, sino false.

.. cpp:function:: void run()

    Método utilizado para iniciar a escuchar conexiones y atender peticiones.

    :returns: void.

.. cpp:function:: void stop()

    Método utilizado para detener el servicio de conexiones y gestión de peticiones.

    :returns: void.
