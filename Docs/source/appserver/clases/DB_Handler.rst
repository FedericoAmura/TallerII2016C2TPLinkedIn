DB_Handler
==========================================

.. cpp:class:: DB_Handler

   Clase encargada de gestionar todo lo relacionado con bases de datos.

----------------
Métodos
----------------

.. cpp:function:: DB_Handler()

    Constructor.

    :returns: Devuelve un objeto de tipo DB_Handler.

.. cpp:function:: bool validateUserPass(std::string user, std::string pass)

    Validación de username y password.

    :param user: Nombre de usuario a validar.
    :param pass: Password de usuario a validar.
    :returns: Devuelve true si el user y pass son válidos.

.. cpp:function:: std::string generateToken(std::string user, std::string pass)

    Generación de token de sesión a partir de un username y un password.

    :returns: Devuelve un token de sesión.

.. cpp:function:: void registerNewUser(json11::Json data)

    Alta de nuevo usuario.

    :param data: Información del nuevo usuario a dar de alta.
    :returns: void.
    :throws: En caso que datos faltantes se lanza una excepción.
