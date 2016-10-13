GET_Handler
==========================================

.. cpp:class:: GET_Handler : public HTTPRequestHandler

   Clase encargada de gestionar las peticiones HTTP de tipo GET.

----------------
Métodos
----------------

.. cpp:function:: GET_Handler(http_request* req)

    Constructor. Llama al constructor de la clase base.

    :param req: Petición HTTP que será gestionada.
    :returns: Devuelve un objeto de tipo GET_Handler.

.. cpp:function:: http_response handleRequest()

    Maneja una petición HTTP de tipo GET.

    :returns: Devuelve un http_response con una respuesta y un código de respuesta HTTP.

.. cpp:function:: http_response handleUsers()

    Busca los usuarios que cumplan con los parámetros específicados en la url.

    :returns: Devuelve un http_response, con los usuarios obtenidos y un código de respuesta HTTP.

.. cpp:function:: http_response handleUserProfile()

    Busca el perfil de un usuario.

    :returns: Devuelve un http_response con información de perfil de un usuario y
              un código de respuesta HTTP.

.. cpp:function:: http_response handleUserSummary()

    Busca el resumen de un usuario.

    :returns: Devuelve un http_response con el resumen de un usuario y un código de respuesta.

.. cpp:function:: http_response handleUserPhoto()

    Busca la foto de un usuario.

    :returns: Devuelve un http_response con "información" de la foto del usuario y un código de respuesta.

.. cpp:function:: http_response handleUserThumb()

    Busca el thumbnail de un usuario.

    :returns: Devuelve un http_response con el thumbnail en texto plano en base64 y un código de respuesta.

.. cpp:function:: http_response handleUserBrief()

    Busca información breve acerca de un usuario

    :returns: Devuelve un http_response información breve de un usuario y un código de respuesta.

.. cpp:function:: http_response handleUsersNotif()

    Busca notificaciones pendientes para un usuario.

    :returns: Devuelve un http_response con información sobre notificaciones pendientes para un usuario y un código de respuesta.

.. cpp:function:: http_response handleUsersNewNotif()

    Busca la cantidad de notificaciones pendientes para un usuario

    :returns: Devuelve un http_response con la cantidad de notificaciones pendientes y un código de respuesta.

.. cpp:function:: http_response handleUsersNotifyRequest()

    Busca una petición en particular.

    :returns: Devuelve un http_response con información sobre la petición (senderID, targetID, message) y un código de respuesta.

.. cpp:function:: http_response handleUsersContacts()

    Busca los contactos de un usuario.

    :returns: Devuelve un http_response con los userID de los contactos del usuario y un código de respuesta.

.. cpp:function:: http_response handleUsersContactManagement()

    Consulta si dos usuarios están conectados entre si.

    :returns: Devuelve un http_response con información de si un usuario es contacto de otro y un código de respuesta.

.. cpp:function:: http_response handlePopular()

    Busca los 10 usuarios más populales.

    :returns: Devuelve un http_response con los userID de los más populares y un código de respuesta.

.. cpp:function:: http_response handlePopularRecommend()

    Consulta si un usuario recomendó a otro.

    :returns: Devuelve un http_response con información de si un usuario recomendó a otro  y un código de respuesta.

.. cpp:function:: http_response handlePopularByPosition()

    Busca los usuarios más populares para una posición dada.

    :returns: Devuelve un http_response con los userID de los encontrados y un código de respuesta.

.. cpp:function:: http_response handlePopularBySkill()

    Busca los usuarios más populares para una skill dada.

    :returns: Devuelve un http_response con los userID de los encontrados y un código de respuesta.

.. cpp:function:: http_response handleChatNew()

    Consulta la cantidad de mensajes nuevos.

    :returns: Devuelve un http_response con todos los mensajes nuevos encontrados y un código de respuesta.

.. cpp:function:: http_response handleChatLastMsg()

    Busca el ID del último mensaje.

    :returns: Devuelve un http_response con el ID del último mensaje y un código de respuesta.

.. cpp:function:: http_response handleChatIncMsg()

    Busca los mensajes comprendidos entre un inicio y fin específicados en la url.

    :returns: Devuelve un http_response con los mensajes comprendidos entre un inicio y fin dado y un código de respuesta.
