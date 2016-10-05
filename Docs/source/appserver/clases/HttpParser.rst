HttpParser
==========================================

.. cpp:class:: HttpParser

   Clase abstracta utilizada para parsear parámetros de una petición http.
   Sólo posee métodos estáticos.

----------------
Métodos estáticos
----------------

.. cpp:function:: struct ns_str* get_header(struct http_message* msg, const char* name)

    Método utilizado para obtener el header asociado al argumento "name" pasado
    como parámetro.

    :param msg: Petición http que se desea procesar.
    :param name: Nombre del header que se quiere obtener.
    :returns: Si se encuentra el header asociado a "name", se devuelve una
              referencia a ese header, sino NULL.

.. cpp:function:: int parse_header(struct http_message* hdr, const char* var_name, char* buffer, size_t buf_size)

    Método utilizado para obtener el valor de una variable si es que se encuentra en el header.

    :param hdr: Header del cual se quiere obtener información.
    :param var_name: Nombre de la variable de la cual se quiere obtener el valor.
    :param buffer: Buffer donde se guardará el valor obtenido.
    :param buf_size: Tamaño del buffer.
    :returns: Retorna 1 si encuentra el nombre de la variable en el header, 0 sino.

.. cpp:function:: bool parse_username_password(struct http_message* msg, std::string &username, std::string &password)

    Método utilizado para parsear el nombre de usuario y su password si es que se encuentran
    en la petición http.

    :param msg: Petición http de la cual se desea obtener información sobre el nombre de usuario y password.
    :param username: Variable donde se guardará el valor de nombre de usuario.
    :param password: Variable donde se guardará el valor de la password.
    :returns: Devuelve true si pudó parsear los valores de las variables, sino false.

.. cpp:function:: bool parse_token(struct http_message* msg, std::string& token)

    Método utilizado para obtener el valor del token.

    :param msg: Petición http de la cual se desea obtener el token de autenticación.
    :param token: Variable donde se guardará el valor del token.
    :returns: Devuelve true si pudó parsear el token, sino false.
