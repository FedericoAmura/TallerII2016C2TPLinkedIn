Server
==========================================

.. cpp:class:: Server

   Clase encargada de gestionar las tareas de servidor. Una vez iniciado el
   servidor, empezará a escuchar conexiones y gestionar peticiones. Para detener
   el servidor se debe presionar una tecla (+ Enter).

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

    Método utilizado para chequear si el servidor está en un estado válido para
    luego poder iniciar el servicio.

    :returns: Devuelve true si el servidor fue creado correctamente, sino false.

.. cpp:function:: void run()

    Método utilizado para iniciar el servidor para poder escuchar conexiones.

    :returns: void
