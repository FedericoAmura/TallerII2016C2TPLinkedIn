Server
==========================================

.. cpp:class:: Server

   Clase encargada de gestionar las tareas de servidor. Una vez iniciado el
   servidor, empezará a escuchar conexiones y gestionar peticiones. Para detener
   el servidor se debe presionar una tecla (seguida de Enter).

----------------
Miembros
----------------

.. cpp:member::  DB_Handler* Server::db_handler

   Descripción del miembro.

.. cpp:member::  ConnectionsHandler* Server::connectionsHandler

   Descripción del miembro.

----------------
Métodos
----------------

.. cpp:function:: bool settting_ok()

    Chequea si el servidor está en un estado válido para
    luego poder iniciar su servicio.

    :returns: Devuelve true si el servidor fue creado correctamente, sino false.

.. cpp:function:: void run()

    Inicia su servicio.

    :returns: void
