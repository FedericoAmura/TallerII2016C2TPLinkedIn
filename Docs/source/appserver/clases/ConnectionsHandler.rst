ConnectionsHandler
==========================================

.. cpp:class:: ConnectionsHandler

   Clase encargada de gestionar las conexiones con el servidor.

----------------
Miembros
----------------

.. cpp:member::  ns_mgr* Server::mgr

    Manejador de eventos que mantiene todas las conexiones activas.

.. cpp:member::  ns_connection Server::connection

    Describe una conexión.

.. cpp:member::  bool Server::running

    Variable que indica si el handler de conexiones está corriendo.

----------------
Métodos
----------------

.. cpp:function:: ConnectionsHandler(DB_Handler* db_handler)

    Constructor.

    :param db_handler: Handler de las bases de datos.
    :returns: Devuelve un objeto de tipo ConnectionsHandler.

.. cpp:function:: bool isRunning()

    Chequea si el servidor está corriendo (escuchando y gestionando peticiones).

    :returns: Devuelve true si el servidor está corriendo, sino false.

.. cpp:function:: void run()

    Inicia a escuchar conexiones y atender peticiones.

    :returns: void.

.. cpp:function:: void stop()

    Detiene el servicio de conexiones y gestión de peticiones.

    :returns: void.
